#include <stdio.h>  // for printf function
#include <stdlib.h> // for memory allocation
#include <time.h>   // for time calculation
#include <math.h>   // for sine and cosine functions

int quick_sort(double *x, int L, int R);    
double find_value(double *x, int L, int R, int target_index);
double find_Q2(double *x, int L, int R);

// quick_sort(x,L,R) can sort x[R] and return the new index of x[R].
// find_value(x,L,R,target_index) will use quick_sort to sort and find x[target_index].
// find_Q2(x,L,R) will use find_value to find the median value of x[L] ~ x[R].

int main()
{
    // Declare all the variables
    int k, N;
    double *x, *y;
    time_t t, t1, t2;

    // Input the number N
    printf("Input N: ");
    scanf("%d", &N);

    // Locate the memory for x and y;
    x = (double *)malloc(N * sizeof(double));

    // Initial setting for x, for example, x[k] = 1.0*rand()/RAND_MAX
    // srand(time(NULL));
    srand(3);
    for (k = 0; k < N; ++k)
    {
        x[k] = 1.0 * rand() / RAND_MAX;
    }

    // print x
    if (N <= 20)
    {
        printf("x = \n");
        for (k = 0; k < N; ++k)
        {
            printf("%f\n", x[k]);
        }
    }

    // find Q2-value of x
    double Q2;

    t = clock();
    int L = 0, R = N - 1;
    Q2 = find_Q2(x, L, R);
    t1 = clock() - t;

    // find Q3 - Q1 of x
    double Q1, Q3;
    int temp_index = N / 2;
    if (N % 2)
    {
        L = 0;
        R = temp_index - 1;
        Q1 = find_Q2(x, L, R);
        L = temp_index + 1;
        R = N - 1;
        Q3 = find_Q2(x, L, R);
    }
    else
    {
        L = 0;
        R = temp_index - 1;
        Q1 = find_Q2(x, L, R);
        L = temp_index;
        R = N - 1;
        Q3 = find_Q2(x, L, R);
    }
    t2 = clock() - t;

    printf("The Q2-value of x is %f\n", Q2);
    printf("Q3 - Q1 = %f\n", Q3 - Q1);
    printf("Finding Q2 value: %f s\n", 1.0 * t1 / CLOCKS_PER_SEC);
    printf("Continue to find Q3-Q1: %f s\n", 1.0 * t2 / CLOCKS_PER_SEC);

    // free the memory located by x, y
    free(x);
    return 0;
}

int quick_sort(double *x, int L, int R)
{
    if (L == R)
    {
        return L;
    }

    int i, n;
    double temp;
    n = L;
    for (i = L; i < R; i++)
    {
        if (x[i] < x[R])
        {
            // swap(x[i],x[n]) and n++
            temp = x[i];
            x[i] = x[n];
            x[n] = temp;
            n++;
        }
    }
    temp = x[R];
    x[R] = x[n];
    x[n] = temp;
    return n;
}

double find_value(double *x, int L, int R, int target_index)
{
    int now_index = R;
    do
    {
        now_index = quick_sort(x, L, R);

        if (now_index < target_index)
        {
            L = now_index + 1;
        }
        else if (now_index > target_index)
        {
            R = now_index - 1;
        }
    } while (now_index != target_index);

    return x[target_index];
}

double find_Q2(double *x, int L, int R)
{
    int Q2L_index = (R - L) / 2 + L;
    int Q2R_index = (R - L + 1) / 2 + L;
    double Q2L, Q2R;
    Q2L = find_value(x, L, R, Q2L_index);
    if (Q2L_index == Q2R_index)
        return Q2L;
    else
    {
        Q2R = find_value(x, Q2L_index, R, Q2R_index);
        return (Q2L + Q2R) / 2;
    }
}

