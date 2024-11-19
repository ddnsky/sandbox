#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <chrono>
#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <optional>

#include <cassert>


using Time = std::chrono::system_clock;


class Scheduler
{
public:
    using Job = std::function<void()>;
    using TimePoint = Time::time_point;

    void schedule(TimePoint time, Job const& job)
    {
        _sync.lock();
        _jobs.push({time, job});
        _sync.unlock();
        _cv.notify_one();
    }

    Scheduler()
     : _worker(threadFun, this) 
     {}
    ~Scheduler()
    {
        _done = true;
        _cv.notify_one();
        _worker.join();
    }

private:
    std::mutex _sync;
    std::condition_variable _cv;
    std::atomic<bool> _done;
    using Elem = std::pair<TimePoint, Job>;
    struct Compare {
        bool operator()(Elem const& e1, Elem const& e2) {
            return e1.first>e2.first;
        }
    };
    using Jobs = std::priority_queue<Elem,std::vector<Elem>, Compare>;
    Jobs _jobs;
    std::thread _worker;    

    static void threadFun(Scheduler* self)
    {   
        while(!self->_done)
        {
            Job job;
            {
                std::lock_guard lk(self->_sync);
                std::cout<<"threadFun cycle ... ";
                if (!self->_jobs.empty()) {
                    if (self->_jobs.top().first<=Time::now()+std::chrono::milliseconds(1)) {
                        job = std::move(self->_jobs.top().second);
                        self->_jobs.pop();
                    }
                } 
            }
            if (job) {
                std::cout<<"found job"<<std::endl;
                try { job(); }
                catch(...) {
                }
                continue;
            }

            if (!self->_done) {
                std::unique_lock lk(self->_sync);
                if (self->_jobs.empty()) {
                    std::cout<<"sleep empty"<<std::endl;
                    self->_cv.wait(lk);
                }
                else {
                    auto dur = self->_jobs.top().first-Time::now();
                    std::cout<<"sleep "<<std::chrono::duration_cast<std::chrono::milliseconds>(dur)<<std::endl;
                    self->_cv.wait_for(lk, dur);
                }
            }
        }
    }   
};

int main() 
{
    using namespace std::chrono_literals;
    std::atomic<int> count = 0;
    std::mutex m;
    std::condition_variable cv;
    //std::cout<<__cplusplus<<std::endl;
    auto start = std::chrono::system_clock::now();

    const auto job = [&](){
        m.lock();
        count++;
        std::cout << "Job "<< count << " ";
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count();
        std::cout << std::endl;
        m.unlock();
        cv.notify_one();
    };

    {
        Scheduler scheduler;
        const int totalCount = 6;
        for(int i=0; i<totalCount; ++i)
        {
            scheduler.schedule(std::chrono::system_clock::now()+1ms+i*21ms, job);
        }

        {
            std::unique_lock lk(m);
            cv.wait(lk,[&](){ return count==totalCount; });
        }

        assert(count==totalCount);
    }

	return 0;
}