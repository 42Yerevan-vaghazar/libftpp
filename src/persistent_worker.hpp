#pragma once
#pragma once
#include <thread>
#include <iostream>
#include "thread_safe_queue.hpp"
#include <unordered_map>

class PersistentWorker
{
public:

    PersistentWorker(const PersistentWorker&) = delete;
    PersistentWorker& operator =(const PersistentWorker&) = delete;
    PersistentWorker() = default;

    ~PersistentWorker();

    void addTask(const std::string& name, const
        std::function<void()>& jobToExecute);

    void removeTask(const std::string& name);
private:
    std::mutex _guard;
    std::unordered_map<std::string, std::thread> _setOfTasks;
    std::unordered_map<std::string, bool> _setOfFinish;
};