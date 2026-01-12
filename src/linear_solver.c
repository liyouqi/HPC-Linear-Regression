/*
 * linear_solver.c - Gaussian elimination implementation
 */

#include "linear_solver.h"
#include <stdio.h>
#include <math.h>

int solve_linear_system(double *A, double *b, double *x, int n) {
    const double EPSILON = 1e-12;
    
    // Forward elimination: convert to upper triangular matrix
    for (int k = 0; k < n; k++) {
        // Find pivot (largest element in column k below row k)
        int pivot_row = k;
        double max_val = fabs(A[k * n + k]);
        for (int i = k + 1; i < n; i++) {
            double val = fabs(A[i * n + k]);
            if (val > max_val) {
                max_val = val;
                pivot_row = i;
            }
        }
        
        // Check for singular matrix
        if (max_val < EPSILON) {
            fprintf(stderr, "Error: Matrix is singular or nearly singular\n");
            return -1;
        }
        
        // Swap rows if needed
        if (pivot_row != k) {
            for (int j = 0; j < n; j++) {
                double temp = A[k * n + j];
                A[k * n + j] = A[pivot_row * n + j];
                A[pivot_row * n + j] = temp;
            }
            double temp = b[k];
            b[k] = b[pivot_row];
            b[pivot_row] = temp;
        }
        
        // Eliminate column below pivot
        for (int i = k + 1; i < n; i++) {
            double factor = A[i * n + k] / A[k * n + k];
            for (int j = k; j < n; j++) {
                A[i * n + j] -= factor * A[k * n + j];
            }
            b[i] -= factor * b[k];
        }
    }
    
    // Back substitution
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i * n + j] * x[j];
        }
        x[i] /= A[i * n + i];
    }
    
    return 0; // Success
}
