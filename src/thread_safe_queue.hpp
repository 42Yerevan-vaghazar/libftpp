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
            throw std::runtime_error("There is no element in queue");
        }
        TType tmp = deque.back();
        mutex.unlock();
        return (deque.pop_back(), tmp);
    }
    TType pop_front() {
        mutex.lock();
        if (deque.empty()) {
            throw std::runtime_error("There is no element in queue");
        }
        TType tmp = deque.front();
        mutex.unlock();
        return (deque.pop_front(), tmp);
    }
private:
    std::mutex mutex;
    std::deque<TType> deque;
};