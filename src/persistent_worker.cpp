#include "persistent_worker.hpp"
PersistentWorker::~PersistentWorker() {
    for(auto &[name, a] : _setOfFinish) {
        a = true;
    }

    for(auto &[name, task] : _setOfTasks) {
        if (task.joinable())
            task.join();
    }
};

void PersistentWorker::addTask(const std::string& name, const
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

void PersistentWorker::removeTask(const std::string& name) {
    {
        std::lock_guard<std::mutex> lock(_guard);
        _setOfFinish[name] = true;
    }
    auto &task = _setOfTasks[name];

    if (task.joinable())
        task.join();

    _setOfTasks.erase(name);
}