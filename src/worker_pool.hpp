#pragma once
#include <thread>
#include <vector>
#include <iostream>
#include "thread_safe_queue.hpp"

class IJobs
{
    public:
        virtual void addJob(const std::function<void()>& jobToExecute) = 0;
        virtual ~IJobs() {};
};

class WorkerPool : public IJobs
{
public:
    WorkerPool(const WorkerPool&) = delete;
    WorkerPool& operator=(const WorkerPool&) = delete;

    WorkerPool(int n);

    ~WorkerPool();

    void addJob(const std::function<void()>& jobToExecute) override;

    void _start_thread();

private:
    size_t _numThreads;
    bool _stopExecuting;
    std::vector<std::thread> _workers;
    ThreadSafeQueue<std::function<void()>> _queue;
    std::mutex _queueMutex;
    std::mutex _guard;
    std::condition_variable _condition;
};