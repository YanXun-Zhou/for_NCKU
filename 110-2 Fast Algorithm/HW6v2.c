#include <stdio.h>   // for printf function
#include <stdlib.h>  // for memory allocation
#include <time.h>    // for time calculation
#include <math.h>    // for sine and cosine functions
#include <complex.h> // for complex number

int FFT2(double complex *X, int Log2_N);

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

    // Setting Log2_N and N 
    int Log2_N = (int)floor(log2(Line));
    int N = 1 << Log2_N;
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
    FFT2(X,Log2_N) ;
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

int FFT2(double complex *X, int Log2_N)
{
    int temp, temp1 , temp2, k, i, j; // Loop Counters and Temporary Calculating Variables
    int N = 1 << Log2_N;
    double complex Y[2];       // for butterfly
    double complex W_N, W_N_j; // for butterfly, W_N_j = (W_N)^j

    if (Log2_N == 0)
    {
        return 1;
    }
    else if (Log2_N == 1)
    {
        // Directly Compute
        X[0] = X[0] + X[1];
        X[1] = X[0] - 2 * X[1];
        return 1;
    }
    else
    {
        // Change Index
        for (k = 0; k < N; k++)
        {
            temp = 0;
            for (i = 1; i <= Log2_N; i++)
            {
                temp += ( (k & (1 << (i - 1))) >> (i - 1) ) * ( 1 << (Log2_N - i) );
                // temp1 = (k & (1 << (i - 1))) >> (i - 1);
                // temp2 = 1 << (Log2_N - i);
                // temp += temp1 * temp2;
            }

            // X[k] <-> X[temp]
            if (k < temp)
            {
                X[k] += X[temp];
                X[temp] = X[k] - X[temp];
                X[k] = X[k] - X[temp];
            }

        }

        // Display Data
        // for (k = 0 ; k < N ; k++)
        // {
        //     printf("%d,%f\n",k,creal(X[k])) ;
        // }

        // Do FFT2
        for (i = 0, temp = 1; i < Log2_N; i++, temp <<= 1)
        {
            //temp = 1 << i;
            W_N = cexp(-2 * M_PI * I / (temp << 1));
            
            for (j = 0, W_N_j = 1; j < temp; j++, W_N_j *= W_N)
            {
                // Compute W_N_j
                // if (j == 1)
                // {
                //     W_N_j = W_N;
                // }
                // else if (j > 1)
                // {
                //     W_N_j *= W_N;
                // }
                
                for (k = 0; k < N; k += (temp << 1))
                {
                    // Multiple W_N_j
                    if (i > 0 && j > 0)
                    {
                        X[k + j + temp] *= W_N_j;
                    }
                    // Do Butterfly with x[k+j] and x[k+j+(1<<i)]
                    Y[0]            = X[k + j] + X[k + j + temp];
                    Y[1]            = X[k + j] - X[k + j + temp];
                    X[k + j]        = Y[0];
                    X[k + j + temp] = Y[1];
                }
            }
        }
    }

    return 1 ;
}