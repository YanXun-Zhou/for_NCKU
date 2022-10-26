#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string.h>

typedef long long LL;
#define Inv(n) powmod( n, P - 2, P)

/*
 * P = C*2^k + 1, P is prime.
 * G is a primitive root. 
 * For the FFT of length N = 2^w, replace the n-th root of 1 by g = G^(phi(P) / N) (mod P), where phi(P) = P-1.
 */
int P;
int _g[ 25 ]; 
int BIT_CNT;

// ret = a^b (mod c)
int powmod( LL a, int b, int c){
    LL ret = 1 ;
    while(b){
         if(b & 0x1) ret = ret * a % c;
         a = a * a % c;                 // a -> a^2 -> a^4 -> a^8 ...
         b >>= 1;         
    }   
    return ret;
}

_Bool is_prime( int n){    
     int i;
     for(i = 2; i * i <= n; ++ i) if(n % i == 0) return 0;
     return 1;     
}

int getP(int Lim){
     // P = C * 2^21 + 1, P >= Lim
     int c;
     for(c = 3; ; ++ c){
         int t = c << 21 | 1;
         if( is_prime( t ) && t >= Lim) return t;        
     }
     return - 1;     
}

// Is g a primitive root modulo P
_Bool is_g( int a, int P){
     int i, p0 = P - 1;
     for(i = 1; i * i <= p0; ++ i)
           if( p0 % i == 0) {
               if( powmod( a, i, P) == 1 && i < p0) return 0;               // If i | (P-1), then i or (P-1)/i may be the delta_P(a).
               if( powmod( a, p0 / i, P) == 1 && p0 / i < p0) return 0;     // If for any i < P-1 and i | (P-1), a^i != 1 (mod P), then delta_P(a) = phi(P), so a is primitive root.
           }  
     return 1;   
} 

int getG( int P){
    int g;
    for(g = 2; !is_g( g, P ); ++ g);
    return g;    
}

/* _g[i] = G^(C*2^N / 2^i) (mod P), i = 0, 1, ..., blim - 1,
 * so _g[i] can see as the 2^i-th root of 1 (mod P)
 */
void get_g(int G, int P, int blim,int _g[]){            
    int i, j;
    for(i = 0; i < blim; ++ i){
          j = 1 << i;
          _g[ i ] = powmod( G, (P - 1) / j, P );                
    }    
}

// k is the bit-reverse of j. BIT_CNT is the length of bits.
int reverse(int j){
  int i,k;
  k = 0;
  for(i = 0;i < BIT_CNT;++i) if( j& (1 << i)) k |= 1 << (BIT_CNT - i - 1);
  return k; 
}

void FFT(int x[], int n){
  int i,j,m,t0, t1, i0, j0, tt;
  for(m = 1;m <= BIT_CNT;++ m){
        i0 = 1 << m;
        j0 = i0 >> 1;
    for(i = 0;i < n;i += i0 ) 
     for(j = 0, tt = 1;j < j0 ;++ j, tt = (LL)tt * _g[ m ] % P)  {
       t0 =  tt;
       t1 =  (LL) x[i + j + j0 ] * t0 % P;
      t0 = ( x[i + j] + t1) % P;
      t1 = (x[i + j] - t1) % P; 
            if( t1 < 0) t1 += P;
      x[i + j] = t0;
      x[ i + j + j0 ] = t1;
    } 
  }
}

void conv( int a[], int b[], int n) {
     int i;
     FFT( a , n);
     FFT( b , n);
     for(i = 0; i < n; ++ i) b[ i ] = (LL) a[ i ] * b[ i ] % P;
     for(i = 0; i < n; ++ i) a[ reverse( i ) ] = b[ i == 0 ? 0 : n - i ];
     FFT( a , n);
     for(i = 0; i < n; ++ i) a[ i ] = (LL) a[i] * Inv( n ) % P ;      
}

enum {maxn = 1 << 19};  // Or: #define length 256
// const int maxn = 1 << 19;
char A[ maxn ], B[ maxn ];
int a[ maxn ], b[ maxn ], n;

void init(){
     P = getP( 1000000000 );
     get_g( getG( P) , P, 21, _g) ;    
}

void get(){
     int i,j;
     printf("the length n:") ;
     scanf("%d", &n);    
     printf("the strings of two numbers A and B: ");
     scanf("%s%s", A, B);
     int v, c = 0,k = 0;
     int av, bv,t = 1;
     av = bv = 0;
     int on = n / 1 + (n % 1 != 0);
     for( BIT_CNT = 1; on + on > (1 << BIT_CNT); ++ BIT_CNT);
     for(i = n - 1; i >= 0; -- i) {
           av = av + t * (A[ i ] - '0');
           bv = bv + t * (B[ i ] - '0');
           ++ c;
           if(c == 1 || i == 0){
               j = reverse( k );
               a[ j ] = av;
               b[ j ] = bv;
               ++ k;
               c = av = bv = 0; t = 1;       
           }else t *= 10;       
     }
     n = 1 << BIT_CNT;
}

int ans[ maxn ];

void work(){
     int i, j = 0, k;
     conv( a, b, n );
     for(i = 0; i < n; ++ i){
           k = a[ i ] + j;
           ans[ i ] = k % 10;
           j = k / 10;      
     }
     for(i = n - 1; i >= 0 && ans[ i ] == 0; -- i);
     for( printf("%d", ans[i --]); i >= 0; -- i) printf("%d", ans[i]);
     puts("");  
}

int main(){
    init();
    get();
    work();
    //while( 1 ) ;
   return 0;
}