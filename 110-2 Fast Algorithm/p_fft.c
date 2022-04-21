#include <stdio.h>   // for printf function
#include <stdlib.h>  // for memory allocation
#include <time.h>    // for time calculation
#include <math.h>    // for sine and cosine functions
#include <complex.h> // for complex number

int FFTp(double complex *X, int LogP_N, int N, int p);

int main(int argc, char *argv[])
{
    // Some Loop Counters and Temporary Variables
    int n, temp1, temp2;
    time_t t;

    // Get the size of data
    int Line = 0;
    char ch;
    FILE *Input;

    Input = fopen("hw6_test.csv", "r");
    while (!feof(Input))
    {
        ch = fgetc(Input);
        if (ch == '\n')
        {
            Line++;
        }
    }
    fclose(Input);
    printf("Lines: %d\n", Line);

    // Setting LogP_N and N
    int LogP_N = -1;
    int N;
    int p = 2;
    for (N = 1; N < Line; N *= p)
    {
        LogP_N++;
    }
    N /= p;
    printf("N: %d\n", N);

    // Load the data (X)
    double complex *X;
    X = (double complex *)malloc(N * sizeof(double complex));

    Input = fopen("hw6_test.csv", "r");
    for (n = 0; n < N; n++)
    {
        fscanf(Input, "%d,%d", &temp1, &temp2);
        X[n] = 1.0 * temp2 + 0 * I;
    }

    // Do FFT and Record the Time
    t = clock();
    FFTp(X, LogP_N, N, p);
    t = clock() - t;

    // Display Result
    printf("Result:\n");
    for (n = 0; n < N; n++)
    {
        printf("%d, %f + %f i\n", n, creal(X[n]), cimag(X[n]));
    }
    printf("Time: %d ms\n", t);
    // printf("2, %f + %f i\n", creal(X[2]), cimag(X[2]));

    free(X);
    return 0;
}

int FFTp(double complex *X, int LogP_N, int N, int p)
{
    int temp, temp1, temp2, k_temp, k_change, k, i, j, n, N_temp, m; // Loop Counters and Temporary Calculating Variables
    double TempDouble, TempDouble1, TempDouble2;
    double complex W_N[N]; // for butterfly, W_N = exp(-2\pi i / N), W_N_k = (W_N)^k
    double complex W_p[p];
    double complex Y[p]; // Y = Fp * X

    // Setting W_N
    W_N[0] = 1;
    for (i = 1; i < N; i++)
    {
        W_N[i] = cexp(-2 * M_PI * I * i / N);
    }

    // Setting W_p
    for (i = 0; i < p; i++)
    {
        W_p[i] = W_N[i * N / p];
    }

    if (LogP_N == 0) // N = 1
    {
        return 1;
    }
    else if (LogP_N == 1) // N = p
    {
        // Directly Compute
        for (k = 0; k < p; k++)
        {
            Y[k] = 0;
            for (n = 0; n < p; n++)
            {
                if (k == 0 || n == 0)
                {
                    Y[k] += X[n];
                }
                else
                {
                    Y[k] += X[n] * W_p[(k * n) % p];
                }
            }
        }
        return 1;
    }
    else
    {
        // Change Index (x[k] <-> x[k_change])
        // Let k = (a_N-1 a_N-2 ... a_1 a_0)_3, k_change - k = \sqrt{3^(LogP_N-1)} \sum a_k \sinh(((LogP_N-1)/2 - k) * log(3))
        for (k = 0; k < N; k++)
        {
            k_temp = k;
            k_change = 0;
            temp2 = N / p;
            for (i = 0; i < LogP_N; i++)
            {
                temp1 = k_temp % p;
                k_temp /= p;
                k_change += temp1 * temp2;
                temp2 /= p;
            }

            // X[k] <-> X[k_change]
            if (k < k_change)
            {
                X[k] += X[k_change];
                X[k_change] = X[k] - X[k_change];
                X[k] = X[k] - X[k_change];
            }
        }

        // Display Data
        //  for (k = 0 ; k < N ; k++)
        //  {
        //      printf("%d,%f\n",k,creal(X[k])) ;
        //  }

        // Do FFT3
        for (i = 0, temp = 1; i < LogP_N; i++, temp *= p)
        {
            N_temp = temp * p;

            for (k = 0; k < temp; k++)
            {
                // Compute W_N_k
                // if (k == 1)
                // {
                //     W_N_k = W_N;
                // }
                // else if (k > 1)
                // {
                //     W_N_k *= W_N;
                // }

                for (j = 0; j < N; j += (temp * p))
                {
                    // Multiple W_N_nk
                    if (i > 0 && k > 0)
                    {
                        for (n = 1; n < p; n++)
                        {
                            if (k != 0 && n != 0)
                            {
                                X[j + k + n * temp] *= W_N[N / N_temp * k * n];
                            }
                        }
                    }
                    // Do Butterfly with x[j+k+n*temp], n = 0, 1, ..., p-1
                    if (p == 2)         // Special case : p = 2
                    {
                        Y[0] = X[j + k] + X[j + k + temp] ;
                        Y[1] = X[j + k] - X[j + k + temp] ;
                        X[j + k]        = Y[0] ;
                        X[j + k + temp] = Y[1] ;
                    }
                    else if (p == 4)    // Special case : p = 4
                    {
                        Y[0] = X[j + k] + X[j + k + temp] + X[j + k + 2 * temp] + X[j + k + 3 * temp] ;
                        Y[1] = creal(X[j+k]) + cimag(X[j+k+temp]) - creal(X[j+k+2*temp]) - cimag(X[j+k+3*temp]) +
                                (cimag(X[j+k]) - creal(X[j+k+temp]) - cimag(X[j+k+2*temp]) + creal(X[j+k+3*temp])) * I ;
                        Y[2] = X[j + k] - X[j + k + temp] + X[j + k + 2 * temp] - X[j + k + 3 * temp] ;
                        Y[3] = creal(X[j+k]) - cimag(X[j+k+temp]) - creal(X[j+k+2*temp]) + cimag(X[j+k+3*temp]) +
                                (cimag(X[j+k]) + creal(X[j+k+temp]) - cimag(X[j+k+2*temp]) - creal(X[j+k+3*temp])) * I ;
                        X[j + k] = Y[0] ;
                        X[j + k + temp] = Y[1] ;
                        X[j + k + 2 * temp] = Y[2] ;
                        X[j + k + 3 * temp] = Y[3] ;
                        
                    }
                    else
                    {
                        for (m = 0; m < p; m++)
                        {
                            Y[m] = 0;
                            for (n = 0; n < p; n++)
                            {
                                if (n == 0 || m == 0)
                                {
                                    Y[m] += X[j + k + n * temp];
                                }
                                else
                                {
                                    Y[m] += X[j + k + n * temp] * W_p[(m * n) % p];
                                }
                            }
                        }
                        for (n = 0; n < p; n++)
                        {
                            X[j + k + n * temp] = Y[n];
                        }
                    }
                }
            }
        }
    }

    return 1;
}