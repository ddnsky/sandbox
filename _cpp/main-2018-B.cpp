#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cassert>
#include <functional>
#include <fstream>

void read(std::vector<int> &nums)
{
#define DO_TESTS 1
#if !DO_TESTS
    int n;
    std::cin >> n;
    nums.resize(n);
    for (auto &num : nums)
        std::cin >> num;
#define CHECK_LIS(nums, lis) (void)0
#define CHECK_LDS(nums, lds) (void)0
#define NUM_ITERS 1
#else
    static std::mt19937 g(200);
    nums.resize(2000);
    std::iota(nums.begin(), nums.end(), 0);

    //std::reverse(nums.begin()+1, nums.end());
    std::shuffle(nums.begin(), nums.end(), g);


#define CHECK_LIS(nums, lis)             \
    for (int i = 1; i < lis.size(); ++i) \
        assert(lis[i - 1] < lis[i] && nums[lis[i - 1]] < nums[lis[i]]);
#define CHECK_LDS(nums, lds)             \
    for (int i = 1; i < lds.size(); ++i) \
        assert(lds[i - 1]<lds[i] && nums[lds[i - 1]]> nums[lds[i]]);
#define NUM_ITERS 1
#endif
}

#if 1
bool findLIS_ALL(std::vector<int> const& nums, int N, const std::function<bool(std::vector<int>&)>& onFound)
{
    std::vector<std::vector<std::pair<int,int>>> maxx;
    maxx.reserve(N);

    for(int i=0; i<nums.size(); ++i)
    {
        auto jmax = std::upper_bound(maxx.begin(), maxx.end(), nums[i], [&nums](int val, std::vector<std::pair<int,int>>& idx) { return val<nums[idx.back().first];} ) - maxx.begin();
        if (jmax==0 || nums[maxx[jmax-1].back().first] < nums[i])
        {
            int prev = jmax>0? maxx[jmax-1].size()-1 : -1;
            if (jmax==maxx.size())
            {
                maxx.push_back({{i,prev}});
            }
            else
            {
                maxx[jmax].push_back({i,prev});
            }
        }
    }
    assert(maxx.size()==N);
    struct Result
    {
        int N; 
        std::vector<int> result;
        const std::vector<int>& nums;
        std::vector<std::vector<std::pair<int,int>>>& maxx;
        const std::function<bool(std::vector<int>&)>& onFound;

        Result(int N, std::vector<std::vector<std::pair<int,int>>>& maxx, const std::vector<int>& nums, const std::function<bool(std::vector<int>&)>& onFound)
            : N(N)
            , nums(nums)
            , maxx(maxx)
            , onFound(onFound) 
        {
            result.resize(N);
        }
        bool process()
        {
            while (!maxx[N-1].empty())
            {
                auto& In = maxx[N-1].back();
                maxx[N-1].pop_back();
                result[N-1] = In.first;
                if (process(N-2, In.second))
                    return true;
            }
            return false;
        }
        bool process(int J, int prev)
        {
            if (J<0)
            {
                assert(prev==-1);
                return onFound(result);
            }
            assert(prev>=0);
            while (maxx[J].size()-1>prev)
            {
                maxx[J].pop_back();
            }
            int next = result[J+1];
            for(int k=maxx[J].size()-1; k>=0; --k)
            {
                auto& cur = maxx[J][k]; 
                assert(cur.first<next);
                if (nums[cur.first]>nums[next])
                    break;
                result[J]=cur.first;
                if (process(J-1, cur.second))
                    return true;
            }
            return false;
        }
    };
    
    Result result(N, maxx, nums, onFound);
    return result.process();
}
#else
bool findLIS_ALL(std::vector<int> const &nums, int N, const std::function<bool(std::vector<int> &)> &onFound, int I = 0, int Jmax = 0, std::vector<int> maxx = {})
{
    for (int i = I; i < nums.size(); ++i)
    {
        // auto jmax = std::upper_bound(maxx.begin(), maxx.end(), nums[i], [&nums](int val, int idx) { return val<nums[idx];} ) - maxx.begin();
        int jmax = 0;
        for (; jmax < maxx.size(); ++jmax)
        {
            if (nums[i] < nums[maxx[jmax]])
                break;
        }
        // assert(found==jmax);
        if (jmax == 0 || nums[maxx[jmax - 1]] < nums[i])
        {
            if (jmax == maxx.size())
            {
                maxx.push_back(i);
            }
            else
            {
                if (jmax + nums.size() - I >= N && jmax >= Jmax)
                {
                    std::vector<int> newMaxx(maxx);
                    newMaxx[jmax] = i;
                    newMaxx.resize(jmax + 1);
                    if (findLIS_ALL(nums, N, onFound, i + 1, jmax + 1, std::move(newMaxx)))
                        return true;
                }
            }
        }
    }

    return maxx.size() == N && onFound(maxx);
}
#endif

