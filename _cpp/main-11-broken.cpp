#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <optional>
#include <map>
#include <numeric>
#include <fstream>

template <size_t N, typename Less>
struct Heap
{
    int _buf[N];
    int _idx[N];
    int _size;
    Less _less;

    Heap(Less const &less)
        : _size(0), _less(less) {}

    void onChange(int index, int value)
    {
        assert(0 <= value && value < _size);
        _idx[value] = index;
    };

    void check()
    {
        for (int i = 0; i < _size; ++i)
        {
            int ch1 = (i << 1) + 1;
            assert(ch1 >= _size || !_less(_buf[ch1], _buf[i]));
            int ch2 = ch1 + 1;
            assert(ch2 >= _size || !_less(_buf[ch2], _buf[i]));
            assert(i == 0 || !_less(_buf[i], _buf[(i - 1) >> 1]));
        }
        for (int j = 0; j < _size; ++j)
            assert(_buf[_idx[j]] == j);
    }
    bool pullup(int index)
    {
        auto parent = (index - 1) >> 1;
        auto _value = _buf[index];
        bool isChanged = false;
        while (index > 0 && _less(_value, _buf[parent]))
        {
            _buf[index] = _buf[parent];
            onChange(index, _buf[index]);
            index = parent;
            parent = (index - 1) >> 1;
            isChanged = true;
        }
        if (isChanged)
        {
            _buf[index] = _value;
            onChange(index, _buf[index]);
        }
        return isChanged;
    }
    void pulldown(int index)
    {
        assert(index < _size);
        auto _value = _buf[index];
        bool isChanged = false;
        while (index < _size)
        {
            auto c1 = (index << 1) + 1;
            if (c1 >= _size)
                break;
            auto c2 = c1 + 1;
            if (c2 < _size && _less(_buf[c2], _buf[c1]))
                c1 = c2;
            if (!_less(_buf[c1], _value))
                break;
            _buf[index] = _buf[c1];
            onChange(index, _buf[index]);
            index = c1;
            isChanged = true;
        }
        if (isChanged)
        {
            _buf[index] = _value;
            onChange(index, _value);
        }
    };
    void update(int idx)
    {
        int index = _idx[idx];
        if (!pullup(index))
        {
            pulldown(index);
        }
        check();
    }
    void push(int value)
    {
        int index = _size++;
        _buf[index] = value;
        onChange(index, value);
        pullup(index);
        check();
    }
    int top() const
    {
        return _buf[0];
    }
    void resize(int K)
    {
        assert(K <= N);
        _size = 0;
        for (int i = 0; i < K; ++i)
        {
            push(i);
        }
        check();
    }
};

struct HeapMinFinder
{
    const int *_nums;
    const int K;
    Heap<10000,std::function<bool(int,int)>> _heap;

    HeapMinFinder(const int *begin, const int *end)
        : _nums(begin)
        , K(end-begin)
        , _heap([this](int v1, int v2)->bool{ 
            assert(0<=v1 && v1<K);
            assert(0<=v2 && v2<K);
            return _nums[v1]<_nums[v2];
            })
    {
        _heap.resize(K);
    }
    int top()
    {
        return _nums[_heap.top()];
    }
    void update(int index, int oldValue, int newValue)
    {
        if (oldValue!=newValue)
        {
            _heap.update(index);
        }
    }
};


struct SimpleMinFinder
{
    const int *_begin;
    const int *_end;
    std::optional<int> _mine;

    SimpleMinFinder(const int *begin, const int *end)
        : _begin(begin), _end(end)
    {
    }
    int top()
    {
        if (!_mine.has_value())
        {
            update(0,0,0);
        }
        return *_mine;
    }
    void update(int index, int oldValue, int newValue)
    {
        if (_mine.has_value() && oldValue != *_mine)
        {
            _mine = std::min(newValue, _mine.value());
        }
        else
        {
            auto found = std::min_element(_begin, _end);
            if (found!=_end)
            {
                _mine = *found;
            }
        }
    }
};

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
    void update(int index, int oldValue, int newValue)
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


struct TwoHeapMinFinder
{
    const int *_nums;
    const int _K;
    std::vector<int> _heap1;
    std::vector<int> _heap2;

