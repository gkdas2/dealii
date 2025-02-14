// ---------------------------------------------------------------------
//
// Copyright (C) 1999 - 2022 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------

#ifndef dealii_sparse_vanka_templates_h
#define dealii_sparse_vanka_templates_h


#include <deal.II/base/config.h>

#include <deal.II/base/memory_consumption.h>
#include <deal.II/base/thread_management.h>

#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/sparse_matrix.h>
#include <deal.II/lac/sparse_vanka.h>
#include <deal.II/lac/vector.h>

#include <algorithm>
#include <map>

DEAL_II_NAMESPACE_OPEN

template <typename number>
SparseVanka<number>::SparseVanka()
  : matrix()
  , selected()
  , inverses()
  , _m(0)
  , _n(0)
{}

template <typename number>
SparseVanka<number>::SparseVanka(const SparseMatrix<number> &M,
                                 const std::vector<bool> &   selected_dofs)
  : matrix(&M, typeid(*this).name())
  , selected(&selected_dofs)
  , inverses(M.m(), nullptr)
  , _m(M.m())
  , _n(M.n())
{
  Assert(M.m() == M.n(), ExcNotQuadratic());
  Assert(M.m() == selected->size(),
         ExcDimensionMismatch(M.m(), selected->size()));

  compute_inverses();
}


template <typename number>
SparseVanka<number>::~SparseVanka()
{
  typename std::vector<
    SmartPointer<FullMatrix<float>, SparseVanka<number>>>::iterator i;
  for (i = inverses.begin(); i != inverses.end(); ++i)
    {
      FullMatrix<float> *p = *i;
      *i                   = nullptr;
      if (p != nullptr)
        delete p;
    }
}


template <typename number>
void
SparseVanka<number>::initialize(const SparseMatrix<number> &M,
                                const AdditionalData &      additional_data)
{
  matrix   = &M;
  selected = &(additional_data.selected);
  inverses.resize(M.m());
  _m = M.m();
  _n = M.n();

  Assert(M.m() == M.n(), ExcNotQuadratic());
  Assert(M.m() == selected->size(),
         ExcDimensionMismatch(M.m(), selected->size()));

  compute_inverses();
}

template <typename number>
void
SparseVanka<number>::compute_inverses()
{
  Assert(matrix != nullptr, ExcNotInitialized());
  Assert(selected != nullptr, ExcNotInitialized());

  const size_type n_inverses =
    std::count(selected->begin(), selected->end(), true);
  // somewhat arbitrarily set up an equal number of tasks as we have threads
  const std::size_t n_tasks = MultithreadInfo::n_threads();
  const size_type   n_inverses_per_task =
    std::max(static_cast<size_type>(n_inverses / n_tasks),
             static_cast<size_type>(1U));

  // set up start and end index
  // for each of the
  // tasks. note that we have
  // to work somewhat to get this
  // appropriate, since the
  // indices for which inverses
  // have to be computed may not
  // be evenly distributed in the
  // vector. as an extreme
  // example consider numbering
  // of DoFs by component, then
  // all indices for which we
  // have to do work will be
  // consecutive, with other
  // consecutive regions where we
  // do not have to do something
  std::vector<std::pair<size_type, unsigned int>> blocking(n_tasks);

  unsigned int c      = 0;
  unsigned int task_n = 0;
  blocking[0].first   = 0;

  for (size_type i = 0; (i < matrix->m()) && (task_n + 1 < n_tasks); ++i)
    {
      if ((*selected)[i] == true)
        ++c;
      if (c == n_inverses_per_task)
        {
          blocking[task_n].second    = i;
          blocking[task_n + 1].first = i;
          ++task_n;

          c = 0;
        }
    }
  blocking[n_tasks - 1].second = matrix->m();

  // Now spawn the tasks
  Threads::TaskGroup<> tasks;
  for (unsigned int i = 0; i < n_tasks; ++i)
    tasks += Threads::new_task([&, i]() {
      this->compute_inverses(blocking[i].first, blocking[i].second);
    });
  tasks.join_all();
}


