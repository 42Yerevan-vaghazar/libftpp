#pragma once
#pragma once
#include <thread>
#include <iostream>
#include "thread_safe_queue.hpp"
#include <unordered_map>

class PersistentWorker
{
public:
    PersistentWorker() {};

    ~PersistentWorker() {
        for(auto &[name, a] : _setOfFinish) {
            a = true;
        }

        for(auto &[name, task] : _setOfTasks) {
            if (task.joinable())
                task.join();
        }
    };

    void addTask(const std::string& name, const
        std::function<void()>& jobToExecute) {
        _setOfTasks.insert({name, std::thread([&]() {
                while (true) {
                    bool is_finished;
                    {
                        std::lock_guard<std::mutex> lock(_guard);
                        is_finished = _setOfFinish[name];
                    }
                    if (is_finished) {
                        return ;
                    }
                    jobToExecute();
                }
            })});
        {
            std::lock_guard<std::mutex> lock(_guard);
            _setOfFinish.insert({name, false});
        }
    };

    void removeTask(const std::string& name) {
        {
            std::lock_guard<std::mutex> lock(_guard);
            _setOfFinish[name] = true;
        }
            auto &task = _setOfTasks[name];

            if (task.joinable())
                task.join();

            _setOfTasks.erase(name);
    }
private:
    void start_task() {

    }
private:
    std::mutex _guard;
    std::unordered_map<std::string, std::thread> _setOfTasks;
    std::unordered_map<std::string, bool> _setOfFinish;
};