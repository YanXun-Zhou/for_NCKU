#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    unsigned int i, N = 1 << 28 ;
    double *a, *b ;
    time_t t1 ;

    srand(time(NULL)) ;
    a = (double *) malloc((N+1)*sizeof(double)) ;       // Compute FLOPS
    b = (double *) malloc((N)*sizeof(double)) ;         // Save the results
    for (i=0;i<N+1;i++){
        a[i] = 1.0*rand()/RAND_MAX ;
    }
    for (i=0;i<N;i++){
        b[i] = 0 ;
    }

    // Start computing multiplication
    t1 = clock() ;
    for(i=0;i<N;i++){
        b[i] = a[i] * a[i+1] ;
    }
    t1 = clock() - t1 ;
    
    // Output the time and some results
    for(i=N-10;i<N;i++){
        printf("no.%d : %f * %f = %f\n",i,a[i],a[i+1],b[i]) ;   // output the last 10 results
    }
    printf("Computing %d times multiplication uses %d ms\n",N,t1) ;
    printf("The average time is %e ms",(double)t1 / (double)N) ;

    free(a) ;
    free(b) ;

    return 0;
}
