#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <err.h>

#include "lib.h"
#include "block_mult.h"

#include "solve.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void sub_mult(const double *a, const double *b, double *result, int n, int m){
	for(int ij_index = 0; ij_index < (n - 1) / m + 1; ++ij_index){
		for(int i_index = 0; i_index < (n - 1) / m + 1; ++i_index){
			int i = i_index * m;
			int is = MIN(m, n - i);

			int j = (i_index + ij_index) % ((n - 1) / m + 1) * m;

			int js = MIN(m, n - j);
			for(int k = 0; k < n; k += m){
				int ks = MIN(m, n - k);
				block_sub_mult(a + i * n + j * is, b + j * n + k * js, result + i * n + k * is, is, js, ks);
			}
		}
	}
}

void inverse(double *a, double *result, int n, int m){
	double *norms = new double[n / m];
	double *block_invs = new double[n < m ? n * n : n / m * m * m];
	double *block_a = new double[m * m];
	double *block_inv;

	identity(result, n, m);

	for(int k = 0; k < n; k += m){
		int ks = MIN(m, n - k);
		if(n - k >= 2 * m){
			for(int i_index = 0; i_index < n / m; ++i_index){
				int i = i_index * m;
				if(i >= k){
					memcpy(block_a, a + i * n + k * m, ks * ks * sizeof(double));
					if(block_inverse(block_a, block_invs + i_index * ks * ks, ks)){
						norms[i_index] = matrix_norm(block_invs + i_index * ks * ks, ks, ks);
					}else{
						norms[i_index] = DBL_MAX;
					}
				}
			}

			double min_norm = DBL_MAX;
			int min_i = -1;

			for(int i = k; i <= n - m; i += m){
				if(norms[i / m] < min_norm){
					min_norm = norms[i / m];
					min_i = i;
				}
			}

			if(min_i == -1){
				errx(1, "I cannot use this method");
			}

			if(min_i != k){
				for(int j = k; j < n; j += m){
					int js = MIN(m, n - j);
					memcpy(block_a,                a + k * n + j * ks,     ks * js * sizeof(double));
					memcpy(a + k * n + j * ks,     a + min_i * n + j * ks, ks * js * sizeof(double));
					memcpy(a + min_i * n + j * ks, block_a,                ks * js * sizeof(double));
				}
				for(int j = 0; j < n; j += m){
					int js = MIN(m, n - j);
					memcpy(block_a,                     result + k * n + j * ks,     ks * js * sizeof(double));
					memcpy(result + k * n + j * ks,     result + min_i * n + j * ks, ks * js * sizeof(double));
					memcpy(result + min_i * n + j * ks, block_a,                     ks * js * sizeof(double));
				}
			}

			block_inv = block_invs + min_i * m;
		}else{
			for(int i = 0; i < n; i += m){
				if(i == k){
					if(!block_inverse(a + k * n + k * ks, block_invs, ks)){
						errx(1, "I cannot use this method");
					}
					block_inv = block_invs;
					break;
				}
			}
		}

		for(int j = 0; j < n; j += m){
			if(j > k){
				int js = MIN(m, n - j);
				memset(block_a, 0, ks * js * sizeof(double));
				block_add_mult(block_inv, a + k * n + j * ks, block_a, ks, ks, js);
				memcpy(a + k * n + j * ks, block_a, ks * js * sizeof(double));
			}
		}

		for(int j = 0; j < n; j += m){
			int js = MIN(m, n - j);
			memset(block_a, 0, ks * js * sizeof(double));
			block_add_mult(block_inv, result + k * n + j * ks, block_a, ks, ks, js);
			memcpy(result + k * n + j * ks, block_a, ks * js * sizeof(double));
		}

		for(int i = 0; i < n; i += m){
			if(i != k){
				int is = MIN(m, n - i);
				for(int j = k + ks; j < n; j += m){
					int js = MIN(m, n - j);
					block_sub_mult(a + i * n + k * is, a + k * n + j * ks, a + i * n + j * is, is, ks, js);
				}
			}
		}

		for(int i = 0 * m; i < n; i += m){
			if(i != k){
				int is = MIN(m, n - i);
				for(int j = 0; j < n; j += m){
					int js = MIN(m, n - j);
					block_sub_mult(a + i * n + k * is, result + k * n + j * ks, result + i * n + j * is, is, ks, js);
				}
			}
		}
	}

	delete [] block_a;
	delete [] norms;
	delete [] block_invs;
}
