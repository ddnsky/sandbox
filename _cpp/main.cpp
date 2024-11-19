#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cassert>
#include <functional>
#include <fstream>
#include <deque>
#include <unordered_map>

long walk2(int E, const std::vector<int> &A, int index = 0)
{
    long cur = 0;
    while (index < A.size() && E > 2)
    {
        long dE = A[index] * (index + 1);
        if (dE + 2 <= E)
        {
            auto w1 = A[index] + (E - dE - 2 > 2 ? walk2(E - dE - 2, A, index + 1) : 0);
            cur = std::max(cur, w1);
        }
        index++;
        E -= 2;
    }
    return cur;
}


int gProcessed = 0;
long walk(int E, const std::vector<int> &A)
{
    struct State
    {
        long Fmax;
        std::unordered_map<int, long> food; // food+index indexed by energy

        State(int E)
            : Fmax(0)
        {
            food.reserve(std::max(32, E / 4));
            food.insert({E, 0});
        }

        void set(State &state)
        {
            Fmax = state.Fmax;
            food.swap(state.food);
        }

        bool step(State &prev, int index, const std::vector<int> &A)
        {
            const auto dEmin = 2;
            const auto dEmax = 2 + A[index - 1] * index;

            Fmax = prev.Fmax;
            food.clear();

            for (auto ii = prev.food.cbegin(); ii != prev.food.cend(); ++ii)
            {
                gProcessed++;
                auto oldE = ii->first;
                auto oldF = ii->second;
                if (oldE<2+index)
                    continue;
                if ((Fmax - oldF) * index + 2 < oldE)
                {
                    if (oldE >= dEmax)
                    {
                        auto newF = oldF + A[index - 1];
                        auto newE = oldE - dEmax;
                        if (newE>2+index) 
                        {
                            auto res = food.insert({newE, newF});
                            if (!res.second)
                            {
                                res.first->second = std::max(res.first->second, newF);
                            }
                        }
                        Fmax = std::max(Fmax, newF);
                    }
                    if (oldE >= dEmin)
                    {
                        auto newE = oldE - dEmin;
                        if (newE>2+index) 
                        {
                            auto res = food.insert({newE, oldF});
                            if (!res.second)
                            {
                                res.first->second = std::max(res.first->second, oldF);
                            }
                        }
                    }
                }
            }

            return !food.empty();
        }
    };

    State cur{E};
    State next{E};

    for (int index = 1; index <= A.size(); ++index)
    {
        if (next.step(cur, index, A))
            cur.set(next);
        else
            break;
    }

    return cur.Fmax;
}

void read(int &E, std::vector<int> &A)
{
    int N;
    std::cin >> N >> E;
    A.resize(N);
    for (auto &a : A)
        std::cin >> a;
}

int main()
{
    assert(5 == walk(100, {1, 1, 1, 1, 1}));
    assert(6 == walk(25, {3, 30, 1, 3}));
    assert(0 == walk(1, {3, 30, 1, 3}));
    assert(5 == walk(13, {2, 3, 3}));

    for (int i = 0; i < 35; ++i)
    {
        int N = 5 + rand() % 1000;
        int E = 2 + rand() % 200000;
        std::vector<int> A;
        A.resize(N);
        for (auto &a : A)
            a = rand() % (E / 2);

        int res = walk(E, A);
        int res2 = walk2(E, A);
        assert(res == res2);
    }

    std::cout<<gProcessed<<std::endl;

    int E;
    std::vector<int> A;
    read(E, A);

    std::cout << walk(E, A);

    return 0;
}
