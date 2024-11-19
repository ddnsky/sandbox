#include <iostream>
#include <string>
#include <vector>

#include <numeric>
#include <algorithm>
#include <random>
#include <cassert>


/*
3. Инверсии
Пусть p1, p2, …, pn перестановка чисел от 1 до n. Будем говорить, что пара индексов 
(i,j) образует инверсию, если i<j и pi>pj.

Задана некоторая перестановка 
(p1,…,pn), требуется определить среднее количество инверсий в перестановке, полученной из данной 
после одной перестановки пары элементов. При этом индексы переставляемых элементов выбираются 
равновероятно среди всех пар различных чисел от 1 до n.
Формат ввода
В первой строке записано одно целое число 
n (2≤n≤2000).

Во второй строке записаны 
n целых чисел p1, p2, …, pn (1≤pi≤n), все числа в строке различны.

Формат вывода
Выведите несократимую дробь 
a/b, задающую значение среднего числа инверсий по всем возможным парам переставляемых индексов элементов.

Ограничения
Ограничение времени 8 с
Ограничение памяти 1 ГБ

Пример 1
Ввод
5
1 2 3 4 5
Вывод
3/1
Пример 2
Ввод
3
3 1 2
Вывод
5/3
*/


int gcd(int N1, int N2)
{

    if (N1<N2)
        std::swap(N1, N2);
    
    while (N2>1)
    {
        auto R = N1%N2;
        if (R==0)
            break;
        N1=N2;
        N2=R;
    }
    return N2;
}

int getInversions(const int* nums, int n)
{
    int result = 0;
    for(int i=0; i<n-1; ++i)
        for(int j=i+1; j<n; ++j)
        {
            result += (nums[i]>nums[j]);
        }
    return result;    
}

void genPermutation(int* nums, int n)
{
    std::iota(nums, nums+n, 0);
    //std::random_device rd;
    std::mt19937 g(200);
    std::shuffle(nums, nums+n, g);
}

int main()
{
    /*
    int N = 10;
    std::vector<int> nums(N);
    std::iota(nums.begin(), nums.end(), 0);


    //std::random_device rd;
    std::mt19937 g(200);
    std::shuffle(nums.begin(), nums.end(), g);

    std::vector<int> inv1(N);
    std::vector<int> inv2(N);
    std::vector<int> inv3(N);
    for(int i=0,k=N-1; i<N; ++i,--k)
    {
        int res1 = 0;
        int res2 = 0;
        for(int j=i+1; j<N; ++j)
            res1 += nums[j]<nums[i];
        for(int j=k-1; j>=0; --j)
            res2 += nums[j]>nums[k];
        inv1[i] = res1;
        inv2[i] = res2;
    }
    for(int i=1; i<N; ++i)
    {
        inv1[i] += inv1[i-1];
        inv2[i] += inv2[i-1];
    }

    assert(inv1[N-1]==getInversions(&nums[0], N));
    assert(inv2[N-1]==getInversions(&nums[0], N));
    auto p1 = getInversions(&nums[4],N-4);
    assert(inv2[N-1]-inv1[3]==getInversions(&nums[4],N-4));
    //assert(inv2[N-2]-inv1[3]==getInversions(&nums[4],N-3));
    //assert(inv2[N-2]-inv1[3]==getInversions(&nums[4],N-3));
    auto getInv = [&](int a, int b)
    {
        return inv2[N-1-(N-1-b)]-(a>0? inv1[a-1]:0);
    };
    auto getInv2 = [&](int a, int b)
    {
        return getInversions(&nums[a], b-a+1);
    };
    assert(getInv(4,N-1)==getInv2(4,N-1));
    assert(getInv(4,N-2)==getInv2(4,N-2));
    assert(getInv(0,N-1)==getInv2(0,N-1));
    assert(getInv(5,7)==getInv2(5,7));
    assert(getInv(3,3)==getInv2(3,3));
    */

#if 0
    int n; std::cin>>n;
    std::vector<int> nums(n);
    for(auto& num: nums)
        std::cin>>num;
#else
    int n=100;
    std::vector<int> nums(n);
    genPermutation(&nums[0], nums.size());
#endif


    long N1 = 0;
    long N2 = 0;
    long originalInvs = 0;
    for(int i=0; i<n; ++i)
        for(int j=i+1; j<n; ++j)
            originalInvs += nums[i]>nums[j];
    assert(originalInvs==getInversions(&nums[0], nums.size()));


    std::vector<int> iinvs(n);
    for(int i=0; i<n; ++i)
    {
        for(int j=i+1; j<n; ++j)
        {
            bool jinv = iinvs[j] = nums[i]>nums[j];
            int dif = jinv? -1 : 1;
            for (int k=i+1; k<j; ++k)
            {
                int ijinv = iinvs[k] + (nums[k]>nums[j]);
                dif += 2*(1-ijinv);
            }
            int newInvs = originalInvs + dif;

            ++N1;
            std::swap(nums[i],nums[j]);
            assert(newInvs==getInversions(&nums[0], nums.size()));
            for(int p=0; p<n; ++p)
                for(int q=p+1; q<n; ++q)
                    if (nums[p]>nums[q])
                        ++N2;        
            std::swap(nums[i],nums[j]);
        }
    }

    if (N2!=0)
    {
        auto R = gcd(N1,N2);
        N1 /= R;
        N2 /= R;
    }
    std::cout<<N2<<"/"<<N1<<std::endl;
 
	return 0;
}