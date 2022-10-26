#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int gcd(int, int);
int mod2Gcd(int, int);
int binaryGcd(int, int);
int main(int argc, char *argv[])
{
    unsigned int m, n, a, b, d, u, v, q, r, i, j, k, l, x, y, z, N, T, xy, ans, ansb;
    N = 1 << 23;

    time_t t0, t1, t2, t3, t4, t5, t6;
    srand(time(NULL));

    m = (rand() / 2) * (rand() / 2) *2;
    n = (rand() / 2) * (rand() / 2) *2;

    if (m < n)
    {
        int T;
        T = n;
        n = m;
        m = T;
    }
    // Euclidean Algorithm
    t0 = clock();
    for (i = 0; i < N; i++)
    {
        a = m;
        b = n;
        while (b != 0)
        {
            r = a % b;
            a = b;
            b = r;
        }
    }
    t0 = clock() - t0;

    // basic method call by func.
    t1 = clock();
    for (j = 0; j < N; j++)
    {
        a = m;
        b = n;
        ans = gcd(a, b);
    }
    t1 = clock() - t1;

    // mod
    t2 = clock();
    for (k = 0; k < N; k++)
    {
        x = m;
        y = n;
        ansb = 1;
        while (y != 0)
        {
            while ((x % 2 == 0) && (y % 2 == 0))
            {
                x /= 2;
                y /= 2;
                ansb *= 2;
            }
            while ((x % 2 == 0) && (y % 2 == 1))
            {
                x /= 2;
            }
            while ((x % 2 == 1) && (y % 2 == 0))
            {
                y /= 2;
            }
            z = x % y;
            x = y;
            y = z;
        }
        ansb *= x;
    }
    t2 = clock() - t2;
    // mod2 call func
    t3 = clock();
    for (k = 0; k < N; k++)
    {
        x = m;
        y = n;
        ansb = 1;
        mod2Gcd(x, y);
    }
    t3 = clock() - t3;

    // mod3
    t4 = clock();
    for (k = 0; k < N; k++)
    {
        x = m;
        y = n;
        ansb = 1;
        while (y != 0)
        {
            while ((x % 3 == 0) && (y % 3 == 0))
            {
                x /= 3;
                y /= 3;
                ansb *= 3;
            }
            while ((x % 3 == 0) && (y % 3 != 0))
            {
                x /= 3;
            }
            while ((x % 3 != 0) && (y % 3 == 0))
            {
                y /= 3;
            }
            z = x % y;
            x = y;
            y = z;
        }
        ansb *= x;
    }
    t4 = clock() - t4;
    // mod5
    t5 = clock();
    for (k = 0; k < N; k++)
    {
        x = m;
        y = n;
        ansb = 1;
        while (y != 0)
        {
            while ((x % 5 == 0) && (y % 5 == 0))
            {
                x /= 5;
                y /= 5;
                ansb *= 5;
            }
            while ((x % 5 == 0) && (y % 5 != 0))
            {
                x /= 5;
            }
            while ((x % 5 != 0) && (y % 5 == 0))
            {
                y /= 5;
            }
            z = x % y;
            x = y;
            y = z;
        }
        ansb *= x;
    }
    t5 = clock() - t5;
    // Binary
    t6 = clock();
    for (l = 0; l < N; l++)
    {
        u = m;
        v = n;

        int binaryGcd(int u, int v);
    }
    t6 = clock() - t6;

    printf(" gcd(%d,%d)=%d\n", m, n, ans);
    printf("(Euclidean Algorithm)Gcd takes:%d ms \n            call by func.takes:%d ms\n\n", t0, t1);
    printf("(mod 2)              Gcd takes:%d ms\n            call by func.takes:%d ms\n\n", t2, t3);
    printf("(mod 3)              Gcd takes:%d ms\n", t4);
    printf("(mod 5)              Gcd takes:%d ms\n\n", t5);
    printf("(Binary EuclideanAlgorithm)takes:%d ms\n gcd(%d,%d)=%d\n\n", t6, m, n, binaryGcd(u, v));
    
    system("pause");
    return 0;
}

int gcd(int a, int b)
{
    int r = a % b;

    if (r == 0)
        return b;
    else
        return gcd(b, r);
}

int mod2Gcd(int u, int v)
{

    if (u > v)
    {
        int T;
        T = v;
        v = u;
        u = T;
    }
    int ansb = 1;
    int r;
    while (v != 0)
    {
        while (!(u & 1) && !(v & 1))
        {
            u >> 1;
            v >> 1;
            ansb << 1;
        }
        while ((u & 1 == 0) && (v & 0x1 == 1))
        {
            u >> 1;
        }
        while ((u & 1 == 1) && (v & 0x1 == 0))
        {
            v >> 1;
        }
        r = u % v;
        u = v;
        v = r;
    }
    ansb = ansb * u;
    return ansb;
}

/*  Stein's algorithm or  binary Euclidean algorithm
    gcd(0, v) = v
    gcd(2u, 2v) = 2·gcd(u, v)
    gcd(2u, v) = gcd(u, v), if v is odd (2 is not a common divisor)
!!!    gcd(u, v) = gcd(|u − v|, min(u, v)), if u and v are both odd !!!
*/
/* truth table
        p   q   p&q   p|q
        0   0    0     0
        0   1    0     1
        1   0    0     1
        1   1    1     1
*/
int binaryGcd(int u, int v)
{
    if (u == v)
    {
        return (u);
    }
    if (!u && !v)
    { //(0,0)=0
        return (0);
    }
    if (!u || !v)
    { //(0,v)=v (u,o)=u
        return (u | v);
    }
    // use & 1 check the last number of binary
    if (!(u & 1) && !(v & 1))
    {                                            // u and v are both even
        return (binaryGcd(u >> 1, v >> 1) << 1); // then gcd(u, v) = 2 * gcd(u/2, v/2)
    }
    if (!(u & 1) && (v & 1))
    {                                // u  even v odd
        return binaryGcd(u >> 1, v); // gcd(u, v) = gcd(u/2, v)
    }
    if ((u & 1) && !(v & 1))
    { //  u  odd v even
        return binaryGcd(u, v >> 1);
    }
    if ((u & 1) && (v & 1))
    { // u and v are odd
        if (u <= v)
        {
            return binaryGcd((v - u) >> 1, u);
        }
        else
        {
            return binaryGcd((u - v) >> 1, v);
        }
    }
}