template <typename number>
void
SparseVanka<number>::compute_inverses(const size_type begin,
                                      const size_type end)
{
  // set-up the vector that will be used
  // by the functions which we call
  // below.
  std::vector<size_type> local_indices;

  // traverse all rows of the matrix
  // which are selected
  for (size_type row = begin; row < end; ++row)
    if ((*selected)[row] == true)
      compute_inverse(row, local_indices);
}



template <typename number>
void
SparseVanka<number>::compute_inverse(const size_type         row,
                                     std::vector<size_type> &local_indices)
{
  Assert(matrix != nullptr, ExcNotInitialized());
  Assert(selected != nullptr, ExcNotInitialized());

  // first define an alias to the sparsity
  // pattern of the matrix, since this
  // will be used quite often
  const SparsityPattern &structure = matrix->get_sparsity_pattern();

  const size_type row_length = structure.row_length(row);

  inverses[row] = new FullMatrix<float>(row_length, row_length);

  // collect the dofs that couple
  // with @p row
  local_indices.resize(row_length);
  for (size_type i = 0; i < row_length; ++i)
    local_indices[i] = structure.column_number(row, i);

  // Build local matrix
  inverses[row]->extract_submatrix_from(*matrix, local_indices, local_indices);

  // Compute inverse
  inverses[row]->gauss_jordan();
}


template <typename number>
template <typename number2>
void
SparseVanka<number>::vmult(Vector<number2> &      dst,
                           const Vector<number2> &src) const
{
  Assert(matrix != nullptr, ExcNotInitialized());
  Assert(selected != nullptr, ExcNotInitialized());

  // first set output vector to zero
  dst = 0;
  // then pass on to the function
  // that actually does the work
  apply_preconditioner(dst, src);
}


template <typename number>
template <typename number2>
void
SparseVanka<number>::apply_preconditioner(
  Vector<number2> &              dst,
  const Vector<number2> &        src,
  const std::vector<bool> *const dof_mask) const
{
  Assert(dst.size() == src.size(),
         ExcDimensionMismatch(dst.size(), src.size()));
  Assert(dst.size() == matrix->m(),
         ExcDimensionMismatch(dst.size(), src.size()));

  // first define an alias to the sparsity
  // pattern of the matrix, since this
  // will be used quite often
  const SparsityPattern &structure = matrix->get_sparsity_pattern();


  // store whether we shall work on
  // the whole matrix, or only on
  // blocks. this variable is used to
  // optimize access to vectors a
  // little bit.
  const bool range_is_restricted = (dof_mask != nullptr);

  // space to be used for local
  // systems. allocate as much memory
  // as is the maximum. this
  // eliminates the need to
  // re-allocate memory inside the
  // loop.
  FullMatrix<float> local_matrix(structure.max_entries_per_row(),
                                 structure.max_entries_per_row());
  Vector<float>     b(structure.max_entries_per_row());
  Vector<float>     x(structure.max_entries_per_row());

  std::map<size_type, size_type> local_index;

  // traverse all rows of the matrix
  // which are selected
  const size_type n = matrix->m();
  for (size_type row = 0; row < n; ++row)
    if (((*selected)[row] == true) &&
        ((range_is_restricted == false) || ((*dof_mask)[row] == true)))
      {
        const size_type row_length = structure.row_length(row);

        b.reinit(row_length);
        x.reinit(row_length);
        // mapping between:
        // 1 column number of all
        //   entries in this row, and
        // 2 the position within this
        //   row (as stored in the
        //   SparsityPattern object
        //
        // since we do not explicitly
        // consider nonsymmetric sparsity
        // patterns, the first element
        // of each entry simply denotes
        // all degrees of freedom that
        // couple with @p row.
        local_index.clear();
        for (size_type i = 0; i < row_length; ++i)
          local_index.insert(
            std::pair<size_type, size_type>(structure.column_number(row, i),
                                            i));

        // Build local matrix and rhs
        for (std::map<size_type, size_type>::const_iterator is =
               local_index.begin();
             is != local_index.end();
             ++is)
          {
            // irow loops over all DoFs that
            // couple with the present DoF
            const size_type irow = is->first;
            // index of DoF irow in the matrix
            // row corresponding to DoF @p row.
            // runs between 0 and row_length
            const size_type i = is->second;

            // copy rhs
            b(i) = src(irow);

            // for all the DoFs that irow
            // couples with
            // number of DoFs coupling to
            // irow (including irow itself)
            for (typename SparseMatrix<number>::const_iterator p =
                   matrix->begin(irow);
                 p != matrix->end(irow);
                 ++p)
              {
                // find out whether this DoF
                // (that couples with @p irow,
                // which itself couples with
                // @p row) also couples with
                // @p row.
                const std::map<size_type, size_type>::const_iterator js =
                  local_index.find(p->column());
                // if not, then still use
                // this dof to modify the rhs
                //
                // note that if so, we already
                // have copied the entry above
                if (js == local_index.end())
                  {
                    if (!range_is_restricted ||
                        ((*dof_mask)[p->column()] == true))
                      b(i) -= p->value() * dst(p->column());
                  }
              }
          }

        // apply preconditioner
        inverses[row]->vmult(x, b);

        // Distribute new values
        for (std::map<size_type, size_type>::const_iterator is =
               local_index.begin();
             is != local_index.end();
             ++is)
          {
            const size_type irow = is->first;
            const size_type i    = is->second;

            if (!range_is_restricted || ((*dof_mask)[irow] == true))
              dst(irow) = x(i);
            // do nothing if not in
            // the range
          }
      }
}



