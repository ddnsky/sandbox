#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cassert>
#include <functional>
#include <fstream>

long walk2(int E, const std::vector<int>& A, int index=0)
{
    long cur = 0;
    while(index<A.size() && E>2)
    {
        long dE = A[index]*(index+1);
        if (dE+2<=E)
        {
            auto w1 = A[index]+(E-dE-2>2?walk2(E-dE-2, A, index+1):0);
            cur = std::max(cur, w1);
        }
        index++;
        E-=2;
    }
    return cur;
}


long walk(int E, const std::vector<int>& A)
{
    struct State
    {
        int Emin;
        int Emax;
        long Fmax;
        std::vector<long> food; // food+index indexed by energy

        State(int E)
            : Emin(E), Emax(E), Fmax(0) {
            food.reserve(std::max(32,E/4));
            food.resize(1,0);
        }

        void swap(State& state) {
            std::swap(Emin, state.Emin);
            std::swap(Emax, state.Emax);
            std::swap(Fmax, state.Fmax);
            std::swap(food, state.food);
        }

        bool step(State& prev, int index, const std::vector<int>& A)
        {
            const auto dEmin = 2;
            const auto dEmax = 2 + A[index-1]*index;

            if (prev.Emax<index+2)
                return false;
            
            Emin = std::max(prev.Emin-dEmax, 0);
            Emax = std::max(prev.Emax-dEmin, Emin);
            Fmax = prev.Fmax;

            food.erase(food.begin(), food.end());
            food.resize(Emax-Emin+1, -1);

            int assigned = 0;
            for (int E=prev.Emin,i=0; E<=prev.Emax; ++i, ++E)
            {
                if (prev.food[i]!=-1) {
                    if (E>=dEmax) {
                        auto e = E-dEmax;
                        food[e-Emin] = std::max(food[e-Emin], prev.food[i]+A[index-1]);
                        Fmax = std::max(Fmax, food[e-Emin]);
                        assigned++;
                    }
                    if (E>=dEmin) {
                        auto e = E-dEmin;
                        food[e-Emin] = std::max(food[e-Emin], prev.food[i]);
                        assigned++;
                    }
                }
            }

            return assigned;
        }
    };

    State cur{E};
    State next{E};

    for(int index=1; index<=A.size(); ++index) {
         if (bool res = next.step(cur, index, A)) 
            next.swap(cur);
        else
            break;
    }

    return cur.Fmax;
}


void read(int& E, std::vector<int>& A)
{
    int N; std::cin>>N>>E;
    A.resize(N);  
    for(auto& a: A)
        std::cin>>a;
}

int main()
{
    assert(5==walk(100, {1,1,1,1,1}));
    assert(6==walk(25, {3,30,1,3}));
    assert(0==walk(1, {3,30,1,3}));
    assert(5==walk(13, {2,3,3}));

    for(int i=0; i<30; ++i)
    {
        int N = 5 + rand()%1000;
        int E = 2 + rand()%200000;
        std::vector<int> A;
        A.resize(N);
        for(auto& a:A)
            a=rand()%(E/2);

        
        int res = walk(E, A);
        int res2 = walk2(E, A);
        assert(res==res2);
    }

    int E; std::vector<int> A;
    read(E, A);

    std::cout<<walk(E, A);

    return 0;
}
