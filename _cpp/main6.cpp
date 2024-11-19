#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <deque>

/*
9. Длина кратчайшего пути
Дан неориентированный граф. Найдите длину минимального пути между двумя вершинами.
Формат ввода
В первой строке записано целое число N (1≤N≤100) – количество вершин в графе.
Далее записывается матрица смежности — N строк, в каждой из которых содержится чисел 0 или 1,
разделённых пробелом. Число 0 означает отсутствие ребра, а 1 — наличие ребра.
В последней строке задаются номера двух вершин — начальной и конечной.
Вершины нумеруются с единицы.

Формат вывода
Выведите длину кратчайшего пути — минимальное количество ребер, которые нужно пройти.
Если пути нет, нужно вывести -1.

Пример 1
Ввод
10
0 1 0 0 0 0 0 0 0 0
1 0 0 1 1 0 1 0 0 0
0 0 0 0 1 0 0 0 1 0
0 1 0 0 0 0 1 0 0 0
0 1 1 0 0 0 0 0 0 1
0 0 0 0 0 0 1 0 0 1
0 1 0 1 0 1 0 0 0 0
0 0 0 0 0 0 0 0 1 0
0 0 1 0 0 0 0 1 0 0
0 0 0 0 1 1 0 0 0 0
5 4
Вывод
2
Пример 2
Ввод
5
0 1 0 0 1
1 0 1 0 0
0 1 0 0 0
0 0 0 0 0
1 0 0 0 0
3 5
Вывод
3
*/

int main()
{
    int N; std::cin>>N;
    bool A[100][100];

    std::cin>>std::noboolalpha;
    for(int i=0; i<N; ++i)
    {
        for (int j=0; j<N; ++j)
            std::cin>>A[i][j];
    }
    int start, finish; std::cin>>start>>finish;
    start-=1;
    finish-=1;

    if (start==finish)
    {
        std::cout<<0<<std::endl;
        return 0;
    }

    struct WayPoint
    {
        int numSteps = -1;
    };
    std::vector<WayPoint> points(N);
    std::deque<int> front = {start};
    points[start].numSteps = 0;
    int result = -1;

    while (!front.empty() && result==-1)
    {
        auto nv = front.front();
        auto& v = points[nv];
        front.pop_front();

        for (int i=0; i<N; ++i)
        {
            if (A[nv][i] && points[i].numSteps==-1)
            {
                if (i==finish)
                {
                    result = v.numSteps + 1;
                    break;
                }
                points[i].numSteps = v.numSteps + 1;
                front.push_back(i);
            }
        }
    }

    std::cout<<result<<std::endl;
    return 0;
}