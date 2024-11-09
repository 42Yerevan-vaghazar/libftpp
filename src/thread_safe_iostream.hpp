#pragma once
#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>

class ThreadSafeIOStream
{
public:
    ThreadSafeIOStream() : _isPrefixPrinted(false){}
    template <typename T>
    friend ThreadSafeIOStream &operator<<(ThreadSafeIOStream &obj, const T &str) ;
    friend inline ThreadSafeIOStream &operator<<(ThreadSafeIOStream &obj, std::ostream& (*manip)(std::ostream&));  
    void setPrefix(const std::string& prefix) {
        _prefixs[std::this_thread::get_id()] = {prefix, false};
    }
private:
    struct thread_info
    {
        std::string prefix;
        bool isPrefixPrinted;
    };

    std::unordered_map<std::thread::id, thread_info> _prefixs;
    bool _isPrefixPrinted;
    std::mutex _mutex;
};

template <typename T>
ThreadSafeIOStream &operator<<(ThreadSafeIOStream &obj, const T &str) {
    auto &[prefix, _isPrefixPrinted] = obj._prefixs[std::this_thread::get_id()];

    if (_isPrefixPrinted == false) {
        obj._mutex.lock();
        _isPrefixPrinted = true;
        std::cout << prefix;
    }
    std::cout << str;
    return (obj);
}
             

inline ThreadSafeIOStream &operator<<(ThreadSafeIOStream &obj, std::ostream& (*manip)(std::ostream&)) {
    auto &[prefix, _isPrefixPrinted] = obj._prefixs[std::this_thread::get_id()];
    if (_isPrefixPrinted == false) {
        _isPrefixPrinted = true;
        std::cout << prefix;
    }
    std::cout << manip;
    _isPrefixPrinted = false;
    obj._mutex.unlock();
    return (obj);
}