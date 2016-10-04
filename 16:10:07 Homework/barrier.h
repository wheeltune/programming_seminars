#include <mutex>   
#include <condition_variable> 
#include <iostream>
#include <thread>
#include <atomic>
#include <array>
#include <vector>

class barrier {   
private:
    std::mutex mtx_;   
    std::condition_variable ready_cond_; 
    
    int num_threads;

    std::atomic<bool> busy;
    std::atomic<int>  num;
public:   
    explicit barrier(size_t num_threads) : num_threads(num_threads)
    {
        num.store(0);
        busy.store(false);
    } 

    void enter() 
    {
        while (busy.load()) {
            std::this_thread::yield();
        }

        std::unique_lock<std::mutex> lock(mtx_);
        if (num.fetch_add(1) == num_threads - 1) {
            busy.store(true);
            ready_cond_.notify_all();
        } else {
            ready_cond_.wait(lock, [this]{return busy.load();});
        }
        if (num.fetch_sub(1) == 1) {
            busy.store(false);
        }
    }
};