    TwoHeapMinFinder(const int *begin, const int *end)
        : _nums(begin)
        , _K(end-begin)
    {
        _heap1.resize(_K);
        _heap2.reserve(_K);

        std::iota(_heap1.begin(), _heap1.end(), 0);
        std::make_heap(_heap1.begin(), _heap1.end(), [this](int v1, int v2){ return _nums[v1]>_nums[v2]; });
    }
    int top()
    {
        assert(!_heap1.empty());
        return _heap2.empty()? _nums[_heap1[0]] : std::min(_nums[_heap1[0]], _nums[_heap2[0]]);
    }
    void update(int index, int oldValue, int newValue)
    {
        const auto cmp = [this](int v1, int v2){ return _nums[v1]>_nums[v2]; };
        if (index!=0)
        {
            assert(_heap1.size()==index);
            _heap1.push_back(index);
            assert(_heap1.size()<=_heap1.capacity());
            std::push_heap(_heap1.begin(), _heap1.end(), cmp);
        }
        else
        {
            std::swap(_heap1, _heap2);
            _heap1.resize(1);
            _heap1[0]=0;
        }
        while (!_heap2.empty() && (_heap2[0]<=index /*|| _heap2.back()<=index*/))
        {
            //while (!_heap2.empty() && _heap2.back()<=index)
                //heap2.pop_back();    
            while (!_heap2.empty() && _heap2[0]<=index)
            {
                std::pop_heap(_heap2.begin(), _heap2.end(), cmp);
                _heap2.pop_back();
            }
        }
    }
};


template<typename InputIter, typename OutputIter>
void solve(int N, int K, InputIter inp, OutputIter out)
{
    int window[10000];
    assert(K<10000);

    std::copy_n(inp, K, window);
    //inp += (K-1);

    //MapMinFinder mf(window, window + K);
    TwoHeapMinFinder mf(window, window + K);
    SimpleMinFinder smf(window, window + K);

    auto min2 = mf.top();
    assert(mf.top() == smf.top());
    *out++ = mf.top();

    for (int j = K, w = 0; j < N; ++j, ++w)
    {
        if (w >= K)
            w -= K;
        auto oldValue = window[w];
        auto newValue = window[w] = *++inp;

        mf.update(w, oldValue, newValue);
        smf.update(w, oldValue, newValue);

        assert(mf.top() == smf.top());

        *out++ = mf.top();
    }
}

struct RandomIntIterator
{
    using iterator_category = std::input_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = const int;
    using pointer           = const int*;
    using reference         = const int&;
    
    RandomIntIterator& operator++()
    {
        return *this;
    }
    RandomIntIterator operator++(int)
    {
        return *this;
    }
    int operator*() const
    {
        return rand() % 16;
    }
};

struct NullOutputIterator {
    using iterator_category = std::output_iterator_tag;
    template <typename T>
    void operator=(T const&) {}
    NullOutputIterator& operator++() { return *this;}
    NullOutputIterator operator++(int) { return *this;}
    NullOutputIterator& operator*() { return *this; }
};        

int main()
{
    int N, K, window[10000];

    for (int I=0; I<1; ++I) {
        N = 17;
        K = 6;
        int nums[4092];
        std::iota(nums, nums+N, 0);
        solve(N, K, nums, std::ostream_iterator<int>(std::cout, "\n"));

        std::iota(std::reverse_iterator(nums+N), std::reverse_iterator(nums), 0);
        solve(N, K, nums, std::ostream_iterator<int>(std::cout, "\n"));

        std::copy_n(RandomIntIterator(), N, nums);
        solve(N, K, nums, std::ostream_iterator<int>(std::cout, "\n"));

        std::cout<<std::endl;

        for (int i=0; i<500; ++i) {
            //N = 15 + rand() % 100;
            //K = 2 + rand() % (N / 5);
            //N = 10 + rand() % 10;
            //K = 2 + rand() % (N / 2);
            N = 9 + rand() % 10;
            K = 1 + rand() % (N / 2);

            //solve(N, K, RandomIntIterator(), NullOutputIterator());
            //solve(N, K, RandomIntIterator(), std::ostream_iterator<int>(std::cout, "\n"));
            std::copy_n(RandomIntIterator(), N, nums);
            if (i==175)
                std::copy_n(nums, N, std::ostream_iterator<int>(std::cout, "\n"));    
            solve(N, K, nums, std::ostream_iterator<int>(std::cout, "\n"));
        }

    }


    std::cin >> N >> K;
    std::istream_iterator<int> inp(std::cin);

    std::ostream_iterator<int> out(std::cout, "\n");
    solve(N, K, inp, out);

    return 0;
}