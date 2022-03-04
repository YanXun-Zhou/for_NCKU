#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1 << 25

unsigned int gcd(unsigned int a, unsigned int b);
int main(int argc, char *argv[])
{
    unsigned int i, r, m, n, a, b, d; // a = bq + r, a >= b, d = gcd(a,b)
    time_t t;

    // m = rand() / 2 * rand() / 2 + 1;
    // n = rand() / 2 * rand() / 2 + 1;

    // method 1 : call function
    srand(1);
    t = clock();
    for (i = 0; i < N; i++)
    {
        a = rand() / 2 * rand() / 2 + 1;
        b = rand() / 2 * rand() / 2 + 1;
        d = gcd(a, b);
    }
    t = clock() - t;

    printf("method 1 : gcd(%d,%d)=%d\n", a, b, d); // 輸出最後一次計算結果提供確認結果正不正確
    printf("use time:%d ms\n\n", t);               // 計算消耗時間

    // method 1 end

    // method 2 : loop
    srand(1);
    t = clock();
    for (i = 0; i < N; i++)
    {
        m = rand() / 2 * rand() / 2 + 1;
        n = rand() / 2 * rand() / 2 + 1;
        a = m;
        b = n;
        while (a != 0 && b != 0)
        {
            r = a % b;
            a = b;
            b = r;
        }
        if (a == 0)
        {
            d = b;
        }
        else
        {
            d = a;
        }
    }
    t = clock() - t;
    printf("method 2 : gcd(%d,%d)=%d\n", m, n, d); // 輸出最後一次計算結果提供確認結果正不正確
    printf("use time:%d ms\n\n", t);               // 計算消耗時間
    // method 2 end

    // method 3
    int Mode = -1; // Mode (== 0) : a,b are even; (== 1) : a is even, b is odd; (== 2) : a is odd, b is even;
    srand(1);
    t = clock();
    for (i = 0; i < N; i++)
    {
        m = rand() / 2 * rand() / 2 + 1;
        n = rand() / 2 * rand() / 2 + 1;
        a = m;
        b = n;
        d = 1;

        // 提出2的倍數
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
    }
    t = clock() - t;
    printf("method 3 : gcd(%d,%d)=%d\n", m, n, d); // 輸出最後一次計算結果提供確認結果正不正確
    printf("use time:%d ms\n\n", t);               // 計算消耗時間
    // method 3 end

    return 0;
}

unsigned int gcd(unsigned int a, unsigned int b)
{
    unsigned int r;
    r = a % b;

    if (r == 0)
        return b;
    else
        return gcd(b, r);
}