#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 10000

int main(int argc, char const *argv[])
{
    int n, i, j;
    double **A, *x, *b;
    time_t t;
    for (n = 1000-1; n < N; n+=1000)
    {
        x = (double *)malloc(N * sizeof(double));
        b = (double *)malloc(N * sizeof(double));
        A = (double **)malloc(N * sizeof(double *));
        for (i = 0; i < N; ++i)
        {
            A[i] = (double *)malloc(N * sizeof(double));
        }

        // Set A and x
        srand(time(NULL));
        for (i = 0; i < n; i++)
        {
            x[i] = 1.0 * rand() / RAND_MAX;
            for (j = 0; j < n; j++)
            {
                A[i][j] = 1.0 * rand() / RAND_MAX;
            }
        }

        // Compute Ax=b
        t = clock();
        for (i = 0; i < n; i++)
        {
            b[i] = 0.0;
            for (j = 0; j < n; ++j)
            {
                b[i] += A[i][j] * x[j];
            }
        }
        t = clock() - t;

        printf("When N = %d, Compute b=Ax: %d ms\n", n+1, t);
        free(x);
        free(b);
        for (i = 0; i < n; i++)
        {
            free(A[i]);
        }
    }
    return 0;
}
