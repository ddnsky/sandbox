#include <iostream>
#include <string>
#include <vector>

#include <cassert>


// https://leetcode.com/problems/length-of-the-longest-subsequence-that-sums-to-target/

int lengthOfLongestSubsequence(const std::vector<int>& nums, int target) {
    const int Inf = -(int)nums.size()-10;
    int dp[1001];
    std::fill(dp, dp + (target+1), Inf);
    dp[0]=0;

    for(auto num: nums)
    {
        for(int sum=target; sum>=num; --sum)
        {
            dp[sum] = std::max(dp[sum], dp[sum-num]+1);
        }
    }

    return dp[target]>0? dp[target] : -1;
}

int main()
{
    assert( 3 == lengthOfLongestSubsequence({1,2,3,4,5}, 9));
    assert( 4 == lengthOfLongestSubsequence({4,1,3,2,1,5}, 7));
    assert(-1 == lengthOfLongestSubsequence({1,1,5,4,5}, 3));

	return 0;
}