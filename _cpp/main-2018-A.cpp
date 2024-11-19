#include <iostream>
#include <vector>


int main()
{
    const int Mil = 1e6;
    int b,c; std::cin>>b>>c;
    int r,d; std::cin>>r>>d;
    int result = 0;

    auto Rmil = r/Mil;
    auto Rrub = r%Mil;


    for(;;)
    {
        auto oldResult = result;

        if (Rrub==0)
        {
            result = b/Rmil + (b%Rmil+c/Mil)/Rmil;
            break;
        }

        if (r<=c)
        {
            auto numBottles = c/r;
            result += numBottles;
            auto dc = r*numBottles;
            c -= dc;
            d += dc;
        }

        if (Rrub<=c && Rmil)
        {
            auto numBottles = std::min(c/Rrub, b/Rmil);
            if (numBottles>0)
            {
                result += numBottles;
                b -= numBottles*Rmil;
                auto dc = numBottles*Rrub;
                c -= dc;
                d += dc; 
            }
        }

        auto Cmil = std::min((c-Rrub)/Mil, Rmil);
        if (c>=Rrub && b>=Rmil-Cmil)
        {
            b -= Rmil-Cmil;
            auto dc = Cmil*Mil+Rrub;
            c -= dc;
            d += dc;
            result += 1;
        }

        if (b>Rmil && d>=Mil-Rrub)
        {
            if (b<2*Rmil+1 || c+Mil-Rrub<Rrub)
            {
                b -= Rmil+1;
                auto dd = Mil-Rrub;
                c += dd;
                d -= dd;
                result += 1;
            }
            else
            {
                auto n1 = b/(2*Rmil+1);
                auto dc = (Mil-2*Rrub);

                if (dc==0)
                {
                    b %= (2*Rmil+1);
                    result += 2*n1;
                }
                else if (dc>0)
                {
                    auto n2 = (d-(Mil-Rrub)) / dc + 1;
                    auto n = std::min(n1, n2);
                    b -= n*(2*Rmil+1);
                    result += 2*n;
                    d -= n*dc;
                    c += n*dc;
                }
                else
                {
                    auto n2 = c / -dc;
                    auto n = std::min(n1, n2);
                    b -= n*(2*Rmil+1);
                    result += 2*n;
                    d -= n*dc;
                    c += n*dc;
                }
            }
        }

        if (result==oldResult)
            break;
    }

    std::cout<<result<<std::endl;

    return 0;
}