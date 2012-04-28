#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <math.h>

#include "lib.h"
#include "solve.h"

int main(){
	int n, m;

	char n_str[10];
	printf("Enter n (size of matrix): ");
	fgets(n_str, 10, stdin);
	n = atoi(n_str);
	char m_str[10];
	printf("Enter m (block size): ");
	fgets(m_str, 10, stdin);
	m = atoi(m_str);

	double *a = new double[n * n];
	double *result = new double[n * n];

	build(a, n, m);
	print(a, n, m);

	inverse(a, result, n, m);

	fputs("Result:\n", stdout);
	print(result, n, m);

	fputs("Residual calculating...\n", stdout);

	double *product = new double[n * n];

	identity(product, n, m);
	build(a, n, m);
	sub_mult(result, a, product, n, m);

	printf("Residual: %d\n", int(fabs(matrix_norm(product, n, m))));

	delete [] a;
	delete [] result;
	delete [] product;
}