template <typename number>
std::size_t
SparseVanka<number>::memory_consumption() const
{
  std::size_t mem =
    (sizeof(*this) + MemoryConsumption::memory_consumption(*selected));
  for (size_type i = 0; i < inverses.size(); ++i)
    mem += MemoryConsumption::memory_consumption(*inverses[i]);

  return mem;
}



template <typename number>
SparseVanka<number>::AdditionalData::AdditionalData(
  const std::vector<bool> &selected)
  : selected(selected)
{}


//---------------------------------------------------------------------------


template <typename number>
SparseBlockVanka<number>::SparseBlockVanka(
  const SparseMatrix<number> &M,
  const std::vector<bool> &   selected,
  const unsigned int          n_blocks,
  const BlockingStrategy      blocking_strategy)
  : SparseVanka<number>(M, selected)
  , n_blocks(n_blocks)
  , dof_masks(n_blocks, std::vector<bool>(M.m(), false))
{
  compute_dof_masks(M, selected, blocking_strategy);
}


template <typename number>
void
SparseBlockVanka<number>::compute_dof_masks(
  const SparseMatrix<number> &M,
  const std::vector<bool> &   selected,
  const BlockingStrategy      blocking_strategy)
{
  Assert(n_blocks > 0, ExcInternalError());

  const size_type n_inverses =
    std::count(selected.begin(), selected.end(), true);

  const size_type n_inverses_per_block =
    std::max(n_inverses / n_blocks, static_cast<size_type>(1U));

  // precompute the splitting points
  std::vector<std::pair<size_type, size_type>> intervals(n_blocks);

  // set up start and end index for
  // each of the blocks. note that
  // we have to work somewhat to get
  // this appropriate, since the
  // indices for which inverses have
  // to be computed may not be evenly
  // distributed in the vector. as an
  // extreme example consider
  // numbering of DoFs by component,
  // then all indices for which we
  // have to do work will be
  // consecutive, with other
  // consecutive regions where we do
  // not have to do something
  {
    unsigned int c     = 0;
    unsigned int block = 0;
    intervals[0].first = 0;

    for (size_type i = 0; (i < M.m()) && (block + 1 < n_blocks); ++i)
      {
        if (selected[i] == true)
          ++c;
        if (c == n_inverses_per_block)
          {
            intervals[block].second    = i;
            intervals[block + 1].first = i;
            ++block;

            c = 0;
          }
      }
    intervals[n_blocks - 1].second = M.m();
  }

  // now transfer the knowledge on
  // the splitting points into the
  // vector<bool>s that the base
  // class wants to see. the way how
  // we do this depends on the
  // requested blocking strategy
  switch (blocking_strategy)
    {
      case index_intervals:
        {
          for (unsigned int block = 0; block < n_blocks; ++block)
            std::fill_n(dof_masks[block].begin() + intervals[block].first,
                        intervals[block].second - intervals[block].first,
                        true);
          break;
        }

      case adaptive:
        {
          // the splitting points for
          // the DoF have been computed
          // above already, but we will
          // only use them to split the
          // Lagrange dofs into
          // blocks. splitting the
          // remaining dofs will be
          // done now.

          // first count how often the
          // Lagrange dofs of each
          // block access the different
          // dofs
          Table<2, size_type> access_count(n_blocks, M.m());

          // set-up the map that will
          // be used to store the
          // indices each Lagrange dof
          // accesses
          const SparsityPattern &structure = M.get_sparsity_pattern();

          for (size_type row = 0; row < M.m(); ++row)
            if (selected[row] == true)
              {
                // first find out to
                // which block the
                // present row belongs
                unsigned int block_number = 0;
                while (row >= intervals[block_number].second)
                  ++block_number;
                Assert(block_number < n_blocks, ExcInternalError());

                // now traverse the
                // matrix structure to
                // find out to which
                // dofs number the
                // present index wants
                // to write
                const size_type row_length = structure.row_length(row);
                for (size_type i = 0; i < row_length; ++i)
                  ++access_count[block_number][structure.column_number(row, i)];
              }

          // now we know that block @p i
          // wants to write to DoF @p j
          // as often as
          // <tt>access_count[i][j]</tt>
          // times. Let @p j be allotted
          // to the block which
          // accesses it most often.
          //
          // if it is a Lagrange dof,
          // the of course we leave it
          // to the block we put it
          // into in the first place
          for (size_type row = 0; row < M.m(); ++row)
            if (selected[row] == true)
              {
                unsigned int block_number = 0;
                while (row >= intervals[block_number].second)
                  ++block_number;

                dof_masks[block_number][row] = true;
              }
            else
              {
                // find out which block
                // accesses this dof
                // the most often
                size_type    max_accesses     = 0;
                unsigned int max_access_block = 0;
                for (unsigned int block = 0; block < n_blocks; ++block)
                  if (access_count[block][row] > max_accesses)
                    {
                      max_accesses     = access_count[block][row];
                      max_access_block = block;
                    }
                dof_masks[max_access_block][row] = true;
              }

          break;
        }

      default:
        Assert(false, ExcInternalError());
    }
}



template <typename number>
template <typename number2>
void
SparseBlockVanka<number>::vmult(Vector<number2> &      dst,
                                const Vector<number2> &src) const
{
  dst = 0;

  Threads::TaskGroup<> tasks;
  for (unsigned int block = 0; block < n_blocks; ++block)
    tasks += Threads::new_task(
      [&, block] { this->apply_preconditioner(dst, src, &dof_masks[block]); });
  tasks.join_all();
}



template <typename number>
std::size_t
SparseBlockVanka<number>::memory_consumption() const
{
  return SparseVanka<number>::memory_consumption() +
         MemoryConsumption::memory_consumption(dof_masks);
}



DEAL_II_NAMESPACE_CLOSE

#endif
