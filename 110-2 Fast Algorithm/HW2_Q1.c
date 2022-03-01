#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    unsigned int i, N = 1 << 28 ;
    double *a, *b ;
    time_t t1 ;

    srand(time(NULL)) ;
    a = (double *) malloc((N+1)*sizeof(double)) ;       // 計算乘法
    b = (double *) malloc((N)*sizeof(double)) ;         // 紀錄結果
    for (i=0;i<N+1;i++){
        a[i] = 1.0*rand()/RAND_MAX ;
    }
    for (i=0;i<N;i++){
        b[i] = 0 ;
    }

    // 開始計算乘法
    t1 = clock() ;
    for(i=0;i<N;i++){
        b[i] = a[i] * a[i+1] ;
    }
    t1 = clock() - t1 ;
    
    // 輸出計算時間與結果
    for(i=N-10;i<N;i++){
        printf("no.%d : %f * %f = %f\n",i,a[i],a[i+1],b[i]) ;
    }
    printf("計算 %d 次浮點數乘法總耗時 %d ms\n",N,t1) ;
    printf("平均計算一次浮點數乘法所需時間為 %f ms",(double)t1 / (double)N) ;

    return 0;
}
