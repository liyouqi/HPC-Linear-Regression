/*
 * utils.h - Utility functions
 * 
 * Helper functions for timing, validation, and output
 */

#ifndef UTILS_H
#define UTILS_H

/*
 * Compute Mean Squared Error
 */
double compute_mse(const double *y_true, const double *y_pred, int n);

/*
 * Compute norm of difference between two vectors
 */
double vector_diff_norm(const double *v1, const double *v2, int n);

/*
 * Print vector (for debugging)
 */
void print_vector(const char *name, const double *v, int n);

/*
 * Print matrix (for debugging)
 */
void print_matrix(const char *name, const double *A, int rows, int cols);

#endif // UTILS_H
