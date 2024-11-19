#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cassert>
#include <functional>
#include <fstream>

/*
find continuous subsequence with target sum
*/

// O(n**2)
// returns pair with first and last indices of subsequence with target sum
std::pair<int,int> findTargetSumBruteForce(const std::vector<int> &nums, long target)
{
    for(int i=nums.size()-1; i>=0; --i)
    {
        long sum = 0;
        for(int j=i; j>=0; --j)
        {
            sum += nums[j];
            if (sum==target)
                return {j,i};
        }
    }
    return {0,-1};
}


// O(N*log(N))
std::pair<int,int> findTargetSum(const std::vector<int> &nums, long target)
{
    using PrefixSum = std::pair<long,int>;
    std::vector<PrefixSum> prefixSums(nums.size()+1);
    prefixSums[0]={0,0};
    for(int i=1; i<nums.size()+1; ++i)
    {
        prefixSums[i] = {nums[i-1]+prefixSums[i-1].first,i};
    }
    std::sort(prefixSums.begin(), prefixSums.end(), [](auto const& p1, auto const& p2){ return p1.first<p2.first || (p1.first==p2.first && p1.second<p2.second); });

    for(int x = 0; x<prefixSums.size()-1; x++)
    {
        PrefixSum sum = {std::abs(target)+prefixSums[x].first, prefixSums[x].second};
        auto begin = prefixSums.begin()+(x+1);
        auto found = std::lower_bound(begin, prefixSums.end(), sum, [](const PrefixSum& s, const PrefixSum& sum) { return s.first<sum.first || (s.first==sum.first && s.second<sum.second); });
        if (target>=0)
        {
            if (found!=prefixSums.end() && found->first==sum.first && found->second>sum.second)
            {
                return {prefixSums[x].second, found->second-1};
            }
        }
        else
        {
            if (found!=begin && (--found)->first==sum.first && found->second<sum.second)
            {
                return {found->second, prefixSums[x].second-1};
            }
        }
    }

    return {0,-1};
}


int main()
{
    std::vector<int> nums = {11,9,-3,7,-2,-1,2,4,-8,10,-10,5};

    const auto check1 = [](std::vector<int> const& nums, long target) {
        auto res = findTargetSum(nums, target);
        if (res.first<=res.second)
        {
            assert(std::accumulate(nums.begin()+res.first, nums.begin()+res.second+1, 0)==target);
        }
        else
        {
            auto r2 = findTargetSum2(nums, target);
            assert(res==r2);
        }
    };

    const auto check2 = [&](std::vector<int> const& nums) {
        check1(nums, std::accumulate(nums.begin(), nums.end(), 0));
        check1(nums, *std::min_element(nums.begin(), nums.end()));
        check1(nums, *std::max_element(nums.begin(), nums.end()));
        check1(nums, nums.front());
        check1(nums, nums.back());
        check1(nums, nums.front()+nums.back());
        check1(nums, std::accumulate(nums.begin()+1, nums.end()-1, 0));
        check1(nums, std::accumulate(nums.begin()+(nums.size()/4), nums.end()-(nums.size()/4), 0));

        auto a = 0;
        for(int i=0; i<10; ++i)
        {
            check1(nums, i);
            check1(nums, -i);
            check1(nums, 10+i);
            check1(nums, -10+i);

            check1(nums, rand()%(4*nums.size()));
            check1(nums, nums[a+=rand()%nums.size()]);
        }
    };
        

    auto res = findTargetSum(nums, -4);
    check2(nums);
    for(int i=0; i<100; ++i)
    {
        nums.resize(10+rand()%10);
        for(int i=0; i<nums.size(); ++i)
            nums[i] = -(rand()%nums.size()) + rand()%(2*nums.size());
        check2(nums);
    }
    
    return 0;
}
