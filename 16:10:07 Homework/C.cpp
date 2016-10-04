#include <mutex>   
#include <condition_variable>   
  
class blocking_flag {   
public:   
    blocking_flag()   
        : ready_(false)   
    {}   
  
    void wait() {   
        std::unique_lock<std::mutex> lock(mtx_);   
        while (!ready_.load()) {   
            ready_cond_.wait(lock);   
        }   
    }   
  
    void set() {   
        ready_.store(true);   
        ready_cond_.notify_all();   
    }   
  
private:   
    std::atomic<bool> ready_;   
    std::mutex mtx_;   
    std::condition_variable ready_cond_;   
};

#include <thread>   
#include <iostream>   
  
int main() {   
    blocking_flag f;   
  
    std::thread t(   
        [&f]() {   
            f.wait();   
            std::cout << "ready!" << std::endl;   
        }   
    );
    std::thread t2(   
        [&f]() {   
            f.wait();   
            std::cout << "ready!" << std::endl;   
        }   
    );  
    std::thread t3(   
        [&f]() {   
            f.wait();   
            std::cout << "ready!" << std::endl;   
        }   
    );  
    std::thread t4(   
        [&f]() {   
            f.wait();   
            std::cout << "ready!" << std::endl;   
        }   
    );     
  
    f.set();   
    t.join();  
    t2.join();
    t3.join();
    t4.join(); 
  
    return 0;   
}