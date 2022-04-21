#include <stdio.h>   // for printf function
#include <stdlib.h>  // for memory allocation
#include <time.h>    // for time calculation
#include <math.h>    // for sine and cosine functions
#include <complex.h> // for complex number

int FFT235(double complex *X, int N);
int BitReverse(int k, int p, int n) ;

int main(int argc, char *argv[])
{
    // Some Loop Counters and Temporary Variables
    int n, temp1, temp2;
    time_t t;

    // Get the size of data
    int N = 0;
    char ch;
    FILE *Input;

    Input = fopen("hw6_test.csv", "r");
    while (!feof(Input))
    {
        ch = fgetc(Input);
        if (ch == '\n')
        {
            N++;
        }
    }
    fclose(Input);
    printf("Lines: %d\n", N);

    // Setting LogP_N and N
    // int LogP_N = -1;
    // int N;
    // int p = 2;
    // for (N = 1; N < Line; N *= p)
    // {
    //     LogP_N++;
    // }
    // N /= p;
    // printf("N: %d\n", N);

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
    FFT235(X, N);
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





int FFT235(double complex *X, int N)
{
    // Loop Counters and Temporary Calculating Variables
    int temp, temp1, temp2, k_temp, k_change, k, i, j, n, N_temp, m, p; 
    double TempDouble, TempDouble1, TempDouble2;

    int IsChange[N];        // for change index
    double complex W_N[N];  // for butterfly, W_N = exp(-2\pi i / N), W_N_k = (W_N)^k
    double complex Y[5];    // Y = FTp * X, p <= 5
    
    // N = \prod_i Prime[i] ^ Digit[i]
    // Np[i] = Prime[i] ^ Digit[i]
    int Prime[3] = {2, 3, 5} ;
    int Digit[3] ;
    int Np[3] ;
    int MaxLayer ;

    // Setting Digit[i]
    N_temp = N ;
    MaxLayer = 0;
    for(i = 0 ; i < 3 ; i++)
    {
        Digit[i] = 0 ;
        Np[i] = 1 ;
        while (!(N_temp % Prime[i]))
        {
            Digit[i]++ ;
            Np[i] *= Prime[i] ;
            N_temp /= Prime[i] ;
        }
        MaxLayer += Digit[i] ;
    }
    if(Np[0] * Np[1] * Np[2] != N)
    {
        printf("error: N is not of the form 2^p * 3^q * 5^r") ;
    }

    
    // Setting W_N
    W_N[0] = 1;
    for (i = 1; i < N; i++)
    {
        W_N[i] = cexp(-2 * M_PI * I * i / N);
    }

    // Setting W_p
    // for (i = 0; i < p; i++)
    // {
    //     W_p[i] = W_N[i * N / p];
    // }

    // Do FFT
    if (N == 1)             // Special Case: N = 1
    {
        return 1;
    }
    else if (N <= 5)        // Special Case: N <= 5
    {
        // Directly Compute
        for (k = 0; k < N; k++)
        {
            Y[k] = 0;
            for (n = 0; n < N; n++)
            {
                if (k == 0 || n == 0)
                {
                    Y[k] += X[n];
                }
                else
                {
                    Y[k] += X[n] * W_N[(k * n) % N];
                }
            }
        }

        for (k = 0 ; k < N ; k++)
        {
            X[k] = Y[k] ;
        }
        return 1;
    }
    else
    {
        // Change Index (x[k] <-> x[k_change])
        for(n = 0; n < N; n++)
        {
            IsChange[n] = 0;
        }
        temp = -1;
        n = 0;
        while(n < N)
        {
            if(n == 0 || n == N-1)      // The First and last index do not change
            {
                IsChange[n] = 1;
                n++;
                continue;
            }
            else if(IsChange[n])            // Check if the index has been changed
            {
                n++;
                continue;
            }

            if(temp == -1)
            {
                temp = n;
                Y[0] = X[n] ;
            }

            // Let X[n] = X[temp2] when X[temp2] has not changed,
            //     X[n] = Y[0]     when X[temp2] has changed
            temp1 = n;
            k = temp1 % Np[2];
            temp1 /= Np[2];
            j = temp1 % Np[1];
            i = temp1 / Np[1];
            temp2 = BitReverse(k, Prime[2], Digit[2]) * Np[1] * Np[0]
                  + BitReverse(j, Prime[1], Digit[1]) * Np[0]
                  + BitReverse(i, Prime[0], Digit[0]) ;
            
            IsChange[n] = 1;    
            if(temp2 != temp)
            {
                X[n] = X[temp2];
                n = temp2;
            }
            else
            {
                X[n] = Y[0];
                temp = -1;
                n = temp2;
            }
        }

        // for(i = 0 ; i < Np[0] ; i++)
        // {
        //     for(j = 0 ; j < Np[1] ; j++)
        //     {
        //         for(k = 0 ; k < Np[2] ; k++)
        //         {
        //             temp1 = i * Np[1] * Np[2] + j * Np[2] + k ;
        //             temp2 = BitReverse(k, Prime[2], Digit[2]) * Np[1] * Np[0]
        //                 + BitReverse(j, Prime[1], Digit[1]) * Np[0]
        //                 + BitReverse(i, Prime[0], Digit[0]) ;
        //             if(temp1 < temp2)
        //             {
        //                 X[temp1] += X[temp2];
        //                 X[temp2] = X[temp1] - X[temp2] ;
        //                 X[temp1] = X[temp1] - X[temp2] ;
        //             }
        //         }
        //     }
        // }

        // Display Data
        //   for (k = 0 ; k < N ; k++)
        //   {
        //       printf("%d,%f\n",k,creal(X[k])) ;
        //   }

        // Do FFT3
        for (i = 0, temp = 1; i < MaxLayer; i++, temp = N_temp)
        {
            if(i < Digit[2])
            {
                p = Prime[2] ;
            }
            else if(i < Digit[2] + Digit[1])
            {
                p = Prime[1] ;
            }
            else
            {
                p = Prime[0] ;
            }
            N_temp = temp * p;

            for (k = 0; k < temp; k++)
            {
                for (j = 0; j < N; j += N_temp)
                {
                    // Multiple W_N^nk
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
                                    Y[m] += X[j + k + n * temp] * W_N[N / p * ((m * n) % p)];
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

int BitReverse(int k, int p, int n)
{
    // Check if n is bigger enough
    if(k >=pow(p,n))
    {
        printf("error: n is too small!\n") ;
        return -1; 
    }

    int j;  // j is the bit-reverse number of k.
    
    int i;  // Loop Counting variable
    
    for (i = 1, j = k % p ; i < n ; i++)
    {
        j *= p ;
        k /= p ;
        j += k % p ;
    }

    return j ;

}
