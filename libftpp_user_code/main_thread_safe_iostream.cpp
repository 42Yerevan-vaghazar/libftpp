// #include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "thread_safe_iostream.hpp"

// // ThreadSafeIOStream threadSafeCout;

void printNumbers(const std::string& p_prefix) {
    threadSafeCout.setPrefix(p_prefix);
    
    for (int i = 1; i <= 5; ++i) {
        threadSafeCout << "Number: " << i << std::endl;
    }
    threadSafeCout << std::endl;
}

void inputNumbers(const std::string& p_prefix, std::vector<int> &vec) {
    threadSafeCout.setPrefix(p_prefix);
    for (int i = 1; i <= 2; ++i) {
        int a;
        
        threadSafeCout >> a;
        vec.push_back(a);
    }
}

int main() {
    using namespace std::chrono_literals;
    std::string prefix1 = "[Thread 1] ";
    std::string prefix2 = "[Thread 2] ";

    // std::thread thread1(printNumbers, prefix1);
    // std::thread thread2(printNumbers, prefix2);
    std::vector<int>vec1;
    std::vector<int>vec2;
    std::thread thread3(inputNumbers, prefix1, std::ref(vec1));
    std::thread thread4(inputNumbers, prefix2, std::ref(vec2));

    // thread1.join();
    // thread2.join();
    thread3.join();
    thread4.join();
    std::cout << "vec1" << std::endl;
    for (auto const &a : vec1) {
        std::cout << a << ", ";
    }
    std::cout << std::endl;
    std::cout << "vec2" << std::endl;
    for (auto const &a : vec2) {
        std::cout << a << ", ";
    }
    std::cout << std::endl;
    // std::cout << "mek";
    // std::this_thread::sleep_for(2000ms);
    return 0;
}

// #include "thread_safe_iostream.hpp"
// #include <thread>

// void threadFunction(int threadId) {
//     threadSafeCout.setPrefix("[Thread " + std::to_string(threadId) + "] ");
//     threadSafeCout << "Hello from thread " << threadId << std::endl;
// }

// int main() {
//     std::thread t1(threadFunction, 1);
//     std::thread t2(threadFunction, 2);
    
//     t1.join();
//     t2.join();
    
//     return 0;
// }
