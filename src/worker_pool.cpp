#include "worker_pool.hpp"


WorkerPool::WorkerPool(int n) : _stopExecuting(false) {
    _workers.reserve(n);
    for (size_t i = 0; i < static_cast<size_t>(n); ++i) {
        _workers.emplace_back(&WorkerPool::_start_thread, this);
    }
};

WorkerPool::~WorkerPool() {
    {
        std::lock_guard<std::mutex> lock(_guard);
        _stopExecuting = true;
    }
    _condition.notify_all();
    for (auto& worker : _workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void WorkerPool::addJob(const std::function<void()>& jobToExecute) {
    _queue.push_back(jobToExecute);
    _condition.notify_one();
};

void WorkerPool::_start_thread() {
    while (true) {
        std::function<void()> job;
        std::unique_lock<std::mutex> lock(_queueMutex);

        _condition.wait(lock, [&]() { return (_stopExecuting || _queue.empty() == false);});
        {
            std::lock_guard<std::mutex> lock(_guard);
            if (_stopExecuting == true) {
                return ;
            }
        }
        job = _queue.pop_front();
        try
        {
            job();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
};