/*
 * gd.c - Gradient Descent implementation
 */

#include "gd.h"
#include <stdlib.h>
#include <string.h>

void gd_serial(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d,
    int iterations,
    double learning_rate
) {
    // TODO: Implementation
    // For each iteration:
    //   1. Compute predictions: y_pred = X * beta
    //   2. Compute error: error = y_pred - y
    //   3. Compute gradient: grad = X^T * error
    //   4. Update beta: beta = beta - (learning_rate / n) * grad
}

void gd_parallel(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d,
    int iterations,
    double learning_rate,
    MPI_Comm comm
) {
    // TODO: Implementation
    // 1. Distribute data among processes
    // For each iteration:
    //   2. Broadcast current beta
    //   3. Compute local gradient
    //   4. Reduce gradients to rank 0
    //   5. Rank 0 updates beta
}
