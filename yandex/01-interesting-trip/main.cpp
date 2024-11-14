#include <fstream>
#include <iostream>
#include <queue>

int main()
{
    std::fstream inp;  inp.open("input.txt");
    //auto& inp = std::cin;

    int citiesCount = 0; inp >> citiesCount;

    struct City {
        long x, y;
        int steps = -1;
    };
    std::vector<City> cities(citiesCount);
    for (auto& city : cities) {
        inp >> city.x >> city.y;
    }
    int maxStep = 0, origin = 0, finish = 0;
    inp >> maxStep >> origin >> finish;
    origin -= 1;
    finish -= 1;

    if (origin == finish) {
        std::cout << '0' << std::endl;
        return 0;
    }
    const auto cmp = [&](int w1, int w2) {
        return cities[w1].steps > cities[w2].steps;
    };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> front(cmp);
    const auto pushToQueue = [&](int city, int steps) {
        cities[city].steps = steps;
        front.push(city);
    };
    pushToQueue(origin, 0);

    while (!front.empty() && cities[finish].steps==-1) {
        auto& fromCity = cities[front.top()];
        front.pop();
        for (int i = 0; i < cities.size(); ++i) {
            auto& toCity = cities[i];
            if (toCity.steps == -1 && std::abs(toCity.x-fromCity.x)+std::abs(toCity.y-fromCity.y) <= maxStep) {
                pushToQueue(i, fromCity.steps + 1);
            }
        }
    }

    std::cout << cities[finish].steps << std::endl;
    return 0;
}