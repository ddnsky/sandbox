#include <iostream>
#include <cassert>

//https://gist.github.com/qi7chen/4243253
#if 0
void unsigned_divide(unsigned int dividend,
		     unsigned int divisor,
		     unsigned int &quotient,
		     unsigned int &remainder )
{
  unsigned int t, num_bits;
  unsigned int q, bit, d;
  int i;

  remainder = 0;
  quotient = 0;

  if (divisor == 0)
    return;

  if (divisor > dividend) {
    remainder = dividend;
    return;
  }

  if (divisor == dividend) {
    quotient = 1;
    return;
  }

  num_bits = 32;

  while (remainder < divisor) {
    bit = (dividend & 0x80000000) >> 31;
    remainder = (remainder << 1) | bit;
    d = dividend;
    dividend = dividend << 1;
    num_bits--;
  }

  /* The loop, above, always goes one iteration too far.
     To avoid inserting an "if" statement inside the loop
     the last iteration is simply reversed. */
  dividend = d;
  remainder = remainder >> 1;
  num_bits++;

  for (i = 0; i < num_bits; i++) {
    bit = (dividend & 0x80000000) >> 31;
    remainder = (remainder << 1) | bit;
    t = remainder - divisor;
    q = !((t & 0x80000000) >> 31);
    dividend = dividend << 1;
    quotient = (quotient << 1) | q;
    if (q) {
       remainder = t;
     }
  }
}   /* unsigned_divide */
#endif



/*
int divide(int dividend, int divisor) {
    if(dividend==divisor)return 1;
    
    int sign=1;
    if ((dividend < 0 && divisor > 0) || (dividend > 0 && divisor < 0)){
        sign = -1;
    }  
    unsigned int n=abs(dividend),d=abs(divisor);  
    unsigned int ans=0;
    while(n>=d){
        int c=0;
        while(n> (d<<(c+1))){
            c++;
        }
        n-=d<<c;
        ans+=1<<c;
    }
    if(ans== (1<<31) && sign==1)return INT_MAX; 
    return ans*sign;

}
*/

/*
    int divide(int dividend, int divisor) {
        if (dividend == (1 << 31) && divisor == -1)
            return INT_MAX;
        uint32_t N = abs(dividend);
        uint32_t D = abs(divisor);
        bool isNegative = (dividend < 0) ^ (divisor < 0);
        if (D == 1)
            return dividend;
        if (D > N)
            return 0;
        if (D > (N >> 1))
            return isNegative ? -1 : 1;
        int R = 0, Q = 0;
        for (int i = 0; i < 32; ++i) {
            R = (R << 1) | (N >> 31);
            N <<= 1;
            Q <<= 1;
            if (R >= D) {
                R -= D;
                Q += 1;
            }
        }
        return isNegative ? -Q : Q;
    }
*/

uint32_t divide(uint32_t N, uint32_t D)
{
    if (D==0)
        return 0xFfFfFfFf;
    //if (D>N)
        //return 0;
    //if (D>(N>>1))
        //return 1;

    uint32_t Q = 0, R = 0; // initialize quotient and reminder to null

    for (int i=31; i>=0; --i)
    {
        R = (R<<1) | (N >> 31);
        N <<= 1;
        Q <<= 1;
        if (R >= D)
        {
            R = R - D;
            Q = Q + 1;
        }
    }
    return Q;
}

int main(int, char**){
    uint32_t n = 10;
    uint32_t d = 5;

    int k = -5;
    uint32_t kk = k;
    kk = ~kk + 1;


    assert(divide(10, 2)==5);
    assert(divide(12, 4)==3);
    assert(divide(12, 17)==0);
    assert(divide(13, 13)==1);
    assert(divide(25, 1)==25);

    for(int i=0; i<150; ++i)
    {
        auto n = 2 + rand()%(INT32_MAX-2);
        //auto n = 2 + rand()%1000;
        auto d = 1 + rand()%(n-1);
        assert(n/d==divide(n, d));
        std::cout << n << "/" << d << "=" << divide(n, d) << "(" << n/d << ")" << std::endl;
    }
    //std::cout << n << '/' << d << '=' << divide(n, d) << std::endl;
    return 0;
}
