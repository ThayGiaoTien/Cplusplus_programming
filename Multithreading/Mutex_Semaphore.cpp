#include <iostream>
#include <thread>
#include <vector>
#include<mutex>
#include <semaphore>
#include<deque>
//#include<boost/thread.hpp> 

std::counting_semaphore<10> emptyCount(10);
std::counting_semaphore<10> fillCount(0);
std::mutex mtx;
std::deque<int> buffer;
int produceIndex = 0;
int consumeIndex = 0;

// Writes 300 element to deque. 
void producer() {
    for (int i = 0; i < 300; i++) {
        emptyCount.acquire();   // decrese
        mtx.lock();
        buffer.push_back(i);
        std::cout << "Producer:" << i << std::endl;
        mtx.unlock();
        fillCount.release(); // increase
        
    }
}

// First consumer take 100 first elements of queue
void consumer() {
    for (int i = 0; i < 100; i++) {
        fillCount.acquire(); // decrese fillCount semaphore or block if it =0;
        mtx.lock();
        int item = buffer.front();
        buffer.pop_front();
        std::cout << "Consumed: " << item << std::endl;
        mtx.unlock();
        emptyCount.release(); // increse the empty count. 
       
    }
}
// Second consumer take 100 next elements of queue. 
void consumer1() {
    for (int i = 0; i < 100; i++) {
        fillCount.acquire(); // decrese fillCount semaphore or block if it =0;
        mtx.lock();
        int item = buffer.front();
        buffer.pop_front();
        std::cout << "Consumed1: " << item << std::endl;
        mtx.unlock();
        emptyCount.release(); // increse the empty count. 

    }
}
// Second consumer take 100 last elements of queue. 
void consumer2() {
    for (int i = 0; i < 100; i++) {
        fillCount.acquire(); // decrese fillCount semaphore or block if it =0;
        mtx.lock();
        int item = buffer.front();
        buffer.pop_front();
        std::cout << "Consumed2: " << item << std::endl;
        mtx.unlock();
        emptyCount.release(); // increse the empty count. 

    }
}

// By combining 3 function into single function, now they will be handle in single thread, so they will following order.
void combineFunc()
{
    consumer();
    consumer1();
    consumer2();
}
int main() {
    std::thread prodThread(producer);
    std::thread consThread(combineFunc);
    //std::thread consThread1(consumer1);

    prodThread.join();
    consThread.join();
    
    //consThread1.join();

    return 0;
}

