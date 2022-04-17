#include <stdio.h>   // for printf function
#include <stdlib.h>  // for memory allocation
#include <time.h>    // for time calculation
#include <math.h>    // for sine and cosine functions
#include <complex.h> // for complex number

int FFT3(double complex *X, int Log3_N, int N);

int main(int argc, char *argv[])
{
    // Some Loop Counters and Temporary Variables
    int n, temp1, temp2 ;
    time_t t ;

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

    // Setting Log3_N and N 
    int Log3_N = -1 ;
    int N ;
    for (N = 1 ; N < Line ; N*=3)
    {
        Log3_N ++ ;
    }
    N /= 3 ;
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
    t = clock() ;
    FFT3(X, Log3_N, N) ;
    t = clock() - t ;

    // Display Result
    printf("Result:\n") ;
    for (n = 0 ; n < N ; n++)
    {
        printf("%d, %f + %f i\n", n , creal(X[n]) , cimag(X[n])) ;
    }
    printf("Time: %d ms\n", t) ;
    printf("2, %f + %f i\n",creal(X[2]),cimag(X[2]));


    free(X);
    return 0;
}

int FFT3(double complex *X, int Log3_N, int N)
{
    int temp, temp1 , temp2, k_temp, k_change, k, i, j; // Loop Counters and Temporary Calculating Variables
    double TempDouble, TempDouble1, TempDouble2 ;
    double complex Y[3];       // for butterfly
    double complex W_N, W_N_k; // for butterfly, W_N = exp(-2\pi i / N), W_N_k = (W_N)^k
    double complex W_31, W_32 ;// W_31 = (W_3)^1 , W_32 = (W_3)^2
    W_31 = cexp(-2 * M_PI * I / 3) ;
    W_32 = W_31 * W_31 ;

    if (Log3_N == 0)
    {
        return 1;
    }
    else if (Log3_N == 1)
    {
        // Directly Compute
        Y[0] = X[0]     + X[1]          + X[2];
        Y[1] = X[0]     + X[1] * W_31   + X[2] * W_32;
        Y[2] = X[0]     + X[1] * W_32   + X[2] * W_31;
        for(i=0 ; i<3 ; i++)
        {
            X[i] = Y[i] ;
        }
        return 1;
    }
    else
    {
        // Change Index (x[k] <-> x[k_change])
        // Let k = (a_N-1 a_N-2 ... a_1 a_0)_3, k_change - k = \sqrt{3^(Log3_N-1)} \sum a_k \sinh(((Log3_N-1)/2 - k) * log(3))
        for (k = 0; k < N; k++)
        {
            k_temp = k;
            k_change = 0 ;
            temp2 = N / 3 ;
            for (i = 0 ; i < Log3_N ; i++)
            {
                temp1 = k_temp % 3 ;
                k_temp /= 3 ;
                k_change += temp1 * temp2 ;
                temp2 /= 3 ;
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
        // for (k = 0 ; k < N ; k++)
        // {
        //     printf("%d,%f\n",k,creal(X[k])) ;
        // }

        // Do FFT3
        for (i = 0, temp = 1; i < Log3_N; i++, temp *= 3)
        {
            
            W_N = cexp(-2 * M_PI * I / (temp * 3));
            
            for (k = 0, W_N_k = 1; k < temp; k++, W_N_k *= W_N)
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
                
                for (j = 0; j < N; j += (temp * 3))
                {
                    // Multiple W_N_k
                    if (i > 0 && k > 0)
                    {
                        X[k + j + temp] *= W_N_k;
                        X[k + j + temp + temp] *= (W_N_k * W_N_k);
                    }
                    // Do Butterfly with x[k+j] and x[k+j+(1<<i)]
                    Y[0]    = X[k + j]  + X[k + j + temp]           + X[k + j + temp + temp];
                    Y[1]    = X[k + j]  + X[k + j + temp] * W_31    + X[k + j + temp + temp] * W_32;
                    Y[2]    = X[k + j]  + X[k + j + temp] * W_32    + X[k + j + temp + temp] * W_31;
                    X[k + j]                = Y[0];
                    X[k + j + temp]         = Y[1];
                    X[k + j + temp + temp]  = Y[2];
                }
            }
        }
    }

    return 1 ;
}