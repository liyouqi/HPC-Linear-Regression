/*
 * gd.c - Gradient Descent implementation
 */

#include "gd.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void gd_serial(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d,
    int iterations,
    double learning_rate
) {
    // Temporary arrays
    double *y_pred = (double *)malloc(n * sizeof(double));
    double *error = (double *)malloc(n * sizeof(double));
    double *gradient = (double *)malloc(d * sizeof(double));
    
    // Initialize beta = 0
    for (int j = 0; j < d; j++) {
        beta[j] = 0.0;
    }
    
    // Iterative optimization
    for (int iter = 0; iter < iterations; iter++) {
        // 1. Compute predictions: y_pred = X * beta
        for (int i = 0; i < n; i++) {
            y_pred[i] = 0.0;
            for (int j = 0; j < d; j++) {
                y_pred[i] += X[i * d + j] * beta[j];
            }
        }
        
        // 2. Compute error: error = y_pred - y
        for (int i = 0; i < n; i++) {
            error[i] = y_pred[i] - y[i];
        }
        
        // 3. Compute gradient: gradient = X^T * error
        for (int j = 0; j < d; j++) {
            gradient[j] = 0.0;
            for (int i = 0; i < n; i++) {
                gradient[j] += X[i * d + j] * error[i];
            }
        }
        
        // 4. Update parameters: beta = beta - (learning_rate / n) * gradient
        double step = learning_rate / n;
        for (int j = 0; j < d; j++) {
            beta[j] -= step * gradient[j];
        }
    }
    
    free(y_pred);
    free(error);
    free(gradient);
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
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    
    // Calculate data distribution for load balancing
    int local_n = n / size + (rank < n % size ? 1 : 0);
    int *sendcounts = NULL;
    int *displs = NULL;
    
    if (rank == 0) {
        sendcounts = (int *)malloc(size * sizeof(int));
        displs = (int *)malloc(size * sizeof(int));
        int offset = 0;
        for (int p = 0; p < size; p++) {
            sendcounts[p] = (n / size + (p < n % size ? 1 : 0)) * d;
            displs[p] = offset;
            offset += sendcounts[p];
        }
    }
    
    // Allocate local data
    double *local_X = (double *)malloc(local_n * d * sizeof(double));
    double *local_y = (double *)malloc(local_n * sizeof(double));
    double *local_y_pred = (double *)malloc(local_n * sizeof(double));
    double *local_error = (double *)malloc(local_n * sizeof(double));
    double *local_gradient = (double *)malloc(d * sizeof(double));
    double *global_beta = (double *)malloc(d * sizeof(double));
    
    // Initialize beta = 0
    for (int j = 0; j < d; j++) {
        global_beta[j] = 0.0;
    }
    
    // Distribute data
    MPI_Scatterv(X, sendcounts, displs, MPI_DOUBLE,
                 local_X, local_n * d, MPI_DOUBLE,
                 0, comm);
    
    // Distribute y (needs separate counts and displs)
    int *y_sendcounts = NULL;
    int *y_displs = NULL;
    if (rank == 0) {
        y_sendcounts = (int *)malloc(size * sizeof(int));
        y_displs = (int *)malloc(size * sizeof(int));
        int offset = 0;
        for (int p = 0; p < size; p++) {
            y_sendcounts[p] = n / size + (p < n % size ? 1 : 0);
            y_displs[p] = offset;
            offset += y_sendcounts[p];
        }
    }
    
    MPI_Scatterv(y, y_sendcounts, y_displs, MPI_DOUBLE,
                 local_y, local_n, MPI_DOUBLE,
                 0, comm);
    
    // Gradient descent iterations
    for (int iter = 0; iter < iterations; iter++) {
        // 1. Broadcast current beta
        MPI_Bcast(global_beta, d, MPI_DOUBLE, 0, comm);
        
        // 2. Compute local predictions: local_y_pred = local_X * beta
        for (int i = 0; i < local_n; i++) {
            local_y_pred[i] = 0.0;
            for (int j = 0; j < d; j++) {
                local_y_pred[i] += local_X[i * d + j] * global_beta[j];
            }
        }
        
        // 3. Compute local error: local_error = local_y_pred - local_y
        for (int i = 0; i < local_n; i++) {
            local_error[i] = local_y_pred[i] - local_y[i];
        }
        
        // 4. Compute local gradient: local_gradient = local_X^T * local_error (row-major traversal)
        for (int j = 0; j < d; j++) {
            local_gradient[j] = 0.0;
        }
        for (int i = 0; i < local_n; i++) {
            for (int j = 0; j < d; j++) {
                local_gradient[j] += local_X[i * d + j] * local_error[i];
            }
        }
        
        // 5. Reduce gradient to rank 0
        if (rank == 0) {
            MPI_Reduce(MPI_IN_PLACE, local_gradient, d, MPI_DOUBLE, MPI_SUM, 0, comm);
        } else {
            MPI_Reduce(local_gradient, NULL, d, MPI_DOUBLE, MPI_SUM, 0, comm);
        }
        
        // 6. Rank 0 updates parameters
        if (rank == 0) {
            double step = learning_rate / n;
            for (int j = 0; j < d; j++) {
                global_beta[j] -= step * local_gradient[j];
            }
        }
    }
    
    // Return result
    if (rank == 0) {
        for (int j = 0; j < d; j++) {
            beta[j] = global_beta[j];
        }
    }
    
    // Clean up
    free(local_X);
    free(local_y);
    free(local_y_pred);
    free(local_error);
    free(local_gradient);
    free(global_beta);
    
    if (rank == 0) {
        free(sendcounts);
        free(displs);
        free(y_sendcounts);
        free(y_displs);
    }
}
