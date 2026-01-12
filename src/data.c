/*
 * data.c - Data generation implementation
 */

#include "data.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
 * Generate random number from standard normal distribution
 * Using Box-Muller transform
 */
static double randn() {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;
    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

void generate_synthetic_data(
    double *X,
    double *y,
    double *beta_true,
    int n,
    int d,
    unsigned int seed
) {
    // Set random seed for reproducibility
    srand(seed);
    
    // 1. Generate random X matrix (n x d) from standard normal distribution
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < d; j++) {
            X[i * d + j] = randn();
        }
    }
    
    // 2. Generate random beta_true (d x 1) from uniform distribution [-5, 5]
    // This creates a well-conditioned problem with moderate coefficients
    for (int j = 0; j < d; j++) {
        beta_true[j] = 10.0 * ((double)rand() / RAND_MAX) - 5.0;
    }
    
    // 3. Calculate y = X * beta_true (no noise for exact solution testing)
    for (int i = 0; i < n; i++) {
        y[i] = 0.0;
        for (int j = 0; j < d; j++) {
            y[i] += X[i * d + j] * beta_true[j];
        }
    }
    
    // 4. Add Gaussian noise: y = X * beta + noise
    // Noise level: 0.1 * std(X*beta) for SNR ~= 20dB
    double y_mean = 0.0;
    double y_std = 0.0;
    
    // Calculate mean
    for (int i = 0; i < n; i++) {
        y_mean += y[i];
    }
    y_mean /= n;
    
    // Calculate standard deviation
    for (int i = 0; i < n; i++) {
        y_std += (y[i] - y_mean) * (y[i] - y_mean);
    }
    y_std = sqrt(y_std / n);
    
    // Add noise proportional to signal standard deviation
    double noise_level = 0.1 * y_std;
    for (int i = 0; i < n; i++) {
        y[i] += noise_level * randn();
    }
    
    printf("[Data] Generated synthetic data: n=%d, d=%d, seed=%u\n", n, d, seed);
    printf("[Data] True beta range: [%.2f, %.2f]\n", 
           beta_true[0], beta_true[d-1]);
    printf("[Data] Signal std: %.4f, Noise level: %.4f (SNR ~20dB)\n", 
           y_std, noise_level);
}
