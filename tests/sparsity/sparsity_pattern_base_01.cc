// ---------------------------------------------------------------------
//
// Copyright (C) 2001 - 2023 by the deal.II authors
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


// Test SparsityPatternBase::add_entries()


#include <deal.II/lac/sparsity_pattern_base.h>

#include "../tests.h"

class TestPattern : public SparsityPatternBase
{
  using SparsityPatternBase::SparsityPatternBase;

  using SparsityPatternBase::size_type;

  virtual void
  add_row_entries(const size_type &                 row,
                  const ArrayView<const size_type> &columns,
                  const bool indices_are_sorted = false) override
  {
    deallog << "row = " << row;

    AssertThrow(std::is_sorted(columns.begin(), columns.end()),
                ExcInternalError());

    for (const auto &col : columns)
      deallog << "    " << col;

    deallog << std::endl;
  }
};



int
main()
{
  initlog();

  std::vector<
    std::pair<SparsityPatternBase::size_type, SparsityPatternBase::size_type>>
    entries;

  for (unsigned int i = 0; i < 100; ++i)
    {
      const auto row = Testing::rand() % 10;
      const auto col = Testing::rand() % 100;
      entries.emplace_back(row, col);
    }

  for (auto &entry : entries)
    deallog << entry.first << ", " << entry.second << std::endl;

  TestPattern test(100, 100);
  test.add_entries(make_array_view(entries));
}
