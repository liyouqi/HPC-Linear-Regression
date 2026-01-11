/*
 * gd.h - Gradient Descent (GD) algorithm
 * 
 * Serial and parallel implementations of iterative gradient descent
 */

#ifndef GD_H
#define GD_H

#include <mpi.h>

/*
 * Serial GD implementation
 * 
 * Parameters:
 *   X - n x d data matrix
 *   y - n x 1 response vector
 *   beta - d x 1 output parameter vector
 *   n - number of samples
 *   d - number of features
 *   iterations - number of iterations (fixed at 10)
 *   learning_rate - step size for gradient descent
 */
void gd_serial(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d,
    int iterations,
    double learning_rate
);

/*
 * Parallel GD implementation using MPI
 * 
 * Parameters:
 *   X - full data matrix (only on rank 0)
 *   y - full response vector (only on rank 0)
 *   beta - output parameters (computed on rank 0)
 *   n - total number of samples
 *   d - number of features
 *   iterations - number of iterations
 *   learning_rate - step size
 *   comm - MPI communicator
 */
void gd_parallel(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d,
    int iterations,
    double learning_rate,
    MPI_Comm comm
);

#endif // GD_H
