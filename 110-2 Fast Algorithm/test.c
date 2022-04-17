#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

int main()
{
    int a = 6 ;
    char *p ;
    double complex z ;
    z = cexp(-2 * M_PI * I / 9.0) ;
    printf("%f + %f i", creal(z) , cimag(z)) ;
    return 0;
}
