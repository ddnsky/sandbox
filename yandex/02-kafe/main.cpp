#include <fstream>
#include <iostream>
#include <queue>
#include <cassert>
#include <numeric>
#include <algorithm>

int getMinCost(std::vector<int> const& costs, int I, int& unused, std::vector<int>& days)
{
    if (I>=costs.size())
        return 0;

    if (I==costs.size()-1)
    {
        if (!unused)
        {
            unused = costs.back()>100;
            return costs.back();
        }
        unused -= 1;
        days.push_back(I);
        return 0;
    }
        

    int acc = 0;
    int originalSize = days.size();

    for (int i=I; i<costs.size(); ++i)
    {
        auto cur = costs[i];
        if (unused==0)
        {
            acc += cur;
            unused += (cur>100);
            continue;
        }

        std::vector<int> minDays = days;
        int minUnused = unused + (cur>100);
        int minCost = acc + cur + getMinCost(costs, i+1, minUnused, minDays);
        
        if (unused>0)
        {
            days.push_back(i);
            int newUnused = unused-1;
            int newCost = acc + getMinCost(costs, i+1, newUnused, days);
            if (minCost>newCost || (minCost==newCost && minUnused>newUnused)/*|| (minCost==newCost && minUnused==newUnused)*/)
            {
                minCost=newCost;
                minUnused=newUnused;
                std::swap(minDays, days);
            }
        }
        unused = minUnused;
        std::swap(days, minDays);
        return minCost;
    }
    return acc;
}

int getMinCost2(std::vector<int> const& costs, int& unused, std::vector<int>& days)
{
    if (costs.empty())
    {
        return 0;
    }
    const int N = 101;
    const int INF = 300*300+300;
    int dp[N][N];
    signed char prev[N][N+1];
    int n = costs.size();
    for (int i=0; i<n; ++i)
        for(int j=0; j<n+1; ++j)
        {
            dp[i][j] = INF;
            prev[i][j] = 0;
        }
            
    int H = 1 + (costs[0]>100);        
    dp[0][H-1]=costs[0];
    
    for(int i=1; i<n; ++i)
    {
        for(int j=1; j<H; ++j)
        {
            dp[i][j-1]=dp[i-1][j];
            prev[i][j-1]=1;
        }
        const auto cost = costs[i]; 
        const int hasBonus = cost>100;
        for(int j=0; j<H; ++j)
        {
            if (dp[i][j+hasBonus]>=dp[i-1][j]+cost)
            {
                dp[i][j+hasBonus]=dp[i-1][j]+cost;
                prev[i][j+hasBonus]=-hasBonus;
            }
        }
        H += hasBonus;
    }

    int res = INF;
    unused = 0;
    for(int j=0; j<H; ++j)
        if (res >= dp[n-1][j])
        {
            res = dp[n-1][j];
            unused = j;
        }
    for(int i=n-1,j=unused; i>0; --i)
    {
        if (prev[i][j]==1)
        {
            days.push_back(i);
        }
        j += prev[i][j];
        assert(j>=0);
    }
    std::reverse(days.begin(), days.end());
    return res;
}

int main()
{
    //5 35 40 101 59 63
    //235 0 1
    //5
    //
    //5 110 40 120 110 60
    //260 0 2
    //3 5
    //
    //3 110 110 110
    //220 1 1
    //2
    //4 102 90 102 83
    //204 0 2
    //2 4
    //5 101 112 100 108 96
    //0 2
    //3 4 | 2 5

    if (1)
    {
        std::vector<int> costs(8, 110);
        std::vector<int> days1,days2;
        int daysNotEq = 0;

        int unused_=0;
        int cost_=getMinCost2({101,112,100,108,96}, unused_, days2);


        for (int i=0; i<100000; ++i)
        {
            int unused1=0, unused2=0;
            costs.resize(2+rand()%5);
            //costs.resize(3);
            days1.clear();
            days2.clear();
            for(int i=0; i<costs.size(); ++i)
            {
                costs[i] = 80 + rand()%40;
            }
            int cost1=getMinCost(costs, 0, unused1, days1);
            int cost2=getMinCost2(costs, unused2, days2);
            assert(cost1==cost2);
            assert(unused1==unused2);
             
            if (days1!=days2)
            {
                daysNotEq++;
            }
        }
        std::cout<<daysNotEq<<std::endl;
        assert(daysNotEq==0);
    }

    size_t N; std::cin>>N;
    std::vector<int> costs(N);
    for(auto& cost: costs)
        std::cin>>cost;

    std::vector<int> days;
    int unused = 0;
    int minCost = getMinCost(costs, 0, unused, days);
    //int minCost = getMinCost2(costs, unused, days);

    std::cout<<minCost<<std::endl;
    std::cout<<unused<<" "<<days.size()<<std::endl;
    for(auto& day: days)
        std::cout<<day+1<<std::endl;
 
	return 0;
}