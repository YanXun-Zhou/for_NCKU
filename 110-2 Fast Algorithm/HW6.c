#include <stdio.h>  // for printf function
#include <stdlib.h> // for memory allocation
#include <time.h>   // for time calculation
#include <math.h>   // for sine and cosine functions

int FFT(double *xr, double *xi, double *yr, double *yi, int N);

int main(int argc, char *argv[])
{
    // Declare all the variables
    int k, n, N;
    double *xr, *xi, *yr, *yi, *zr, *zi, t, a, b, an, bn, c, s;
    time_t T;

    n = 0 ;
    // File Input
    FILE *fp;
    char ch;
    int L;
    fp = fopen("hw6.csv", "r"); // open csv file, feof: end of file
    L = 0;

    while (!feof(fp))
    {
        ch = fgetc(fp); // fgetc: get a char from file
        if (ch == '\n')
        { // check \n L++
            L++;
        }
    }
    fclose(fp); // free
    printf("Lines: %d\n", L);
    system("pause");

    // Input the number N
    N = 1;
    while (N < L)
    {
        N = N * 2;
    }
    N = N / 2;
    printf("%d,%d\n", L, N);
    // scanf ("%d",&N);

    // Locate the memory for x, yr, yi;
    xr = (double *)malloc(N * sizeof(double));
    xi = (double *)malloc(N * sizeof(double));
    yr = (double *)malloc(N * sizeof(double));
    yi = (double *)malloc(N * sizeof(double));
    zr = (double *)malloc(N * sizeof(double));
    zi = (double *)malloc(N * sizeof(double));

    fp = fopen("hw6.csv", "r"); // file ok
    // initial x
    for (k = 0; k < N; ++k)
    {
        fscanf(fp, "%d,%d\n", &n, &L);
        xr[k] = 1.0 * L;
        xi[k] = 0.0;
    }
    fclose(fp);
    /*
    for(k=0; k < 5;++k)
    {
        printf("%d: %f\n", k, xr[k]);  //print for sure
    }
    */

    T = clock();
    a = cos(2 * M_PI / N);
    b = sin(2 * M_PI / N);
    an = 1;
    bn = 0;
    for (n = 0; n < N; n++)
    {
        yr[n] = 0.0;
        yi[n] = 0.0;
        c = 1;
        s = 0;
        for (k = 0; k < N; ++k)
        {
            yr[n] += c * xr[k];
            yi[n] -= s * xr[k];
            t = c * an - s * bn;
            s = c * an + s * bn;
        }
        t = an * a - bn * b;
        bn = an * b + bn * a;
        an = t;
    }

    T = clock() - T;
    printf("%d ms for discrete FT of %d elements\n", T, N);

    T = clock();
    FFT(xr, xi, zr, zi, N);
    T = clock() - T;
    printf("%d ms for FFT of %d elements\n", T, N);

    t = 0;
    for (k = 0; k < N; ++k)
    {
        t = t + fabs(yr[k] - zr[k]) + fabs(yi[k] - zi[k]);
    }
    printf("ERROR: %e\n", t);

    free(xr);
    free(xi);
    free(yr);
    free(yi);

    return 100;
}
int FFT(double *xr, double *xi, double *yr, double *yi, int N)
{
    if (N == 2)
    {
        yr[0] = xr[0] + xr[1];
        yi[0] = xi[0] + xi[1];
        yr[1] = xr[0] - xr[1];
        yi[1] = xi[0] - xi[1];
    }
    else
    {
        int k;
        double *yEr, *yEi, *yOr, *yOi; // Even Odd
        double *xEr, *xEi, *xOr, *xOi;
        double u, v, R, I, NR, NI, R_temp;
        // open memory for above variable
        yEr = (double *)malloc(N / 2 * sizeof(double));
        yEi = (double *)malloc(N / 2 * sizeof(double));
        xEr = (double *)malloc(N / 2 * sizeof(double));
        xEi = (double *)malloc(N / 2 * sizeof(double));
        yOr = (double *)malloc(N / 2 * sizeof(double));
        yOi = (double *)malloc(N / 2 * sizeof(double));
        xOr = (double *)malloc(N / 2 * sizeof(double));
        xOi = (double *)malloc(N / 2 * sizeof(double));

        for (k = 0; k < N / 2; ++k)
        {
            xEr[k] = xr[2 * k];
            xEi[k] = xi[2 * k];
            xOr[k] = xr[2 * k + 1];
            xOi[k] = xi[2 * k + 1];
        }
        FFT(yEr, yEi, xEr, xEi, N / 2);
        FFT(yOr, yOi, xOr, xOi, N / 2);
        NR = cos(2 * M_PI / N);
        NI = sin(2 * M_PI / N);
        R = 1.0;
        I = 0.0;
        for (k = 0; k < N / 2; ++k)
        {
            u = R * yOr[k] - I * yOi[k];
            v = R * yOi[k] + I * yOr[k];
            yr[k] = yEr[k] + u;
            yi[k] = yEi[k] + v;
            yr[N / 2 + k] = yEr[k] - u;
            yi[N / 2 + k] = yEi[k] - v;
            R_temp = R;
            R = R * NR - I * NI;
            I = R_temp * NI + I * NR;
        }
        free(xEr);
        free(xEi);
        free(yEr);
        free(yEi);
        free(xOr);
        free(xOi);
        free(yOr);
        free(yOi);
    }

    return 0;
}
