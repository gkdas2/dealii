## ---------------------------------------------------------------------
##
## Copyright (C) 2012 - 2022 by the deal.II authors
##
## This file is part of the deal.II library.
##
## The deal.II library is free software; you can use it, redistribute
## it, and/or modify it under the terms of the GNU Lesser General
## Public License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
## The full text of the license can be found in the file LICENSE.md at
## the top level directory of deal.II.
##
## ---------------------------------------------------------------------

#
# Configure config.h and revision.h
#
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/deal.II/base/config.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/deal.II/base/config.h
  )
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/deal.II/base/revision.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/deal.II/base/revision.h
  )

#
# Add a rule for how to install the header files:
#
install(DIRECTORY deal.II
  DESTINATION ${DEAL_II_INCLUDE_RELDIR}
  COMPONENT library
  FILES_MATCHING PATTERN "*.h"
  )

#
# and don't forget to install all generated header files, too:
#
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/deal.II
  DESTINATION ${DEAL_II_INCLUDE_RELDIR}
  COMPONENT library
  FILES_MATCHING PATTERN "*.h"
  )
