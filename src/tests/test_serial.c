/*
 * test_serial.c - Test serial OLS implementation
 * 
 * This tests the basic OLS algorithm without MPI
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../data.h"
#include "../ols.h"
#include "../utils.h"

int main() {
    // Small test case
    int n = 100;   // samples
    int d = 5;     // features
    unsigned int seed = 42;
    
    printf("=== Testing Serial OLS ===\n");
    printf("Problem size: n=%d, d=%d\n\n", n, d);
    
    // Allocate memory
    double *X = (double *)malloc(n * d * sizeof(double));
    double *y = (double *)malloc(n * sizeof(double));
    double *beta_true = (double *)malloc(d * sizeof(double));
    double *beta_computed = (double *)malloc(d * sizeof(double));
    
    // Generate synthetic data
    printf("Generating synthetic data...\n");
    generate_synthetic_data(X, y, beta_true, n, d, seed);
    
    // Print true beta
    printf("\nTrue beta:\n");
    print_vector("beta_true", beta_true, d);
    
    // Solve using serial OLS
    printf("\nSolving with serial OLS...\n");
    ols_serial(X, y, beta_computed, n, d);
    
    // Print computed beta
    printf("\nComputed beta:\n");
    print_vector("beta_computed", beta_computed, d);
    
    // Compute error
    double error = vector_diff_norm(beta_true, beta_computed, d);
    printf("\nError ||beta_true - beta_computed|| = %.10e\n", error);
    
    // Check if error is acceptable
    // Note: We added noise (SNR ~20dB), so error should be small but not zero
    if (error < 0.5) {  // Relaxed threshold due to noise
        printf("✓ TEST PASSED: Error is within acceptable range (with noise)\n");
    } else {
        printf("✗ TEST FAILED: Error is too large (%.4f > 0.5)\n", error);
    }
    
    // Clean up
    free(X);
    free(y);
    free(beta_true);
    free(beta_computed);
    
    return 0;
}
