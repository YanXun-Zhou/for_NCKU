#include <stdio.h>
#include <stdlib.h>

int main()
{
    /* local variable definition */
    unsigned int m, n, a, b, r, d;
    int i = 1;
    while (i == 1)
    {
        m = 75349409;
        n = 84242782;
        a = m;
        b = n;
        d = 1;
        int Mode;
        while (a % 2 == 0 || b % 2 == 0)
        {
            Mode = 2 * (a % 2) + (b % 2);
            switch (Mode)
            {
            case 0:
                a = a >> 1;
                b = b >> 1;
                d = d << 1;
                break;
            case 1:
                a = a >> 1;
                break;
            case 2:
                b = b >> 1;
                break;
            default:
                break;
            }
        }

        while (a != 0 && b != 0)
        {
            r = a % b;
            a = b;
            b = r;
        }
        if (a == 0)
        {
            d *= b;
        }
        else
        {
            d *= a;
        }

        printf("%d,%d,%d", m, n, d);
        i = 0;
    }
    return 0;
}
