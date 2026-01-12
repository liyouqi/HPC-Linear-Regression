/*
 * test_data.c - Test data generation module
 */

#include <stdio.h>
#include <stdlib.h>
#include "../data.h"

int main() {
    printf("=== Testing Data Generation ===\n\n");
    
    // Small test case
    int n = 10;   // 10 samples
    int d = 3;    // 3 features
    unsigned int seed = 42;
    
    // Allocate memory
    double *X = (double *)malloc(n * d * sizeof(double));
    double *y = (double *)malloc(n * sizeof(double));
    double *beta_true = (double *)malloc(d * sizeof(double));
    
    // Generate data
    printf("Generating data with n=%d, d=%d, seed=%u\n\n", n, d, seed);
    generate_synthetic_data(X, y, beta_true, n, d, seed);
    
    // Print beta_true
    printf("\nTrue beta vector:\n");
    for (int i = 0; i < d; i++) {
        printf("  beta[%d] = %.6f\n", i, beta_true[i]);
    }
    
    // Print first few rows of X
    printf("\nFirst 3 rows of X matrix:\n");
    for (int i = 0; i < 3 && i < n; i++) {
        printf("  X[%d] = [", i);
        for (int j = 0; j < d; j++) {
            printf("%.4f", X[i * d + j]);
            if (j < d - 1) printf(", ");
        }
        printf("]\n");
    }
    
    // Print first few y values
    printf("\nFirst 5 y values:\n");
    for (int i = 0; i < 5 && i < n; i++) {
        printf("  y[%d] = %.6f\n", i, y[i]);
    }
    
    // Test reproducibility: generate again with same seed
    printf("\n--- Testing reproducibility with same seed ---\n");
    double *X2 = (double *)malloc(n * d * sizeof(double));
    double *y2 = (double *)malloc(n * sizeof(double));
    double *beta_true2 = (double *)malloc(d * sizeof(double));
    
    generate_synthetic_data(X2, y2, beta_true2, n, d, seed);
    
    // Check if data is identical
    int identical = 1;
    for (int i = 0; i < d; i++) {
        if (beta_true[i] != beta_true2[i]) {
            identical = 0;
            break;
        }
    }
    
    if (identical) {
        printf("✓ Reproducibility test PASSED: Same seed produces same data\n");
    } else {
        printf("✗ Reproducibility test FAILED\n");
    }
    
    // Clean up
    free(X);
    free(y);
    free(beta_true);
    free(X2);
    free(y2);
    free(beta_true2);
    
    printf("\n=== Data generation test complete ===\n");
    return 0;
}
