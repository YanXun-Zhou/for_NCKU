#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

int BitReverse(int k, int p, int n) ;

int main()
{
    int k = 1000 ;
    int p = 10;
    int n = 5 ;
    int j ;
    j = BitReverse(k,p,n) ;
    printf("%d <-> %d\n",k,j) ;

    int test[10] = {0} ;
    printf("%d\n",test[3]) ;
    return 0;
}

int BitReverse(int k, int p, int n)
{
    // Check if n is bigger enough
    if(k >= pow(p,n))
    {
        printf("error: n is too small!\n") ;
        return -1; 
    }

    int j;  // j is the bit-reverse number of k.
    
    int i;  // Counting variable
    
    for (i = 1, j = k % p ; i < n ; i++)
    {
        j *= p ;
        k /= p ;
        j += k % p ;
    }

    return j ;

}