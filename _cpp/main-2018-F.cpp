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
https://contest.yandex.ru/algorithm2018/contest/8254/problems/F/

Инженер Яндекса Иван — очень скрупулёзный человек, из тех кто никогда не создаст массив на большее количество 
элементов, чем ему действительно требуется.
Сегодня у Ивана есть массив из n различных целых положительных чисел a1, a2, …, an, который он хочет положить 
в хеш-таблицу. Для этого он собирается выбрать некоторое значение m — количество корзин в хеш-таблице, а затем 
отправить число x в корзину . Таким образом, число a1 попадёт в корзину h(a1), a2 попадёт в корзину h(a2) и 
так далее.
Настолько же сильно, насколько Иван любит порядок, он ненавидит коллизии. Поэтому он хочет выбрать значение m таким 
образом, чтобы все значения  были различными. От вас требуется найти подходящее m. Поскольку эффективность во всём 
является одной из целей инженера, работающего с большими данными, Иван просит вас выбрать среди всех подходящих m 
минимальное.

Формат ввода
В первой строке входных данных записано число n (1 ≤ n ≤ 2 000 000) — количество чисел в массиве Ивана. В следующей 
строке записаны n целых чисел a1, a2, …, an (1 ≤ ai ≤ 2 000 000). Гарантируется, что все элементы последовательности 
a различны.

Формат вывода
Выведите одно целое число — минимальное значение m, такое что все числа  будут различными.

Пример 1
Ввод 3 1 2 3 Вывод 3
Пример 2
Ввод 3 1 2 4 Вывод 4
Пример 3
Ввод 5 1 3 6 10 15 Вывод 8
*/

int getPerfectMod(std::vector<int> &nums)
{
    std::sort(nums.begin(), nums.end());

    auto first = nums[0];
    for (auto &num : nums)
        num -= first;

    auto last = nums.back();
    std::vector<int> rems(last + 1, 0);
    int result = last + 1;
    int step = 1;
    int M = nums.size();
    first = std::lower_bound(nums.begin(), nums.end(), M) - nums.begin();
    assert(first == nums.size() || (nums[first] >= M && nums[first - 1] < M));
    for (; M < last; ++M, ++step)
    {
        while (first < nums.size() && nums[first] < M)
            first++;
        for (int i = 0; i < first; ++i)
            rems[nums[i]] = step;
        bool hasCollision = false;
        for (int i = first; i < nums.size(); ++i)
        {
            auto index = nums[i] % M;
            if (rems[index] == step)
            {
                hasCollision = true;
                break;
            }
            rems[index] = step;
        }
        if (!hasCollision)
        {
            result = M;
            break;
        }
    }

    // for(int i=1; i<nums.size(); ++i)
    // for(int j=0,R=nums[i]%result; j<i; ++j)
    // assert(R!=nums[j]%result);
    return result;
}

void read(std::vector<int> &A)
{
#if 1
    int N;
    std::cin >> N;
    A.resize(N);
    for (auto &a : A)
        std::cin >> a;
#else
    A.resize(20000);
    for (auto &a : A)
        a = rand() % (2 * A.size());

    std::sort(A.begin(), A.end());
    auto last = std::unique(A.begin(), A.end());
    A.erase(last, A.end());
    std::mt19937 g(123);
    std::shuffle(A.begin(), A.end(), g);
#endif
}

int main()
{
    std::vector<int> nums;

    read(nums);

    std::cout << getPerfectMod(nums) << std::endl;

    return 0;
}
