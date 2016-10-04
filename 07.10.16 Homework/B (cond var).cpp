#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

std::mutex mtx;   
std::condition_variable sync_cond; 
std::atomic<bool> centred;

int main() 
{
    std::unique_lock<std::mutex> lock(mtx);

    std::thread left_t (   
        [&lock]() {   
            while (1) {
                sync_cond.wait (lock, [] {return centred.load(); });
                std::cout << "1: step(\"left\")\n";
                centred.store (false);
                sync_cond.notify_one();
            }  
        }   
    );  

    std::thread right_t (   
        [&lock]() {   
            while (1) {
                sync_cond.wait (lock, [] {return !centred.load(); });
                std::cout << "2: step(\"right\")\n";
                centred.store (true);
                sync_cond.notify_one();
            }  
        }   
    );  

    left_t.join();
    right_t.join();

    return 0;
}