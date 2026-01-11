/*
 * ols.h - Ordinary Least Squares (OLS) algorithm
 * 
 * Serial and parallel implementations of OLS linear regression
 */

#ifndef OLS_H
#define OLS_H

#include <mpi.h>

/*
 * Serial OLS implementation (for baseline comparison)
 * 
 * Parameters:
 *   X - n x d data matrix
 *   y - n x 1 response vector
 *   beta - d x 1 output parameter vector
 *   n - number of samples
 *   d - number of features
 */
void ols_serial(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d
);

/*
 * Parallel OLS implementation using MPI
 * 
 * Parameters:
 *   X - full data matrix (only on rank 0)
 *   y - full response vector (only on rank 0)
 *   beta - output parameters (computed on rank 0)
 *   n - total number of samples
 *   d - number of features
 *   comm - MPI communicator
 */
void ols_parallel(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d,
    MPI_Comm comm
);

#endif // OLS_H
