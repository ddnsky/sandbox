#include <fstream>
#include <iostream>
#include <queue>
#include <cassert>
#include <numeric>
#include <algorithm>


/*
https://coderun.yandex.ru/selections/code-life-balance/problems/stylish-clothes

Глеб обожает шоппинг. Как-то раз он загорелся идеей подобрать себе майку и штаны так, чтобы выглядеть в них максимально стильно. 
В понимании Глеба стильность одежды тем больше, чем меньше разница в цвете элементов его одежды.
В наличии имеется N (1  ≤  N  ≤  100 000) маек и M (1  ≤  M  ≤  100 000) штанов, про каждый элемент известен его цвет 
(целое число от 1 до 10 000 000). Помогите Глебу выбрать одну майку и одни штаны так, чтобы разница в их цвете была как можно меньше.

Формат ввода
Сначала вводится информация о майках: в первой строке целое число N (1  ≤  N  ≤  100 000) и во второй N целых чисел от 1 до 10 000 000 — 
цвета имеющихся в наличии маек. Гарантируется, что номера цветов идут в возрастающем порядке (в частности, цвета никаких двух маек не совпадают).

Далее в том же формате идёт описание штанов: их количество M (1  ≤  M  ≤  100 000) и в следующей строке M целых чисел от 1 до 10 000 000 
в возрастающем порядке — цвета штанов.

Формат вывода
Выведите пару неотрицательных чисел — цвет майки и цвет штанов, которые следует выбрать Глебу. Если вариантов выбора несколько, 
выведите любой из них.

Ограничения
Ограничение времени 1 с
Ограничение памяти 64 МБ

Пример 1 
Ввод
2
3 4
3
1 2 3
Вывод 3 3

Пример 2
Ввод
2
4 5
3
1 2 3
Вывод 4 3
Пример 3
Ввод
5
1 2 3 4 5
5
1 2 3 4 5
Вывод 1 1
*/

int main()
{
    long N; std::cin>>N;
    std::vector<long> colors[2];
    colors[0].resize(N);
    for(auto& c: colors[0])
        std::cin>>c;
    long M; std::cin>>M;
    colors[1].resize(M);
    for(auto& c: colors[1])
        std::cin>>c;

    auto curValue = colors[0][0];
    int curVector = 0;
    long curIndex = 1;
    long otherIndex = 0;
    
    if (colors[0][0]>colors[1][0])
    {
        curValue = colors[1][0];
        curVector = 1;
    }

    long bestValues[2] = {colors[0][0], colors[1][0]};
    auto bestLoss = std::abs(bestValues[0]-bestValues[1]);
        
    while(bestLoss>0 && curIndex<colors[curVector].size())
    {
        if (colors[curVector][curIndex] <= colors[curVector^1][otherIndex])             
        {
            curValue = colors[curVector][curIndex];
            curIndex++;
            continue;
        }    
        auto newLoss = std::abs(curValue-colors[curVector^1][otherIndex]);
        if (newLoss<bestLoss)
        {
            bestLoss = newLoss;
            bestValues[curVector]=curValue;
            bestValues[curVector^1]=colors[curVector^1][otherIndex];
        }
        std::swap(curIndex, otherIndex);
        curVector ^= 1;
        curValue = colors[curVector][curIndex];
        curIndex++;
    }

    if ((curIndex == colors[curVector].size()) && (otherIndex < colors[curVector^1].size()))
    {
        auto newLoss = std::abs(curValue-colors[curVector^1][otherIndex]);
        if (newLoss<bestLoss)
        {
            bestLoss = newLoss;
            bestValues[curVector]=curValue;
            bestValues[curVector^1]=colors[curVector^1][otherIndex];
        }
    }

    std::cout<<bestValues[0]<<" "<<bestValues[1]<<std::endl;

	return 0;
}