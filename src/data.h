/*
 * data.h - Data generation module
 * 
 * Functions for generating synthetic data for linear regression
 */

#ifndef DATA_H
#define DATA_H

/*
 * Generate synthetic data for linear regression
 * 
 * Parameters:
 *   X - n x d matrix (output)
 *   y - n x 1 vector (output)
 *   beta_true - d x 1 vector of true parameters (output, for verification)
 *   n - number of samples
 *   d - number of features
 *   seed - random seed for reproducibility
 */
void generate_synthetic_data(
    double *X,
    double *y,
    double *beta_true,
    int n,
    int d,
    unsigned int seed
);

#endif // DATA_H
