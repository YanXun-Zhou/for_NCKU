#include <stdio.h>      // for print function
#include <stdlib.h>     // for memory allocation
#include <math.h>       // for power function

int BitReverse(int k, int p, int n) ;

int main(int argc, char *argv[])
{
    int k, p, n, j;
    printf("Given an integer k: ");
    scanf("%d", &k) ;
    printf("The base p: ");
    scanf("%d", &p) ;
    printf("The number of Digits n: ");
    scanf("%d", &n) ;

    j = BitReverse(k,p,n) ;
    return 0 ;
}

int BitReverse(int k, int p, int n)
{
    // Check if n is bigger enough
    if(k >=pow(p,n))
    {
        printf("n is too small!") ;
        return -1; 
    }

    // Some Temporary Variables and Counting Variables
    int i, temp ;

    int Digit[n];   // Record the digits of k and j in base p.
    int j;          // Integer j is the base-p reverse of k.

    // Get the digits of k in base p.
    j = k;
    for(i = 0 ; i < n ; i++)
    {
        Digit[i] = j % p ;
        j /= p ;
    }

    // Show k = (a_0 a_1 ... a_n)_p
    printf("k = %d = ( ",k) ;
    for(i = 0 ; i < n ; i++)
    {
        printf("%d ",Digit[n-1-i]) ;
    }
    printf(")_%d\n",p) ;

    // Do Bit Reverse
    for(i = 1, j = Digit[0] ; i < n ; i++)
    {
        j *= p;
        j += Digit[i] ;
    }

    // Show j = (a_n ... a_1 a_0)_p
    printf("j = %d = ( ",j) ;
    for(i=0 ; i < n ; i++)
    {
        printf("%d ",Digit[i]) ;
    }
    printf(")_%d\n",p) ;

    return j ;
}
