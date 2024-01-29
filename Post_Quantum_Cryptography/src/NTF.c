/*
 * NTF.c
 *
 *  Created on: Jan 28, 2024
 *      Author: user
 */

#define N 4
#define MODULUS 97
#define W 22
#define SQRT_W 33

// Helper function to compute modular exponentiation
void permute_bitreverse(int data[], int x[]) {
    // Bit-reversal permutation logic
    // This is a simplified example assuming N = 4
    x[0] = data[0];
    x[1] = data[2];
    x[2] = data[1];
    x[3] = data[3];
}
#include <stdio.h>
#include <stdlib.h>

#define MODULUS 97
#define W 22
#define SQRT_W 33
#define N 4

// Helper function to compute modular exponentiation
int mod_exp(int base, int exp, int modulus) {
    int result = 1;
    base = base % modulus;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % modulus;
        exp = exp >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

// Number Theoretic Transform (NTT) function
void ntt(int data[], int forward) {
    int i, s, m, b, op, factor, temp;
    int x[N];

    // Bit-reversal permutation
    // Implementation of this function is assumed to be available
    permute_bitreverse(data, x);

    int factors[N];

    // Compute factors for forward NTT
    if (forward) {
        for (i = 0; i < N; i++) {
            factors[i] = mod_exp(SQRT_W, i, MODULUS);
        }
    }
    // Compute factors for inverse NTT
    else {
        for (i = 0; i < N; i++) {
            factors[i] = mod_exp(-SQRT_W, i, MODULUS);
        }
    }

    // Perform the NTT
    for (s = 1; s <= 2; s++) {
        m = 1 << s;
        for (b = 0; b < N; b += m) {
            factor = 1;
            for (op = 0; op < m / 2; op++) {
                int a0 = x[b + op];
                int a1 = (x[b + op + m / 2] * factor) % MODULUS;
                x[b + op] = (a0 + a1) % MODULUS;
                temp = (a0 - a1) % MODULUS;
                if (temp < 0) temp += MODULUS;
                x[b + op + m / 2] = temp;
                factor = (factor * W) % MODULUS;
            }
        }
    }

    // Apply the factors
    for (i = 0; i < N; i++) {
        x[i] = (x[i] * factors[i]) % MODULUS;
    }

    // Copy x back to data
    for (i = 0; i < N; i++) {
        data[i] = x[i];
    }
}

// Polynomial multiplication in NTT domain
void multiply_polynomials(int poly1[], int poly2[], int result[]) {
    int i;

    // Transform polynomials to NTT domain
    ntt(poly1, 1);
    ntt(poly2, 1);

    // Perform element-wise multiplication
    for (i = 0; i < N; i++) {
        result[i] = (poly1[i] * poly2[i]) % MODULUS;
    }

    // Transform back to polynomial domain
    ntt(result, 0);
}
//
////int main() {
////    int poly1[N] = {2, 3, 5, 7}; // Example polynomial 1 coefficients
////    int poly2[N] = {1, 0, 2, 4}; // Example polynomial 2 coefficients
////    int result[N];
////
////    multiply_polynomials(poly1, poly2, result);
////
////    // Print the result
////    printf("Result of polynomial multiplication: ");
////    for (int i = 0; i < N; i++) {
////        printf("%d ", result[i]);
////    }
////    printf("\n");
////
////    return 0;
////}

