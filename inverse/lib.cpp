#include <stdio.h>
#include <string.h>
#include <math.h>
#include <err.h>

#include "lib.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

void build(double *a, int n, int m){
	for(int i = 0; i < n; i += m){
		int is = MIN(m, n - i);
		for(int j = 0; j < n; j += m){
			int js = MIN(m, n - j);
			for(int ii = 0; ii != is; ++ii){
				for(int jj = 0; jj != js; ++jj){
					//a[i * n + j * is + ii * js + jj] = fabs(i + ii - j - jj);
					a[i * n + j * is + ii * js + jj] = 1. / (i + ii + j + jj + 1);
				}
			}
		}
	}
}

#define MAX_PRINT 6

void print(const double *a, int n, int m){
	for(int i = 0; i < n && i < MAX_PRINT; i += m){
		int is = MIN(m, n - i);
		for(int ii = 0; ii != is && i + ii < MAX_PRINT; ++ii){
			for(int j = 0; j < n && j < MAX_PRINT; j += m){
				int js = MIN(m, n - j);
				for(int jj = 0; jj != js && j + jj < MAX_PRINT; ++jj){
					printf(" %d", int(fabs(a[i * n + j * is + ii * js + jj])));
				}
			}
			putchar('\n');
		}
	}

}

void identity(double *a, int n, int m){
	memset(a, 0, n * n * sizeof(double));

	for(int i = 0; i < n; i += m){
		int is = MIN(m, n - i);
		for(int ii = 0; ii != is; ++ii){
			a[i * n + i * is + ii * is + ii] = 1;
		}
	}
}

/* Infinite norm */
double matrix_norm(const double *a, int n, int m){
	double result = 0;
	for(int i = 0; i < n; i += m){
		int is = MIN(m, n - i);
		for(int ii = 0; ii != is; ++ii){
			double s = 0;
			for(int j = 0; j < n; j += m){
				int js = MIN(m, n - j);
				for(int jj = 0; jj != js; ++jj){
					s += fabs(a[i * n + j * is + ii * js + jj]);
				}
			}
			if(s > result){
				result = s;
			}
		}
	}
	return result;
}

/*double matrix_norm(const double *a, int n, int m){
	double result = 0;
	for(int j = 0; j < n; j += m){
		int js = MIN(m, n - j);
		for(int jj = 0; jj != js; ++jj){
			double s = 0;
			for(int i = 0; i < n; i += m){
				int is = MIN(m, n - i);
				for(int ii = 0; ii != is; ++ii){
					s += fabs(a[i * n + j * is + ii * js + jj]);
				}
			}
			if(s > result){
				result = s;
			}
		}
	}
	return result;
}*/

bool block_inverse(double *a, double *result, int n){
	identity(result, n, n);
	for(int k = 0; k < n; ++k){
		double main_item = a[k * n + k];
		int min_i = k;
		for(int i = k + 1; i <= n - 1; ++i){
			if(fabs(a[i * n + k]) > fabs(main_item)){
				main_item = a[i * n + k];
				min_i = i;
			}
		}
		if(fabs(main_item) <= 1e-100){
			return false;
		}
		if(min_i != k){
			for(int j = k; j != n; ++j){
				double swa = a[k * n + j];
				a[k * n + j] = a[min_i * n + j];
				a[min_i * n + j] = swa;
			}
			for(int j = 0; j != n; ++j){
				double swa = result[k * n + j];
				result[k * n + j] = result[min_i * n + j];
				result[min_i * n + j] = swa;
			}
		}
		for(int j = k + 1; j < n; ++j){
			a[k * n + j] /= main_item;
		}
		for(int j = 0; j < n; ++j){
			result[k * n + j] /= main_item;
		}
		for(int i = 0; i < n; ++i){
			if(i != k){
				double block_b = a[i * n + k];
				if(fabs(block_b) >= 1e-100){
					for(int j = k + 1; j < n; ++j){
						a[i * n + j] -= block_b * a[k * n + j];
					}
				}
			}
		}
		for(int i = 0; i < n; ++i){
			if(i != k){
				double block_b = a[i * n + k];
				if(fabs(block_b) >= 1e-100){
					for(int j = 0; j < n; ++j){
						result[i * n + j] -= block_b * result[k * n + j];
					}
				}
			}
		}
	}
	return true;
}
