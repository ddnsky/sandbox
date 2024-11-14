#include <fstream>
#include <iostream>
#include <queue>
#include <cassert>
#include <cmath>

int main()
{
    long gcd, lcm; std::cin>>gcd>>lcm;

    if (lcm%gcd!=0)
    {
        std::cout<<"0"<<std::endl;
        return 0;
    }

    lcm /= gcd;
    long count = 0;
    for (long div=2; div*div<=lcm; div++)
    {
        if (lcm%div==0)
        {
            do lcm /= div; while (lcm%div==0);
            count++;
        }
    }
    if (lcm>1)
        count++;

    std::cout<<(1UL<<count)<<std::endl;
    return 0;
}