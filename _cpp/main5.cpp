#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

/*
https://coderun.yandex.ru/selections/hr-tech-interview/problems/sum-of-numbers/description

Вася очень любит везде искать своё счастливое число K. Каждый день он ходит в школу по улице, вдоль которой припарковано
N машин. Он заинтересовался вопросом, сколько существует отрезков из подряд идущих машин таких, что сумма их номеров равна K.
Помогите Васе узнать ответ на его вопрос.

Формат ввода
В первой строке входных данных задаются числа N и K (1≤N≤100000 1≤K≤10**9).

Во второй строке содержится N чисел, задающих номера машин. Номера машин могут принимать значения от 1 до 999 включительно.

Формат вывода
Необходимо вывести одно число — количество отрезков.

Примечание
В первом примере существуют четыре подходящих отрезка машин:

только первая машина — 17,
машины 2 и 3 — 7, 10,
машины 3 и 4 — 10, 7,
машины 4 и 5 — 7, 10.

Пример 1
Ввод
5 17
17 7 10 7 10
Вывод
4
Пример 2
Ввод
5 10
1 2 3 4 1
Вывод
2
*/

int main()
{
    long N,K; std::cin>>N>>K;
    std::vector<int> nums(N);
    for(auto& num: nums)
        std::cin>>num;

    int result = 0;
    long long sum = nums[0];
    int i=0;
    for (int j=1; j<nums.size(); )
    {
        if (sum==K)
        {
            result++;
            sum -= nums[i++];
            sum += nums[j++];
        }
        if (sum>K)
        {
            sum -= nums[i++];
        }
        if (sum<K)
        {
            sum += nums[j++];
        }
    }

    while (sum>K && i<nums.size())
        sum -= nums[i++];

    if (sum==K)
        result++;

    std::cout<<result<<std::endl;

    return 0;
}