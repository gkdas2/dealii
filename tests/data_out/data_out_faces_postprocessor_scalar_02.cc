// ---------------------------------------------------------------------
//
// Copyright (C) 2016 - 2022 by the deal.II authors
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


// tests DataPostprocessor: access the cell we are currently working
// on for a scalar finite element field and DataOutFaces


#include <deal.II/base/function.h>

#include <deal.II/dofs/dof_accessor.h>
#include <deal.II/dofs/dof_handler.h>

#include <deal.II/fe/fe_dgq.h>
#include <deal.II/fe/fe_system.h>

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>

#include <deal.II/lac/vector.h>

#include <deal.II/numerics/data_out_faces.h>
#include <deal.II/numerics/data_postprocessor.h>
#include <deal.II/numerics/matrix_tools.h>
#include <deal.II/numerics/vector_tools.h>

#include "../tests.h"



std::ofstream logfile("output");


template <int dim>
class MyPostprocessor : public DataPostprocessorScalar<dim>
{
public:
  MyPostprocessor()
    : DataPostprocessorScalar<dim>("data", update_values)
  {}

  virtual void
  evaluate_scalar_field(const DataPostprocessorInputs::Scalar<dim> &input_data,
                        std::vector<Vector<double>> &computed_quantities) const
  {
    for (unsigned int q = 0; q < input_data.solution_values.size(); ++q)
      {
        Assert(computed_quantities[q].size() == 1, ExcInternalError());

        // get the cell this all belongs to
        typename DoFHandler<dim>::cell_iterator cell =
          input_data.template get_cell<dim>();

        Assert(input_data.solution_values[q] ==
                 double(cell->active_cell_index()),
               ExcInternalError());

        computed_quantities[q][0] = input_data.solution_values[q];
      }
  }
};



template <int dim>
void
test()
{
  Triangulation<dim> triangulation;
  FE_DGQ<dim>        fe(1);
  DoFHandler<dim>    dof_handler(triangulation);

  GridGenerator::hyper_cube(triangulation, 0, 1);
  triangulation.refine_global(1);
  triangulation.begin_active()->set_refine_flag();
  triangulation.execute_coarsening_and_refinement();

  dof_handler.distribute_dofs(fe);

  // create a vector with piecewise constants, and set the elements of
  // the vector so that on each cell the field has values equal to the
  // cell's active_cell_index
  Vector<double> solution(dof_handler.n_dofs());
  for (typename DoFHandler<dim>::active_cell_iterator cell =
         dof_handler.begin_active();
       cell != dof_handler.end();
       ++cell)
    {
      std::vector<types::global_dof_index> local_dof_indices(
        cell->get_fe().dofs_per_cell);
      cell->get_dof_indices(local_dof_indices);
      for (unsigned int i = 0; i < local_dof_indices.size(); ++i)
        solution(local_dof_indices[i]) = cell->active_cell_index();
    }

  MyPostprocessor<dim> p;
  DataOutFaces<dim>    data_out;
  data_out.attach_dof_handler(dof_handler);
  data_out.add_data_vector(solution, p);
  data_out.build_patches();
  data_out.write_gnuplot(logfile);
}


int
main()
{
  logfile << std::setprecision(2);
  deallog << std::setprecision(2);

  test<2>();
  test<3>();

  return 0;
}
