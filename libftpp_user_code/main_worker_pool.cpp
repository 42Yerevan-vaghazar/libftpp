#include "worker_pool.hpp"
#include <iostream>
#include "thread_safe_iostream.hpp"

// #pragma once
// #include <thread>
// #include <vector>
// #include <queue>
// #include <unordered_map>
// #include <iostream>
// #include <functional>
// #include <mutex>
// #include <condition_variable>

// class IJobs {
// public:
//     virtual void addJob(const std::function<void()>& jobToExecute) = 0;
//     virtual ~IJobs() = default;
// };

// class WorkerPool : public IJobs {
// public:
//     WorkerPool(int n) : _n(n), _isStopping(false) {
//         for (size_t i = 0; i < _n; ++i) {
//             _workers.emplace_back(&WorkerPool::_workerThread, this);
//         }
//     }

//     ~WorkerPool() {
//         // Graceful shutdown
//         {
//             std::unique_lock<std::mutex> lock(_queueMutex);
//             _isStopping = true;
//         }
//         _condition.notify_all();
//         for (auto& worker : _workers) {
//             if (worker.joinable()) {
//                 worker.join();
//             }
//         }
//     }

//     void addJob(const std::function<void()>& jobToExecute) override {
//         {
//             std::unique_lock<std::mutex> lock(_queueMutex);
//             _queue.push(jobToExecute);
//         }
//         _condition.notify_one(); // Notify one waiting thread
//     }

// private:
//     void _workerThread() {
//         while (true) {
//             std::function<void()> job;
//             {
//                 std::unique_lock<std::mutex> lock(_queueMutex);
//                 _condition.wait(lock, [this] {
//                     return !_queue.empty() || _isStopping;
//                 });

//                 if (_isStopping && _queue.empty()) {
//                     break; // Exit the loop if stopping and no jobs are left
//                 }

//                 job = _queue.front();
//                 _queue.pop();
//             }
//             try {
//                 job(); // Execute the job
//             } catch (const std::exception& e) {
//                 std::cerr << "Job execution error: " << e.what() << '\n';
//             }
//         }
//     }

//     const size_t _n;
//     bool _isStopping;
//     std::vector<std::thread> _workers;
//     std::queue<std::function<void()>> _queue;
//     std::mutex _queueMutex;
//     std::condition_variable _condition;
// };


int main() {
    WorkerPool pool(4);

    auto job = []() {
        threadSafeCout << "Executing job on thread: " << std::this_thread::get_id() << std::endl;
    };

    for (int i = 0; i < 1000; ++i) {
        pool.addJob(job);
    }

    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for jobs to finish

    return 0;
}


// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <condition_variable>

// std::mutex mtx;
// std::condition_variable cv;
// bool ready = false;

// void worker_thread() {
//     std::unique_lock<std::mutex> lock(mtx); // Lock mutex
//     cv.wait(lock, [] { return ready; });    // Wait until `ready` is true
//     std::cout << "Worker thread processing.\n";
// }

// void signal_thread() {
//     {
//         std::lock_guard<std::mutex> lock(mtx); // Lock mutex
//         ready = true;                          // Update condition
//     }
//     // cv.notify_one(); // Notify one waiting thread
// }

// int main() {
//     std::thread t1(worker_thread);
//     std::thread t2(signal_thread);

//     t1.join();
//     t2.join();
//     return 0;
// }
