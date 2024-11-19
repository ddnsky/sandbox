class Solution {
public:
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
};