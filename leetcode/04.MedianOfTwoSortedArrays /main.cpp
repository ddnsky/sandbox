#include <algorithm>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

class Solution {
public:
  static double getMedian(const int *nums1, size_t n1, const int *nums2,
                          size_t n2, int N, int N0) {
    // two pointers method - returns median by linear search
    // N - total count of numbers
    // N0 - count of numbers already has been counted

    int Imed0 = (N - 1) / 2 - N0;
    for (; Imed0 > 0 && n1 && n2; --Imed0) {
      if (nums1[0] < nums2[0]) {
        --n1;
        ++nums1;
      } else {
        --n2;
        ++nums2;
      }
    }

    const int dImed = (N % 2 == 0);
    if (n1 == 0) {
      return (nums2[Imed0] + nums2[Imed0 + dImed]) * 0.5;
    }
    if (n2 == 0) {
      return (nums1[Imed0] + nums1[Imed0 + dImed]) * 0.5;
    }

    auto const p1 = (nums1[0] < nums2[0]) ? (--n1, *nums1++) : (--n2, *nums2++);
    if (!dImed)
      return p1;

    auto const p2 =
        n2 == 0 || (n1 > 0 && nums1[0] < nums2[0]) ? *nums1 : *nums2;
    return (p1 + p2) * 0.5;
  }

  double findMedianSortedArrays(std::vector<int> &nums1,
                                std::vector<int> &nums2) {
    if (nums1.empty()) {
        return getMedian(&nums2[0], nums2.size(), nullptr, 0, nums2.size(), 0);
    }
    if (nums2.empty()) {
        return getMedian(&nums1[0], nums1.size(), nullptr, 0, nums1.size(), 0);
    }
    size_t N0 = 0, N1 = 0;
    size_t n1 = nums1.size();
    size_t n2 = nums2.size();
    size_t N = n1 + n2;
    auto *p1 = &nums1[0];
    auto *p2 = &nums2[0];
    size_t Imed0 = (N - 1) / 2;
    size_t Imed1 = N / 2;

    for (;;) {
      if (n1 < n2) {
        std::swap(p1, p2);
        std::swap(n1, n2);
      }
      if (n1 <= 8 || n2 == 0) {
        return getMedian(p1, n1, p2, n2, N, N0);
      }

      auto ival = n1 / 2;
      auto n1left = ival + 1;
      auto p2med = std::upper_bound(p2, p2 + n2, p1[ival]);
      auto n2left = p2med - p2;
      auto N1new = N1 + n1 - n1left + n2 - n2left;

      if (Imed1 < N - N1new) {
        N1 = N1new;
        n1 = n1left;
        n2 = n2left;
        continue;
      }

      if (Imed0 <= N0 + n1left + n2left) {
        ival = (n1 - 1) / 2;
        n1left = ival;
        p2med = std::lower_bound(p2, p2 + n2, p1[ival]);
        n2left = p2med - p2;
      }

      p1 += n1left;
      p2 += n2left;

      N0 = N0 + n1left + n2left;
      n1 = n1 - n1left;
      n2 = n2 - n2left;
    }
  }
};

static double getMedianSortedArrays(std::vector<int> &nums1,
                                    std::vector<int> &nums2) {
  return Solution::getMedian(&nums1[0], nums1.size(), &nums2[0], nums2.size(),
                             nums1.size() + nums2.size(), 0);
}

int main(int, char **) {
  std::cout << "Hello, from MedianofTwoSortedArrays!\n";
  Solution s;

  {
    std::vector<int> nums1 = {1, 3}, nums2 = {2};
    assert(getMedianSortedArrays(nums1, nums2) == 2);
    assert(s.findMedianSortedArrays(nums1, nums2) == 2);
  }
  {
    std::vector<int> nums1 = {1, 2}, nums2 = {3, 4};
    assert(getMedianSortedArrays(nums1, nums2) == 2.5);
    assert(s.findMedianSortedArrays(nums1, nums2) == 2.5);
  }
  {
    std::vector<int> nums1 = {1, 2, 3, 4, 5, 6, 7, 8}, nums2 = {3, 4};
    assert(getMedianSortedArrays(nums1, nums2) == 4);
    assert(s.findMedianSortedArrays(nums1, nums2) == 4);
  }
  {
    std::vector<int> nums1 = {3, 4, 5, 6, 7, 8}, nums2 = {1, 2};
    assert(getMedianSortedArrays(nums1, nums2) == 4.5);
    assert(s.findMedianSortedArrays(nums1, nums2) == 4.5);
  }

  std::vector<int> nums1, nums2;
  for (int i = 0; i < 100000; ++i) {
    auto n1 = 2 + rand() % 200;
    auto n2 = 2 + rand() % 200;
    nums1.resize(n1);
    nums2.resize(n2);

    if (i % 2 == 0) {
      auto I0 = rand() % 256;
      auto I1 = I0 - rand() % 500;
      for (auto &num : nums1) {
        num = I0++;
      }
      for (auto &num : nums2) {
        num = I1++;
      }
    } else {
      for (auto &num : nums1) {
        num = rand() % 500;
      }
      for (auto &num : nums2) {
        num = rand() % 500;
      }
      std::sort(nums1.begin(), nums1.end());
      std::sort(nums2.begin(), nums2.end());
    }

    auto med1 = getMedianSortedArrays(nums1, nums2);
    auto med2 = s.findMedianSortedArrays(nums1, nums2);
    assert(med1 == med2);
  }

  return 0;
}
