#include "block_mult.h"

void block_add_mult(const double *a, const double *b, double *result, int is, int js, int ks){
	for(int i = 0; i != is; ++i){
		for(int j = 0; j != js; ++j){
			double x = a[i * js + j];
			for(int k = 0; k != ks; ++k){
				result[i * ks + k] += x * b[j * ks + k];
			}
		}
	}
}

void block_sub_mult(const double *a, const double *b, double *result, int is, int js, int ks){
	for(int i = 0; i != is; ++i){
		for(int j = 0; j != js; ++j){
			double x = a[i * js + j];
			for(int k = 0; k != ks; ++k){
				result[i * ks + k] -= x * b[j * ks + k];
			}
		}
	}
}

