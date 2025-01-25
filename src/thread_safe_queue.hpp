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
        std::lock_guard<std::mutex> lock(mutex);
        deque.push_back(newElement);
    }
    void push_front(const TType& newElement) {
        std::lock_guard<std::mutex> lock(mutex);
        deque.push_front(newElement);
    }
    TType pop_back() {
        std::lock_guard<std::mutex> lock(mutex);
        if (deque.empty()) {
            throw std::runtime_error("There is no element in queue");
        }
        TType tmp = deque.back();
        deque.pop_back();
        return (tmp);
    }
    TType pop_front() {
        std::lock_guard<std::mutex> lock(mutex);
        if (deque.empty()) {
            throw std::runtime_error("There is no element in queue");
        }
        TType tmp = deque.front();
        deque.pop_front();
        return (tmp);
    }
    bool empty() const {
        return (deque.empty());
    }
private:
    std::mutex mutex;
    std::deque<TType> deque;
};