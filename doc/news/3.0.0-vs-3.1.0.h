// ---------------------------------------------------------------------
//
// Copyright (C) 2013, 2014 by the deal.II authors
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

/**
 * @page changes_between_3_0_0_and_3_1_0 Changes between Version 3.0.0 and 3.1.0

<p>
This is the list of changes made between the deal.II releases listed above.
All entries are signed with the names of the author.
</p>



<a name="general"></a>
<h3>General</h3>

<ol>
  <li> <p>
       New: All identifiers that the C++ standard (see
       <a href="https://www.cygnus.com/misc/wp/dec96pub/"
       target="_top"> this page</a> for the public draft -- the
       standard itself is not available online for copyright reasons)
       mandates to be in namespace <code>std::</code> are correctly
       prefixed by the namespace name. While previous gcc compilers
       accepted these names also without the prefix, gcc 3.0 will
       require them, as do some commercial compilers.
       <br>
       (WB 2001/01/16)
       </p>

  <li> <p>
       New: In multithreading mode, the compile flags now also have
       <code class="program">_REENTRANT</code> defined. If this flag
       is defined, then the standard Unix headers also declare
       reentrant versions (with suffix <code class="program">_r</code>)
       of many functions.
       <br>
       (WB 2000/09/20)
       </p>

  <li> <p>
       New: Major and minor version numbers of the
       deal.II library are now passed to the
       compiler as preprocessor variables
       <code class="program">DEAL_2_MAJOR</code> and
       <code class="program">DEAL_2_MINOR</code>. Their present values
       are 3 and 1, respectively. Variables of the same name are also
       available in Makefiles.
       <br>
       (WB 2000/09/18)
       </p>

  <li> <p>
       New: the <code class="program">step-9</code> example program is
       now ready, showing several advanced programming techniques in
       deal.II.
       <br>
       (WB 2000/07/18)
       </p>

  <li> <p>
       Changed: deal.II now uses the
       <code class="program">kdoc2</code> program to generate the API
       documentation. This makes up for much nices documentation and
       also works better than the previous <code class="program">kdoc1</code>.
       A copy of <code class="program">kdoc2</code> is provided within
       the deal.II source tree.
       <br>
       (WB 2000/06/28)
       </p>

  <li> <p>
       Changed: when compiling files, the output generated by the
       <code class="program">make</code> program now indicates also
       whether we are compiling for multithread mode, besides the
       information whether the file is compiled for debug or optimized
       mode.
       <br>
       (Ralf Hartmann 2000/06/26)
       </p>

  <li> <p>
       Fixed: the
       <code class="program">common/scripts/make_dependencies.pl</code>
       script that sets up the dependencies for the make files had a
       problem when the path to the library included special characters
       such as `+'. This is now fixed.
       <br>
       (WB 2000/06/15)
       </p>

  <li> <p>
       New: the <code class="program">configure</code> script now
       checks whether the <code>getrusage</code> function is properly
       declared (this function is used in the timer class). The
       problem is that on SunOS 4.x, this function exists, but is not
       declared in the right file (although it is listed in the man
       pages). We then have to declare it ourselves.
       <br>
       (WB 2000/06/14)
       </p>

  <li> <p>
       New: the <code class="program">configure</code> script now
       enforces the compiler flags <code>-ansi -pedantic</code> when
       compiling in debug mode. This should force us to write more
       standard compliant code. (Since the ACE library is not
       standards conforming, the <code>-ansi</code> flag is not used
       when multithreading is requested.)
       <br>
       (WB 2000/06/14)
       </p>

  <li> <p>
       Improved: the <code class="program">configure</code> script is
       now used to select the compiler options. Previously, selection
       of compiler options was done both in
       <code class="program">configure</code> as well as in the global
       options Makefile
       <code class="program">common/Make.global_options</code>.
       <br>
       (WB 2000/06/02)
       </p>

  <li> <p>
       Improved: Dependence on the files
       <code class="file">forward_declarations.h</code> has been widely
       removed to improve compilation time
       <br>
       (GK 2000/05/24)
       </p>


  <li> <p>
       Fix: <code class="program">configure</code> now uses
       <code class="program">config.guess</code> to determine the
       operating system and hardware platform.
       <br>
       (GK 2000/04/05)
       </p>

  <li> <p>
       Changed: License is now deal.II Public
       License, restricting commercial use more explicitly.
       <br>
       (WB/RH/GK)
       </p>
</ol>



<a name="base"></a>
<h3>base</h3>

<ol>

  <li> <p>
       Improved: exceptions <code>ExcIO</code> and <code
       class="class">ExcOutOfMemory</code> are defined globally now.
       <br>
       (GK 2000/12/07)
       </p>

  <li> <p>
       Improved: <code>ParameterHandler</code> now has a
       function <code>log_parameters</code> which
       allows output of all parameters to a log file.
       <br>
       (GK 2000/12/07)
       </p>

  <li> <p>
       New: almost all classes that store data now have a function
       <code>memory_consumption</code> that returns an
       estimate of the amount of memory (in bytes) used up by this
       class. Supporting functions to compute the size of STL vectors
       and other objects can be found the
       <code>MemoryConsumption</code> namespace.
       <br>
       (WB 2000/11/27)
       </p>

  <li> <p>
       New: Class <code>FunctionDerivative</code>
       computes finite difference approximations of a directional
       derivative of a <code>Function</code>.
       <br>
       (GK 2000/11/13)
       </p>

  <li> <p>
       New: The <code>DataOutBase</code> and
       <code>DataOutBase::Patch</code> classes have been
       changed so as to allow output of objects that have an other
       dimension than the surrounding space, for example writing faces
       instead of cells (this might be useful to write only external
       faces in 3d computations).
       <br>
       (WB 2000/09/07)
       </p>

  <li> <p>
       New: There is a new <code>distance</code> function
       in the <code>Point</code> class that calculates the
       distance between two points.
       <br>
       (Ralf Hartmann 2000/09/06)
       </p>

  <li> <p>
       New: <code>Timer</code> now uses the system
       function <code>getrusage (RUSAGE_CHILDREN,
       .)</code> that is need in multithreading. But still the <code
       class="class">Timer</code> class does not yet work in
       multithreading, as getrusage with flag RUSAGE_CHILDREN gives always
       0 (at least on Solaris7).
       <br>
       (Ralf Hartmann 2000/08/25)
       </p>

  <li> <p>
       New: There are now a set of functions
       <code>outer_product</code> that for the outer
       product of tensors.
       <br>
       (WB 2000/07/23)
       </p>

  <li> <p>
       New: The classes
       <code>Patterns::Integer</code>
       and <code>Patterns::Double</code> now allow that
       a range may be specified in which the parameter shall
       be. Furthermore, instead of a class,
       <code>Patterns</code> is now a namespace, so it
       can be reopened in case you want to write another pattern class
       and want to put it into the same namespace.
       <br>
       (WB 2000/07/21)
       </p>

  <li> <p>
       New: classes <code>QMilne</code> and <code
       class="class">QWeddle</code> for closed Newton-Cotes-formul&aelig; of
       orders 7 and 9, respectively.
       <br>
       (GK 2000/07/07)
       </p>

  <li> <p>
       New: There is now a function <code>invert
       (Tensor<2,dim>)</code> that returns the inverse of a tensor of
       rank 2.
       <br>
       (WB 2000/04/14)
       </p>

  <li> <p>
       New multithreading scheme is implemented.
       <br>
       (WB 2000/04/13)
       </p>

  <li> <p>
       Improved: <code>Subscriptor</code> prints the
       real class name if a subscribed object is deleted.
       <br>
       (GK 2000/04/12)
       </p>
</ol>



<a name="lac"></a>
<h3>lac</h3>

<ol>
  <li> <p>
       Fixed: <code>SparseILU</code> had two bugs which
       are now fixed.
       <br>
       (<a href="mailto:or@winfos.com">Oliver Rheinbach</a>, 2001/02/02)
       </p>

  <li> <p>
       Improved: block classes have a variable number of blocks now,
       not a template parameter.
       <br>
       (GK 2000/12/07)
       </p>

  <li> <p>
       New: <code>BlockDiagonalMatrix</code> is a
       template that generates a matrix with multiple copies of the
       same block on the diagonal.
       <br>
       (GK 2000/12/07)
       </p>

  <li> <p>

       Improved: Krylov-space solvers do not use the function residual
       anymore. This allows easier implementation of new matrix
       classes. <code>SolverGMRES</code> now also counts
       the first iteration step. Finally, all solvers inherit a <code
       class="class">Subscriptor</code>.
       <br>
       (GK 2000/12/07)
       </p>

  <li> <p>
       New: There are now functions
       <code>SparsityPattern::symmetrize</code> and
       <code>SparseMatrix::symmetrize</code> that
       generate a symmetric matrix from a non-symmetric one.
       <br>
       (WB 2000/12/02)
       </p>

  <li> <p>
       New: almost all classes that store data now have a function
       <code>memory_consumption</code> that returns an
       estimate of the amount of memory (in bytes) used up by this
       class.
       <br>
       (WB 2000/11/27)
       </p>

  <li> <p>
       New: <code>
       SparseMatrix<number>::n_actually_nonzero_elements</code>
       returns the number of entries that are actually nonzero.
       <br>
       (Ralf Hartmann 2000/11/22)
       </p>

  <li> <p>
       Fixed: unlike announced in the docs, the
       <code>FullMatrix::norm2</code> function did not
       return the Frobenius norm of a matrix, but its square. This is
       fixed now.
       <br>
       (GK 2000/08/28)
       </p>

  <li> <p>
       Improved: <code>PreconditionBlockSOR::Tvmult()</code> is implemented.
       <br>
       (GK 2000/07/07)
       </p>

  <li> <p>
       Improved: The breakdown criterion of <code
       class="class">SolverBicgstab</code> can be changed by the use
       of <code>SolverBicgstab::AdditionalData</code>.
       <br>
       (GK 2000/07/07)
       </p>

  <li> <p>
       New: <code>SolverRichardson</code> has a
       transposed solver <code>Tsolve(...)</code>. It
       uses the functions <code>Tvmult(...)</code> of
       the provided matrix and preconditioner.
       <br>
       (GK 2000/07/07)
       </p>

  <li> <p>
       Improved: <code>FullMatrix::invert</code>
       now inverts matrices of all
       sizes. If there is no hardcoded inversion, <code
       class="member">gauss_jordan()</code> is used implicitly.
       <br>
       (GK 2000/06/30)
       </p>

  <li> <p>
       New: For recognizing a diverging solver before the maximum
       number of steps is reached,
       <code>SolverControl</code> returns
       <code>failure</code> also if the residual
       increases over the start residual by a specific factor. This
       factor is given to the <code>SolverControl</code>
       object by the <code>set_failure_criterion</code>
       function. After calling the latter function, checking of this
       additional failure criterion may again be disabled by calling
       <code>clear_failure_criterion</code>.
       <br>
       (Ralf Hartmann 2000/06/26)
       </p>

  <li> <p>
       Improved: The interface of preconditioner classes has changed.
       Preconditioners are now considered linear operators like
       matrices: they have members <code>vmult</code>
       and <code>Tvmult</code> instead of the old <code
       class="member">operator()</code>. This will allow the
       implementation of further non-symmetric solvers.
       <br>
       (GK 2000/06/20)
       </p>

  <li> <p>
       New: there is now a function
       <code>SparseMatrix::el</code> that does mostly
       the same as <code>SparseMatrix::operator()</code>,
       but returns a zero if elements of the matrix are accessed that
       are not in the sparsity pattern. Thus, the new function allows
       to actually traverse rows or columns of the matrix without
       taking care of the sparsity pattern, while
       <code>SparseMatrix::operator()</code> should be
       used to write algorithms more efficiently.
       <br>
       (WB 2000/06/02)
       </p>

  <li> <p>
       Fix:
       <code>SparsityPattern::print_gnuplot</code>
       wrote rows and columns exchanged. Since most matrices have
       symmetric sparsity patterns, this has gone unnoticed by now.
       <br>
       (WB 2000/05/30)
       </p>

  <li> <p>
       Fix: the
       <code>FullMatrix::Tvmult</code> function
       suffered from the same problems as the
       <code>FullMatrix::Tmmult</code> function. This
       is now fixed as well.
       <br>
       (WB 2000/05/26)
       </p>

  <li> <p>
       New: Class <code>PreconditionBlockJacobi</code>.
       <br>
       (GK 2000/05/24)
       </p>

  <li> <p>
       New: <code>SolverControl</code> has an interface
       to <code>ParameterHandler</code>, defining and
       reading parameters from a file automatically.
       <br>
       (GK 2000/05/24)
       </p>

  <li> <p>
       New: <code>BlockIndices</code>: Class that
       manages the conversion of global indices into a block
       vector/matrix/... to the indices local to each of the blocks.
       <br>
       (WB 2000/05/08)
       </p>

  <li> <p>
       New: <code>BlockSparsityPattern</code> and
       <code>BlockSparseMatrix</code>: Classes that
       represent matrices that are composed of sparse matrices.
       <br>
       (WB 2000/05/08)
       </p>

  <li> <p>
       Fix: the
       <code>FullMatrix::mmult</code> and
       <code>FullMatrix::Tmmult</code> code don't
       resize their output argument any more, as this is not common
       style in the library. Furthermore,
       <code>FullMatrix::Tmmult</code> was utterly
       broken.
       <br>
       (WB 2000/05/08)
       </p>

  <li> <p>
       Change: the <code>matrix_norm</code> functions
       of sparse and full matrices are renamed to
       <code>matrix_norm_square</code>, since they in
       fact return the square of the norm. This should avoid confusion
       in some cases.
       <br>
       (WB 2000/05/05)
       </p>

  <li> <p>
       Fix: the ``copy-like'' constructor of
       <code>SparsityPattern</code> that copies another
       object and adds some off-diagonals had a bug that caused an
       exception in some cases. This is now fixed.
       <br>
       (WB 2000/05/04)
       </p>

  <li> <p>
       New: <code>SwappableVector</code>: Class that
       allows to swap out the data of a vector to disk and reload it
       later on. It also has a function to preload the data before its
       use in a separate thread if the library is configured for
       multi-threading.
       <br>
       (WB 2000/05/03)
       </p>

  <li> <p>
       New: there are now functions <code>Vector::swap</code>
       and <code>BlockVector::swap</code>, as well as
       global functions <code>swap(u,v)</code> that
       exchange the data of two vectors without needing a temporary
       vector and without copying around data. Their run-time is
       therefore independent of the length of the vectors.
       <br>
       (WB 2000/05/02)
       </p>

  <li> <p>
       Fix: <code>SolverCG</code> counts steps properly.
       <br>
       (GK 2000/04/25)
       </p>

  <li> <p>
       Change: the solver classes in LAC lost their first template
       argument. Their names are now
       <code>SolverXX&lt;VECTOR&gt;</code>, where
       <code>XX</code> denotes the name of the solver
       (e.g. CG, GMRES, etc). Furthermore, the
       inheritance from <code>Solver</code> was made private
       to reflect the logical structure.
       <br>
       (GK 2000/04/25)
       </p>

  <li> <p>
       New: <code>EigenInverse</code> implements inverse
       iteration by Wieland.
       <br>
       (GK 2000/04/20)
       </p>

  <li> <p>
       New: <code>EigenPower</code> implements power
       method by von Mises
       <br>
       (GK 2000/04/19)
       </p>

  <li> <p>
       New: <code>PreconditionBlockSOR::set_omega</code>
       allows to change the relaxation parameter.
       <br>
       (GK 2000/04/12)
       </p>

  <li> <p>
       New: <code>SolverXX</code>: There is a virtual function
       <code>print_vectors</code> called in every step. It is void in the
       solver itself but can be used to print intermediate iteration
       vectors.
       <br>
       (GK 2000/04/05)
       </p>
</ol>



<a name="deal.II"></a>
<h3>deal.II</h3>

<ol>
  <li> <p>
       Extend: <code>DoFTools::extract_boundary_dofs</code>
       now allows to also specify which boundary conditions shall be
       considered.
       <br>
       (WB 2000/12/04)
       </p>

  <li> <p>
       New: some arguments of the functions
       <code>DoFHandler::n_boundary_dofs</code>,
       <code>DoFTools::extract_boundary_dofs</code>,
       and
       <code>DoFTools::map_dof_to_boundary_indices</code>
       are changed from <code>list</code> to
       <code>set</code>, since that resembles more
       closely the purpose of the parameter, and makes computations
       slightly faster.
       <br>
       (WB 2000/12/04)
       </p>

  <li> <p>
       New: almost all classes that store data now have a function
       <code>memory_consumption</code> that returns an
       estimate of the amount of memory (in bytes) used up by this
       class.
       <br>
       (WB 2000/11/27)
       </p>

  <li> <p>
       New: The <code>ConstraintMatrix::add_entries</code>
       function add several constraints at once.
       <br>
       (WB 2000/10/26)
       </p>

  <li> <p>
       New: The <code>DoFRenumbering::random</code>
       function renumbers degrees of freedom in a random way.
       <br>
       (WB 2000/10/22)
       </p>

  <li> <p>
       New: The <code>TriaAccessor</code> now has a
       function <code>point_inside</code> that checks
       whether a certain point is inside a given cell.
       <br>
       (Thomas Richter 2000/10/12)
       </p>

  <li> <p>
       New: The <code>FiniteElement</code> has got two
       new functions <code>transform_unit_to_real_cell
       </code> and <code>transform_real_to_unit_cell
       </code>. They allow to transform points from reference (unit)
       cell to real cell and visa versa. <code
       class="member">transform_real_to_unit_cell</code> involves a
       Newton iteration and works for all dimensions and all mappings
       used to transform from unit to real cell.
       <br>
       (Ralf Hartmann 2000/10/12)
       </p>

  <li> <p>
       Extended: The <code>Triangulation</code>
       class can handle boundary information in 3d as well
       (i.e. lines and quads in 3d with special material IDs).
       <br>
       (<a href="mailto://ms@biomech.tu-graz.ac.at">Michael
        Stadler</a> 2000/10/11)
       </p>

  <li> <p>
       Extended: The <code>GridIn</code>
       class can now read 3D UCD data, including boundary information
       (i.e. lines and quads in 3d with special material IDs).
       <br>
       (<a href="mailto://ms@biomech.tu-graz.ac.at">Michael
        Stadler</a> 2000/10/11)
       </p>

  <li> <p>
       New: The <code>GridRefinement::refine_and_coarsen_optimize</code>
       function implements an alternative way to flag cells for
       refinement and coarsening.
       <br>
       (Thomas Richter 2000/10/10)
       </p>

  <li> <p>
       Extended: The <code>GridIn::delete_unused_vertices</code>
       function now eliminates vertices from the input that are not
       referenced by any of the cells in the input file. This makes is
       simpler to delete some cells from the input file by hand,
       without the need to update the vertex lists, which can be
       tiring as several cells usually use each vertex. All functions
       in the <code>GridIn</code> reading grids in
       several input files call this function before passing the data
       to the triangulation object.
       <br>
       (WB 2000/09/26)
       </p>

  <li> <p>
       New: The <code>GridIn</code>
       class can now read the basics of grids in DB Mesh format.
       <br>
       (WB 2000/09/26)
       </p>

  <li> <p>
       Extended: The <code>KellyErrorEstimator</code>
       class is now able to estimate errors for several solution
       vectors at the same time, provided they live on the same
       <code>DoFHandler</code> object.
       <br>
       (WB 2000/09/11)
       </p>

  <li> <p>
       New: The <code>DataOut_Faces</code> class allows to
       output faces instead of cells. This might be handy for 3d
       computations if one is only interested in surface plots, or
       cuts through the domain.
       <br>
       (WB 2000/09/07)
       </p>

  <li> <p>
       Removed: The
       <code>DataOut_Old</code> class has finally gone for
       good. It was already deprecated in version 3.0, and has been
       superceded for a long time by the framework of classes around
       <code>DataOutBase</code> and
       <code>DataOut</code>.
       <br>
       (WB 2000/09/07)
       </p>

  <li> <p>
       New: There is now a function
       <code>DoFHandler::n_boundary_dofs</code>
       that takes the list of selected boundary indicators as a
       <code>list</code> of values, rather than the
       usual <code>map</code> of pairs of boundary
       indicators and function object pointers.
       <br>
       (WB 2000/08/25)
       </p>

  <li> <p>
       Changed: The
       <code>map_dof_to_boundary_index</code>
       functions have been moved from the <code>DoFHandler</code>
       to the <code>DoFTools</code> class, in order to
       further remove code from the big classes which is necessarily
       needed there.
       <br>
       (WB 2000/08/25)
       </p>

  <li> <p>
       New: there is now a class <code>DataOutRotation</code>
       that can be used to output data which has been computed
       exploiting rotational symmetry, on the original domain. Thus,
       the output is of one dimension higher than the computation was,
       where the computed solution is rotated around the axis of
       symmetry.
       <br>
       (WB 2000/08/14)
       </p>

  <li> <p>
       New: class <code>HalfHyperShellBoundary</code>
       and <code>GridGenerator::half_hyper_shell</code>
       generate a half shell, useful for computations with a shell
       domain and rotational symmetry.
       <br>
       (WB 2000/08/08)
       </p>

  <li> <p>
       Changed: The functions
       <code>Triangulation::refine</code>,
       <code>Triangulation::coarsen</code>,
       <code
       class="member">Triangulation::refine_and_coarsen_fixed_fraction</code>,
       and
       <code
       class="member">Triangulation::refine_and_coarsen_fixed_number</code>
       have been moved from the triangulation class to a separate
       class <code>GridRefinement</code> as they are not
       intricately bound to the triangulation but rather form a
       distinct class of functions that flag cells for refinement or
       coarsening based on error indicators.
       <br>
       (WB 2000/07/28)
       </p>

  <li> <p>
       New: <code>DataOut::clear_input_data_references</code>
       clears all reference to input data vectors and to the <code
       class="class">DoFHandler</code>. This function may be useful
       after the patches have been built, to release memory as early
       as possible, in this case before the output is actually written.
       <br>
       (WB 2000/07/26)
       </p>

  <li> <p>
       New: <code>class DerivativeApproximation</code>
       approximates the norm of the gradient or second derivative of a
       finite element field on each cell from finite difference
       approximations.
       <br>
       (WB 2000/04/14 and 2000/07/23)
       </p>

  <li> <p>
       Fix: Add a missing assertions in <code
       class="member">FEValuesBase::get_function_*</code>. If an <code
       class="class">ExcAccessToUninitializedField</code> is now thrown
       then probably an <code>update_values</code> or
       <code>update_gradients</code> is missing in the
       <code>UpdateFlags</code> of a used <code
       class="class">FEValues</code>. Adding this assertion uncovered
       several other errors which are now also fixed; these errors
       were dormant, since the values of finite elements are always
       computed for the presently available Lagrange elements, but
       would have been activated once there are other classes of
       elements.
       <br>
       (Ralf Hartmann 2000/07/20)
       </p>

  <li> <p>
       New: Class <code>DoFRenumbering</code> now has
       functions for cell-wise downstream renumbering for
       discontinuous elements. The parameters of the function <code
       class="member">sort_selected_dofs_back</code> have been
       switched to match the interfaces of all other functions in this
       class.
       <br>
       (GK 2000 Jour de Bastille)
       </p>

  <li> <p>
       New: Function <code>VectorTools::compute_mean_value()</code> integrates
       the mean value of one component of a finite element function.
       <br>
       (GK 2000/07/12)
       </p>

  <li> <p>
       New: The new function <code>get_face</code>
       of <code>FEFaceValues</code> and <code
       class="class">FESubfaceValues</code> returns an iterator of the
       present face. This is the face, for that the <code
       class="class">FE(Sub)FaceValues</code> object was reinited
       the last time.
       <br>
       (Ralf Hartmann 2000/06/26)
       </p>

  <li> <p>
       New: classes <code>FEDG_Px</code> implement
       complete polynomial spaces of degree <em>x</em> on
       quadrilaterals. Since they have less degrees of freedom than
       <code>FEDG_Qx</code>, there is no continuous
       version for these elements. Implementation for P4 and 3D is
       still incomplete.
       <br>
       (GK 2000/06/20)
       </p>

  <li> <p>
       Fix: slight bug in
       <code>DataOut::build_patches</code>
       in multithreaded mode fixed.
       <br>
       (Ralf Hartmann, 2000/05/29)
       </p>

  <li> <p>
       New: class <code>FETools</code> performs
       interpolations and extrapolations of discrete functions from one
       <code>FiniteElement</code> to another <code
       class="class">FiniteElement</code>. It also provides the local
       interpolation matrices that interpolate on each
       cell. Furthermore it provides the interpolation difference
       matrix id-I<sub>h</sub> that is needed for evaluating
       (id-I<sub>h</sub>)z for e.g. the dual solution z.
       </p>

       <p>
       Removed: The obsolete
       <code>MatrixCreator::create_interpolation_matrix</code>
       function is now removed.
       </p>

       <p>
       (Ralf Hartmann 2000/05/26)
       </p>

  <li> <p>
       New: <code>DoFTools::make_flux_sparsity_pattern</code>
       can be optimized by providing two coefficient matrices.
       <br>
       (GK 2000/05/25)
       </p>

  <li> <p>
       Improved: <code>VectorTools::integrate_difference</code>
       allows for vector-valued weight functions. L<sup>1</sup> and
       L<sup>infinity</sup> norms are calculated correctly.
       <br>
       (GK 2000/05/25)
       </p>

  <li> <p>
       Changed: <code>FE_DGx::restriction</code> is
       not an interpolation any more but a local projection which is
       more reasonable for DG elements.
       <br>
       (Ralf Hartmann 2000/05/22)
       </p>

  <li> <p>
       Changed: enum
       <code>MeshSmoothing</code> is moved into the
       <code>Triangulation</code> class.
       <br>
       (Ralf Hartmann 2000/05/18)
       </p>

  <li> <p>
       New: <code>Triangulation<dim>::patch_level_1</code>
       is a new mesh smoothing. A mesh of patch level 1 consists of
       patches, i.e. they consists of cells that are all refined at
       least once.
       </p>

       <p>
       Changed: As follows from the existence of this new smoothing,
       <code>maximum_smoothing</code> will include this
       smoothing.
       </p>

       <p>
       (Ralf Hartmann 2000/05/18)
       </p>

  <li> <p>
       Changed: the
       <code>MatrixTools::apply_boundary_values</code>
       now uses a much faster algorithm when working on matrices with
       symmetric sparsity patterns. On the other hand, it does no more
       eliminate whole rows when a matrix has a non-symmetric sparsity
       pattern, or if the user (through a new flag) tells the function
       that this is not necessary, for example if the matrix itself is
       non-symmetric.
       </p>

       <p>
       For symmetric sparsity patterns, the algorithm now eliminates
       each boundary value in O(m*log(m)) steps instead of
       O(N*log(m)), where N=number of rows of the matrix, and m=number
       of entries per row. Note that m is roughly constant,
       irrespective of N, so the old algorithm became slower with
       finer grids, while the new one is O(1) for each boundary degree
       of freedom.
       </p>

       <p>
       (John Burnell, WB 2000/05/17)
       </p>

  <li> <p>
       New: many functions are now templatized on the data type of the
       vector they take or return. They thus now support
       <code>Vector&lt;float&gt;</code>, but also
       <code>BlockVector&lt;...&gt;</code>. An
       incomplete and growing list of functions that were treated in
       this way is:
       <ul>
       <li>
       <code>DoFAccessor::get_dof_values</code>,
       <li>
       <code>DoFAccessor::set_dof_values</code>,
       <li>
       <code>DoFAccessor::get_interpolated_dof_values</code>,
       <li>
       <code>DoFAccessor::set_dof_values_by_interpolation</code>,
       <li>
       <code>FEValues::get_function_values</code>,
       <li>
       <code>FEValues::get_function_grads</code>,
       <li>
       <code>FEValues::get_function_2nd_derivatives</code>,
       <li>
       <code>MatrixTools::apply_boundary_values</code>,
       <li>
       Several functions in <code>ConstraintMatrix</code>.
       </ul>
       <br>
       (WB 2000/05/16 and later)
       </p>

  <li> <p>
       Changed: The computation of the Jacobian matrices in the
       <code>FEValues</code> class is now done more
       efficiently. The speedup is in the range of a factor of 40 for
       3D.
       <br>
       (John Burnell, WB 2000/05/16)
       </p>

  <li> <p>
       Change: <code>DoFTools::make_hanging_node_constraints ()</code>
       does not use the user flags any more, and can thus run in
       parallel more than once.
       <br>
       (WB 2000/05/15)
       </p>

  <li> <p>
       Extended: <code>DoFTools::make_sparsity_pattern ()</code>
       now accepts a template parameter as sparsity pattern. This
       allows to use this function for the usual
       <code>SparsityPattern</code>, or for
       <code>BlockSparsityPattern</code> arguments.
       <br>
       (WB 2000/05/15)
       </p>

  <li> <p>
       New: <code>DoFTools::extract_hanging_node_dofs ()</code>
       identifies nodes that will be constrained by hanging node constraints.
       <br>
       (WB 2000/05/12)
       </p>

  <li> <p>
       New: <code>DoFRenumbering::sort_selected_dofs_back ()</code>
       sorts selected degrees of freedom to the end of the index
       range.
       <br>
       (WB 2000/05/12)
       </p>

  <li> <p>
       Change: the return value of
       <code>DoFHandler::max_couplings_between_dofs ()</code>
       is bounded by <code>DoFHandler::n_dofs()</code>.
       <br>
       (Ralf Hartmann 2000/05/11)
       </p>

  <li> <p>
       New: <code>FEValuesBase::get_cell ()</code>
       returns present cell.
       <br>
       (Ralf Hartmann 2000/05/11)
       </p>

  <li> <p>
       Fix: <code>FESystem::reinit()</code> generated
       an exception if <code>update_support_points</code>
       was set.
       <br>
       (WB 2000/05/10)
       </p>

  <li> <p>
       New: <code>IntergridMap::get_{source,destination}_grid</code>
       functions return the grids for which the map was created.
       <br>
       (WB 2000/04/19)
       </p>

  <li> <p>
       Fix: in three space dimensions, the triangulation class
       over-estimated the necessary amount of memory needed upon
       refinement and allocated too much.
       <br>
       (WB 2000/04/19)
       </p>

  <li> <p>
       New: <code>DoFTools::extract_boundary_dofs</code>
       finds all degrees of freedom which are at the boundary and belong to
       specified components.
       <br>
       (WB 2000/04/17)
       </p>

  <li> <p>
       New: <code>DoFTools::compute_intergrid_constraints</code>
       allows to use different discretization grids for different
       variables.
       <br>
       (WB 2000/04/15)
       </p>

  <li> <p>
       New: <code>DataOut::clear_data_vectors</code>
       allows to re-use an object without deleting the <code
       class="class">DoFHandler</code>.
       <br>
       (GK 2000/04/05)
       </p>

  <li> <p>
       New: <code>class DoFPrintSolverStep</code> prints
       intermediate vectors of a solver as finite element functions using
       <code>DataOut</code>.
       <br>
       (GK 2000/04/05)
       </p>

</ol>

*/
