/*
 * main.c - Main program entry point
 * 
 * Parallel Linear Regression using MPI
 * Implements OLS (Ordinary Least Squares) and GD (Gradient Descent)
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (rank == 0) {
        printf("Parallel Linear Regression\n");
        printf("Number of processes: %d\n", size);
    }
    
    // TODO: Implementation
    
    MPI_Finalize();
    return 0;
}