template <typename Less>
void findLIS(std::vector<int> const &nums, const Less &less, int const *mask, std::vector<int> &maxx)
{
    maxx.resize(0);
    std::vector<int> prev(nums.size(), -1);

    for (int i = 0; i < nums.size(); ++i)
    {
        if (mask && mask[i])
            continue;
        auto jmax = std::upper_bound(maxx.begin(), maxx.end(), nums[i], [&nums, &less](int val, int idx)
                                     { return less(val, nums[idx]); }) -
                    maxx.begin();
        if (jmax == 0 || less(nums[maxx[jmax - 1]], nums[i]))
        {
            if (jmax == maxx.size())
            {
                maxx.push_back(i);
            }
            else
            {
                maxx[jmax] = i;
            }
            if (jmax > 0)
                prev[i] = maxx[jmax - 1];
        }
    }

    if (maxx.size() > 1)
    {
        for (int i = maxx.size() - 2; i >= 0; --i)
        {
            maxx[i] = prev[maxx[i + 1]];
        }
    }
}

int main()
{
    std::vector<int> nums;
    std::vector<int> lis;
    std::vector<int> lds;
    std::vector<int> lds2;
    std::vector<int> mask;

    const auto getMask = [&](std::vector<int> const &idx) -> int const *
    {
        mask.resize(0);
        mask.resize(nums.size(), false);
        for (auto const i : idx)
            mask[i] = true;
        return &mask[0];
    };

    std::ostream& sout = std::cout;
    //std::ofstream sout("output.txt");

    for (int i = 0; i < NUM_ITERS; ++i)
    {
        read(nums);

        findLIS(nums, std::less<int>(), nullptr, lis);
        findLIS(nums, std::greater<int>(), nullptr, lds);

        CHECK_LIS(nums, lis);
        CHECK_LDS(nums, lds);

        if (lis.size() + lds.size() <= nums.size())
        {
            findLIS(nums, std::greater<int>(), getMask(lis), lds2);
            CHECK_LDS(nums, lds2);

            if (lds.size() != lds2.size())
            {
                const auto onFound = [&](std::vector<int> &a)
                {
                    CHECK_LIS(nums, a);
                    findLIS(nums, std::greater<int>(), getMask(a), lds2);
                    CHECK_LDS(nums, lds2);
                    if (lds2.size() == lds.size())
                    {
                        lis = a;
                        return true;
                    }
                    return false;
                };
                findLIS_ALL(nums, lis.size(), onFound);
            }
        }

#if DO_TESTS
        sout << "\n"
             << i << ")============\n";
        sout << "nums[" << nums.size() << "]";
        sout << " lis=" << lis.size() << " lds=" << lds.size() <<std::endl;
        //for (auto &a : nums)
            //sout << (a + 1) << " ";
        sout << std::endl;
#endif

        if (lds.size() != lds2.size())
        {
            sout << "IMPOSSIBLE" << std::endl;
        }
        else
        {
            sout << lis.size() << std::endl;
            for (auto &a : lis)
                sout << (a + 1) << " ";
            sout << std::endl
                 << lds2.size() << std::endl;
            for (auto &a : lds2)
                sout << (a + 1) << " ";
            sout << std::endl;
        }
    }

    return 0;
}
