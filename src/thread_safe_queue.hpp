#pragma once
#include <deque>
#include <thread>
#include <mutex>

template<typename TType>
class ThreadSafeQueue
{
public:
    void push_back(const TType& newElement)
    {
        mutex.lock();
        deque.push_back(newElement);
        mutex.unlock();
    }
    void push_front(const TType& newElement) {
        mutex.lock();
        deque.push_front(newElement);
        mutex.unlock();
    }
    TType pop_back() {
        mutex.lock();
        if (deque.empty()) {
            mutex.unlock();
            throw std::runtime_error("There is no element in queue");
        }
        TType tmp = deque.back();
        deque.pop_back();
        mutex.unlock();
        return (tmp);
    }
    TType pop_front() {
        mutex.lock();
        if (deque.empty()) {
            mutex.unlock();
            throw std::runtime_error("There is no element in queue");
        }
        TType tmp = deque.front();
        deque.pop_front();
        mutex.unlock();
        return (tmp);
    }
private:
    std::mutex mutex;
    std::deque<TType> deque;
};