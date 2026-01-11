/*
 * linear_solver.h - Linear system solver
 * 
 * Gaussian elimination for solving linear systems
 */

#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H

/*
 * Solve linear system Ax = b using Gaussian elimination
 * 
 * Parameters:
 *   A - n x n coefficient matrix (will be modified)
 *   b - n x 1 right-hand side vector (will be modified)
 *   x - n x 1 solution vector (output)
 *   n - size of the system
 * 
 * Returns:
 *   0 on success, -1 if matrix is singular
 */
int solve_linear_system(double *A, double *b, double *x, int n);

#endif // LINEAR_SOLVER_H
