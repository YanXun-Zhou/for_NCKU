#include <stdio.h>	// for printf function
#include <stdlib.h> // for memory allocation
#include <time.h>	// for time calculation
#include <math.h>	// for sine and cosine functions
int main()
{
	// Declare all the variables
	int k, n, N;
	double *x, *yr, *yi;
	time_t t;

	// Input the number N
	printf("Input the number N : ");
	scanf("%d", &N);

	// Locate the memory for x, yr, yi;
	x = (double *)malloc(N * sizeof(double));
	yr = (double *)malloc(N * sizeof(double));
	yi = (double *)malloc(N * sizeof(double));

	// Initial setting for x, for example, x[k] = k
	for (k = 0; k < N; k++)
	{
		x[k] = k;
		yr[k] = 0;
		yi[k] = 0;
	}

	t = clock();
	// yr[n]+i*yi[n] = sum(exp(-i 2 Pi k n / N)*x[k], k=0..N-1), n=0..N-1
	
    // method 1 : directly compute
    // compute sin or cos N^2 times
    for (n = 0; n < N; n++)
	{
		for (k = 0; k < N; k++)
		{
			yr[n] += cos(2 * M_PI * k * n / N) * x[k];
			yi[n] -= sin(2 * M_PI * k * n / N) * x[k];
		}
	}

    // method 2 : compute by complex number
    // (C_1-iS_1)(C_2-iS_2) = (C_1C_2-S_1S_2) - i(C_1S_2 + C_2S_1dw)

	// output the results
	t = clock() - t;
	printf("%d ms for discrete Fourier Transform of %d elements\n", t, N);

	printf("x = \n");
	printf("[ ");
	for (k = 0; k < N; k++)
	{
		if (k % 10 == 0 && k > 1)
			printf("...\n  ");
		printf(" %.0f ", x[k]);
	}
	printf(" ]\n");

	printf("y = F(x) = \n");
	printf("[ ");
	for (n = 0; n < N; n++)
	{
		if (n % 5 == 0 && n > 1)
			printf("...\n  ");
		printf(" %.2f %+.2fi ", yr[n], yi[n]);
	}
	printf(" ]\n");

	// free the memory located by x, yr, yi
	free(x);
	free(yr);
	free(yi);

	return 100;
}
