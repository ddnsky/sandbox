#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <cassert>

/*
https://coderun.yandex.ru/selections/hr-tech-interview/problems/minimum-of-the-segment
11. Минимум на отрезке

Рассмотрим последовательность целых чисел длины n.
По ней двигается «окно» длины k: сначала в «окне» находятся первые k
чисел, на следующем шаге в «окне» уже будут находиться 
k чисел, начиная со второго, и так далее до конца последовательности.
Требуется для каждого положения «окна» определить минимум в нём.

Формат ввода
В первой строке входных данных содержатся два натуральных числа n и k (n≤150000, k≤10000, k≤n) 
— длины последовательности и «окна», соответственно.

На следующей строке находятся n целых чисел — сама последовательность.

Формат вывода
Выведите 
n−k+1 строк. В каждой строке должно быть одно число — минимум для соответствующего положения «окна».

Пример 1
Ввод
7 3
1 3 2 4 5 3 1
Вывод
1
2
2
3
1
*/

struct MapMinFinder
{
    std::map<int,int> _map;

    MapMinFinder(const int *begin, const int *end)
    {
        for(auto ii=begin; ii!=end; ++ii)
        {
            insert(*ii);
        }
    }
    int top()
    {
        return _map.begin()->first;
    }
    void insert(int num)
    {
        auto res = _map.insert({num, 1});
        if (!res.second)
        {
            res.first->second++;
        }
    }
    void update(int oldValue, int newValue)
    {
        if (newValue!=oldValue)
        {
            auto found = _map.find(oldValue);
            assert(found!=_map.end());
            if (found!=_map.end())
            {
                if (found->second>1)
                {
                    found->second--;
                }
                else
                {
                    _map.erase(found);
                }
            }
            insert(newValue);
        }
    }
};


int main() 
{
    int N,K; std::cin>>N>>K;
    std::vector<int> nums(N);
    for (auto& num: nums)
        std::cin>>num;

    if (!nums.empty())
    {
        MapMinFinder mins(&*nums.begin(), (&*nums.begin())+K);
        std::cout<<mins.top()<<std::endl;
    
        for(int i=1; i<=(N-K); ++i)
        {
            auto oldValue = nums[i-1];
            auto newValue = nums[i+K-1];
            mins.update(oldValue, newValue);
            std::cout<<mins.top()<<std::endl;
        }
    }

	return 0;
}