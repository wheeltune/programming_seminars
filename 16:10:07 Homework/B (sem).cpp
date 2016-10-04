// РЕАЛИЗАЦИЯ ПРИ ПОМОЩИ СЕМАФОРОВ

#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include "semaphore.h"

Semaphore left_sem, right_sem;       // бывает 3 положения (1, 0) - следующим будет движение влево (0, 1) - вправо (0, 0) - исполняется

int main() 
{
    left_sem.post();                 // ставим в позицию (1, 0) чтобы первым было движение влево
    std::thread left_t (   
        []() {   
            while (1) {
                left_sem.wait ();                       // (0, 0) для исполнения
                std::cout << "1: step(\"left\")\n";     // исполняем нужную функцию
                right_sem.post ();                      // (0, 1) следующее движение вправо 
            }  
        }   
    );  

    std::thread right_t (   
        []() {   
            while (1) {
                right_sem.wait ();                       // (0, 0) для исполнения
                std::cout << "2: step(\"right\")\n";     // исполняем нужную функцию
                left_sem.post ();                        // (1, 0) следующее движение влево
            }  
        }   
    );  

    left_t.join();
    right_t.join();

    return 0;
}