/*
 * ols.c - OLS implementation
 */

#include "ols.h"
#include "linear_solver.h"
#include <stdlib.h>
#include <string.h>

void ols_serial(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d
) {
    // TODO: Implementation
    // 1. Compute XtX = X^T * X (d x d matrix)
    // 2. Compute Xty = X^T * y (d x 1 vector)
    // 3. Solve linear system: XtX * beta = Xty
}

void ols_parallel(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d,
    MPI_Comm comm
) {
    // TODO: Implementation
    // 1. Distribute data among processes
    // 2. Compute local XtX and Xty
    // 3. Reduce to rank 0
    // 4. Rank 0 solves the system
}
