// ---------------------------------------------------------------------
//
// Copyright (C) 1998 - 2022 by the deal.II authors
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

#include <deal.II/base/graph_coloring.h>

#include <deal.II/lac/sparsity_tools.h>

DEAL_II_NAMESPACE_OPEN

namespace GraphColoring
{
  unsigned int
  color_sparsity_pattern(const SparsityPattern &    sparsity_pattern,
                         std::vector<unsigned int> &color_indices)
  {
    return SparsityTools::color_sparsity_pattern(sparsity_pattern,
                                                 color_indices);
  }
} // namespace GraphColoring

DEAL_II_NAMESPACE_CLOSE
