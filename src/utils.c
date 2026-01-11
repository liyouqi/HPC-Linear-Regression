/*
 * utils.c - Utility functions implementation
 */

#include "utils.h"
#include <stdio.h>
#include <math.h>

double compute_mse(const double *y_true, const double *y_pred, int n) {
    double mse = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = y_true[i] - y_pred[i];
        mse += diff * diff;
    }
    return mse / n;
}

double vector_diff_norm(const double *v1, const double *v2, int n) {
    double norm = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = v1[i] - v2[i];
        norm += diff * diff;
    }
    return sqrt(norm);
}

void print_vector(const char *name, const double *v, int n) {
    printf("%s = [", name);
    for (int i = 0; i < n; i++) {
        printf("%.6f", v[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

void print_matrix(const char *name, const double *A, int rows, int cols) {
    printf("%s =\n", name);
    for (int i = 0; i < rows; i++) {
        printf("  [");
        for (int j = 0; j < cols; j++) {
            printf("%.6f", A[i * cols + j]);
            if (j < cols - 1) printf(", ");
        }
        printf("]\n");
    }
}
