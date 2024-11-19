#include <fstream>
#include <iostream>
#include <queue>
#include <cassert>
#include <cmath>
#include <map>


class Intervals
{
public:
    int occupy(int index)
    {
        assert(index<_map.size());
        int found = -1;
        for (int i=index; i>=0; )
        {
            if (_map[i]==-1)
            {
                _map[i] = 1;
                found = i;
                occupied++;
                break;
            }
            i -= _map[i];
        }
        return found;
    }

    Intervals(int maxIndex)
    {
        _map.resize(maxIndex+1, -1);
    }

    bool empty() const
    {
        return occupied==_map.size();
    }
private:
    int occupied = 0;
    std::vector<int> _map;
};


class Intervals1
{
public:
    int occupy(int index)
    {
        assert(index<_map.size());
        int found = -1;
        for (int i=index; i>=0; )
        {
            if (_map[i]==-1)
            {
                found = i;
                int dI = i>0 && _map[i-1]>0? _map[i-1] + 1 : 1;
                _map[i] = dI;
                int numIterations = index-i;
                if (numIterations>3)
                {
                    ++dI;
                    for (int j=i+1; j<_map.size() && _map[j]!=-1 && _map[j] < dI; ++j, ++dI)
                        _map[j] = dI;
                }
                break;
            }
            i -= _map[i];
        }
        return found;
    }

    Intervals1(int maxIndex)
    {
        _map.resize(maxIndex+1, -1);
    }

private:
    std::vector<int> _map;
};




class Intervals2
{
public:
    int occupy(int index)
    {
        if (_map.empty())
        {
            return -1;
        }

        auto found = _map.lower_bound(index);
        if (found==_map.end())
        {
            --found;
        }
        if (index<found->second.begin && found!=_map.begin())
        {
            --found;
        }
        if (found->second.begin>index)
        {
            return -1;
        }

        auto& interval = found->second;
        auto last = found->first;
        assert(interval.begin<=index);
        assert(last-interval.begin+1>interval.occupied);

        auto newOccupied = last - index + 1;
        if (newOccupied<=interval.occupied+1)
        {
            int result = last-interval.occupied;
            interval.occupied++;
            if (last-interval.begin+1==interval.occupied)
            {
                _map.erase(found);
            }
            return result;
        }

        auto begin = interval.begin;
        interval.begin = index + 1;
        assert(last-interval.begin+1>interval.occupied);

        if (index==begin)
        {
            return index;
        }

        assert(index-begin>0);
        _map.insert({index-1, {begin,0}});
        return index;
    }


    Intervals2(int maxIndex)
    {
        _map.insert({maxIndex, {0,0}});
    }

private:
    struct Interval
    {
        int begin;
        int occupied;
    };
    std::map<int, Interval> _map;
};




int main()
{
    for(int i=0; i<10000; ++i)
    {
        auto maxIndex = rand()%124;
        Intervals2 intervals2(maxIndex);
        Intervals1 intervals1(maxIndex);
        Intervals intervals0(maxIndex);
        for(int j=0; j<=6*maxIndex; ++j)
        {
            int index = rand()%(maxIndex+1);
            int k0 = intervals0.occupy(index);
            int k1 = intervals1.occupy(index);
            int k2 = intervals2.occupy(index);
            assert(k1==k0);
            assert(k2==k0);
        }
    }


    using Task = std::pair<int,int>;
    using Tasks = std::vector<Task>;
    const auto taskCmp = [](const Task& t1, const Task& t2)
    {
        return t1.second<t2.second;
    };
    using TasksQueue = std::priority_queue<Task,Tasks,decltype(taskCmp)>;

    int n; std::cin>>n;
    TasksQueue tasksQueue(taskCmp);
    int maxIndex = n-1;
    for(int i=0; i<n; ++i) {
        Task task;
        std::cin>>task.first>>task.second;
        task.first -= 1;
        maxIndex = std::max(maxIndex, task.first);
        tasksQueue.push(task);
    }
    Intervals intervals(maxIndex);

    long totalStress = 0;
    while (!tasksQueue.empty())
    {
        auto task = tasksQueue.top();
        tasksQueue.pop();
        int index = task.first;
        int found = intervals.occupy(index);
        if (found==-1)
            totalStress += task.second;
    }

    std::cout<<totalStress<<std::endl;

    return 0;
}