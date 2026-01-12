/*
 * main.c - Main program entry point
 * 
 * Parallel Linear Regression using MPI
 * Implements OLS (Ordinary Least Squares) and GD (Gradient Descent)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "src/data.h"
#include "src/ols.h"
#include "src/gd.h"
#include "src/utils.h"

void print_usage(const char *prog_name) {
    printf("Usage: %s [OPTIONS]\n", prog_name);
    printf("Options:\n");
    printf("  -a <algorithm>  Algorithm: ols or gd (default: ols)\n");
    printf("  -n <samples>    Number of samples (default: 100000)\n");
    printf("  -d <features>   Number of features (default: 100)\n");
    printf("  -s <seed>       Random seed (default: 42)\n");
    printf("  -i <iterations> GD iterations (default: 1000)\n");
    printf("  -l <lr>         GD learning rate (default: 0.01)\n");
    printf("  -h              Show this help message\n");
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Default parameters
    char algorithm[10] = "ols";
    int n = 100000;  // Large dataset for performance testing
    int d = 100;
    unsigned int seed = 42;
    int gd_iterations = 1000;
    double gd_learning_rate = 0.01;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            strncpy(algorithm, argv[++i], sizeof(algorithm) - 1);
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            n = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            d = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            seed = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            gd_iterations = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            gd_learning_rate = atof(argv[++i]);
        } else if (strcmp(argv[i], "-h") == 0) {
            if (rank == 0) print_usage(argv[0]);
            MPI_Finalize();
            return 0;
        }
    }
    
    // Validate algorithm choice
    int use_gd = 0;
    if (strcmp(algorithm, "gd") == 0) {
        use_gd = 1;
    } else if (strcmp(algorithm, "ols") != 0) {
        if (rank == 0) {
            fprintf(stderr, "Error: Unknown algorithm '%s'. Use 'ols' or 'gd'.\n", algorithm);
        }
        MPI_Finalize();
        return 1;
    }
    
    // Print configuration (rank 0 only)
    if (rank == 0) {
        printf("=== Parallel Linear Regression (%s) ===\n", use_gd ? "GD" : "OLS");
        printf("Problem size: n=%d, d=%d\n", n, d);
        printf("Random seed: %u\n", seed);
        if (use_gd) {
            printf("GD iterations: %d\n", gd_iterations);
            printf("Learning rate: %.6f\n", gd_learning_rate);
        }
        printf("MPI processes: %d\n", size);
        printf("=========================================\n\n");
    }
    
    // Allocate memory (only rank 0 generates data)
    double *X = NULL;
    double *y = NULL;
    double *beta_true = NULL;
    double *beta = (double *)malloc(d * sizeof(double));
    
    if (rank == 0) {
        X = (double *)malloc(n * d * sizeof(double));
        y = (double *)malloc(n * sizeof(double));
        beta_true = (double *)malloc(d * sizeof(double));
        
        if (!X || !y || !beta_true) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        
        // Generate synthetic data
        printf("[Rank 0] Generating synthetic data...\n");
        generate_synthetic_data(X, y, beta_true, n, d, seed);
        printf("[Rank 0] Data generation complete.\n\n");
    }
    
    // Synchronize before timing
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();
    
    // Execute chosen algorithm
    if (use_gd) {
        gd_parallel(X, y, beta, n, d, gd_iterations, gd_learning_rate, MPI_COMM_WORLD);
    } else {
        ols_parallel(X, y, beta, n, d, MPI_COMM_WORLD);
    }
    
    // Synchronize after computation
    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;
    
    // Report results (rank 0 only)
    if (rank == 0) {
        printf("\n=== Results ===\n");
        printf("Execution time: %.6f seconds\n", elapsed_time);
        
        // Print first few beta coefficients
        printf("\nComputed beta (first 5):\n");
        int print_d = (d < 5) ? d : 5;
        for (int i = 0; i < print_d; i++) {
            printf("  beta[%d] = %.6f\n", i, beta[i]);
        }
        
        // Compute error against true beta
        if (beta_true) {
            double error = vector_diff_norm(beta_true, beta, d);
            printf("\nError ||beta_true - beta_computed|| = %.6e\n", error);
        }
        
        // Output timing data in CSV format for analysis
        printf("\n=== CSV Output ===\n");
        printf("algorithm,n,d,processes,time_seconds\n");
        printf("%s,%d,%d,%d,%.6f\n", algorithm, n, d, size, elapsed_time);
        
        // Clean up
        free(X);
        free(y);
        free(beta_true);
    }
    
    free(beta);
    
    MPI_Finalize();
    return 0;
}
