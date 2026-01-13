/*
 * ols.c - OLS implementation
 */

#include "ols.h"
#include "linear_solver.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void ols_serial(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d
) {
    // Allocate memory for XtX (d x d) and Xty (d x 1)
    double *XtX = (double *)malloc(d * d * sizeof(double));
    double *Xty = (double *)malloc(d * sizeof(double));
    
    // 1. Compute XtX = X^T * X (d x d symmetric matrix)
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            XtX[i * d + j] = 0.0;
            for (int k = 0; k < n; k++) {
                XtX[i * d + j] += X[k * d + i] * X[k * d + j];
            }
        }
    }
    
    // 2. Compute Xty = X^T * y (d x 1 vector)
    for (int i = 0; i < d; i++) {
        Xty[i] = 0.0;
        for (int k = 0; k < n; k++) {
            Xty[i] += X[k * d + i] * y[k];
        }
    }
    
    // 3. Solve linear system: XtX * beta = Xty
    int result = solve_linear_system(XtX, Xty, beta, d);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to solve linear system in OLS\n");
    }
    
    // Clean up
    free(XtX);
    free(Xty);
}

void ols_parallel(
    const double *X,
    const double *y,
    double *beta,
    int n,
    int d,
    MPI_Comm comm
) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    
    // Step 1: Calculate load distribution
    int rows_per_proc = n / size;
    int remainder = n % size;
    int local_n = rows_per_proc + (rank < remainder ? 1 : 0);
    int start_row = rank * rows_per_proc + (rank < remainder ? rank : remainder);
    
    // Step 2: Allocate local data
    double *local_X = (double *)malloc(local_n * d * sizeof(double));
    double *local_y = (double *)malloc(local_n * sizeof(double));
    
    // Step 3: Prepare for Scatterv (only rank 0)
    int *sendcounts = NULL;
    int *displs = NULL;
    if (rank == 0) {
        sendcounts = (int *)malloc(size * sizeof(int));
        displs = (int *)malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) {
            int i_rows = rows_per_proc + (i < remainder ? 1 : 0);
            sendcounts[i] = i_rows * d;
            displs[i] = (i * rows_per_proc + (i < remainder ? i : remainder)) * d;
        }
    }
    
    // Step 4: Distribute X using Scatterv
    MPI_Scatterv(X, sendcounts, displs, MPI_DOUBLE,
                 local_X, local_n * d, MPI_DOUBLE,
                 0, comm);
    
    // Step 5: Prepare for y distribution
    int *sendcounts_y = NULL;
    int *displs_y = NULL;
    if (rank == 0) {
        sendcounts_y = (int *)malloc(size * sizeof(int));
        displs_y = (int *)malloc(size * sizeof(int));
        for (int i = 0; i < size; i++) {
            int i_rows = rows_per_proc + (i < remainder ? 1 : 0);
            sendcounts_y[i] = i_rows;
            displs_y[i] = i * rows_per_proc + (i < remainder ? i : remainder);
        }
    }
    
    // Step 6: Distribute y using Scatterv
    MPI_Scatterv(y, sendcounts_y, displs_y, MPI_DOUBLE,
                 local_y, local_n, MPI_DOUBLE,
                 0, comm);
    
    // // Step 7: Compute local XtX and Xty
    // double *local_XtX = (double *)malloc(d * d * sizeof(double));
    // double *local_Xty = (double *)malloc(d * sizeof(double));
    
    // // Compute local XtX = local_X^T * local_X
    // for (int i = 0; i < d; i++) {
    //     for (int j = 0; j < d; j++) {
    //         local_XtX[i * d + j] = 0.0;
    //         for (int k = 0; k < local_n; k++) {
    //             local_XtX[i * d + j] += local_X[k * d + i] * local_X[k * d + j];
    //         }
    //     }
    // }
    // Step 7: Compute local XtX and Xty
    // Initialize matrices to zero first
    // usage of calloc is safer/cleaner, or manual loop initialization
    double *local_XtX = (double *)calloc(d * d, sizeof(double));
    double *local_Xty = (double *)calloc(d, sizeof(double));

    // Compute local XtX = local_X^T * local_X
    // Optimization: Iterate k (rows) in the outer loop for sequential memory access
    for (int k = 0; k < local_n; k++) {
        for (int i = 0; i < d; i++) {
            // Cache the value from the i-th column of the current row
            double val_i = local_X[k * d + i]; 
            
            // Inner loop updates the i-th row of the result matrix
            for (int j = 0; j < d; j++) {
                local_XtX[i * d + j] += val_i * local_X[k * d + j];
            }
        }
    }

    // Compute local Xty = local_X^T * local_y
    // Optimization: Same strategy, iterate k outside
    for (int k = 0; k < local_n; k++) {
        double y_val = local_y[k]; // Read y value once per row
        for (int i = 0; i < d; i++) {
            local_Xty[i] += local_X[k * d + i] * y_val;
        }
    }
    
    // Compute local Xty = local_X^T * local_y
    for (int i = 0; i < d; i++) {
        local_Xty[i] = 0.0;
        for (int k = 0; k < local_n; k++) {
            local_Xty[i] += local_X[k * d + i] * local_y[k];
        }
    }
    
    // Step 8: Reduce to rank 0
    double *global_XtX = NULL;
    double *global_Xty = NULL;
    if (rank == 0) {
        global_XtX = (double *)malloc(d * d * sizeof(double));
        global_Xty = (double *)malloc(d * sizeof(double));
    }
    
    MPI_Reduce(local_XtX, global_XtX, d * d, MPI_DOUBLE, MPI_SUM, 0, comm);
    MPI_Reduce(local_Xty, global_Xty, d, MPI_DOUBLE, MPI_SUM, 0, comm);
    
    // Step 9: Rank 0 solves the system
    if (rank == 0) {
        int result = solve_linear_system(global_XtX, global_Xty, beta, d);
        if (result != 0) {
            fprintf(stderr, "Error: Failed to solve linear system in parallel OLS\n");
        }
        free(global_XtX);
        free(global_Xty);
    }
    
    // Clean up
    free(local_X);
    free(local_y);
    free(local_XtX);
    free(local_Xty);
    if (rank == 0) {
        free(sendcounts);
        free(displs);
        free(sendcounts_y);
        free(displs_y);
    }
}
