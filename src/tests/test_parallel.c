/*
 * test_parallel.c - Test parallel OLS implementation
 * 
 * Compare parallel OLS with serial OLS results
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../data.h"
#include "../ols.h"
#include "../utils.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Test parameters
    int n = 100;
    int d = 5;
    unsigned int seed = 42;
    
    // Allocate memory (all processes need space, but only rank 0 generates)
    double *X = NULL;
    double *y = NULL;
    double *beta_true = NULL;
    double *beta_serial = NULL;
    double *beta_parallel = (double *)malloc(d * sizeof(double));
    
    if (rank == 0) {
        printf("=== Testing Parallel OLS ===\n");
        printf("Problem size: n=%d, d=%d\n", n, d);
        printf("Number of processes: %d\n\n", size);
        
        X = (double *)malloc(n * d * sizeof(double));
        y = (double *)malloc(n * sizeof(double));
        beta_true = (double *)malloc(d * sizeof(double));
        beta_serial = (double *)malloc(d * sizeof(double));
        
        // Generate data
        printf("Generating synthetic data...\n");
        generate_synthetic_data(X, y, beta_true, n, d, seed);
        
        // Compute serial solution for comparison
        printf("Computing serial OLS...\n");
        ols_serial(X, y, beta_serial, n, d);
    }
    
    // Compute parallel solution
    if (rank == 0) {
        printf("Computing parallel OLS with %d processes...\n", size);
    }
    
    ols_parallel(X, y, beta_parallel, n, d, MPI_COMM_WORLD);
    
    // Compare results (only rank 0)
    if (rank == 0) {
        printf("\nResults comparison:\n");
        printf("Serial beta:\n");
        print_vector("beta_serial", beta_serial, d);
        
        printf("\nParallel beta:\n");
        print_vector("beta_parallel", beta_parallel, d);
        
        // Compute difference
        double diff = vector_diff_norm(beta_serial, beta_parallel, d);
        printf("\nDifference ||beta_serial - beta_parallel|| = %.10e\n", diff);
        
        // Check correctness
        if (diff < 1e-10) {
            printf("✓ TEST PASSED: Parallel and serial results match!\n");
        } else if (diff < 1e-6) {
            printf("✓ TEST PASSED: Results match within numerical precision\n");
        } else {
            printf("✗ TEST FAILED: Results differ significantly (%.6e)\n", diff);
        }
        
        // Clean up
        free(X);
        free(y);
        free(beta_true);
        free(beta_serial);
    }
    
    free(beta_parallel);
    
    MPI_Finalize();
    return 0;
}
