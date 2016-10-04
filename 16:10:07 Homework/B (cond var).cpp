// РЕАЛИЗАЦИЯ ПРИ ПОМОЩИ УСЛОВНЫХ ПЕРЕМЕННЫХ

#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>

std::mutex mtx;                                                                 
std::condition_variable sync_cond; 
std::atomic<bool> centred;                                                // переменная, обозначающая находится ли робот в центре в данный момент 
                                                                          // (бывает 2 положения false - смещен влево, true - никуда не смещен)


int main() 
{
    std::unique_lock<std::mutex> lock(mtx);

    std::thread left_t (   
        [&lock]() {   
            while (1) {
                sync_cond.wait (lock, [] {return centred.load(); });       // ждем если он уже смещен 
                std::cout << "1: step(\"left\")\n";                        // исполняем нужную функцию
                centred.store (false);                                     // сдвигаем робота
                sync_cond.notify_one();                                    // оповещаем о сдвиге
            }  
        }   
    );  

    std::thread right_t (   
        [&lock]() {   
            while (1) {
                sync_cond.wait (lock, [] {return !centred.load(); });      // ждем если он в центре
                std::cout << "2: step(\"right\")\n";                       // исполняем нужную функцию
                centred.store (true);                                      // сдвигаем робота
                sync_cond.notify_one();                                    // оповещаем о сдвиге
            }  
        }   
    );  

    left_t.join();
    right_t.join();

    return 0;
}