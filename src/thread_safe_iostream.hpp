#pragma once
#include <iostream>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <sstream>


class ThreadSafeIOStream
{
public:
    void setPrefix(const std::string& prefix);

    template <typename T>
    ThreadSafeIOStream &operator<<(const T &obj) {
        _buffer << obj;
        return (*this);
    }

    ThreadSafeIOStream &operator<<(std::ostream& (*manip)(std::ostream&));

    template <typename T>
    ThreadSafeIOStream &operator>>(T &obj) {
        const std::lock_guard<std::mutex> lock(_mutex);

        std::cin >> obj;
        return (*this);
    }

private:

    std::ostringstream _buffer;
    std::string threadLocalPrefix;
    std::mutex _mutex;
};

extern thread_local ThreadSafeIOStream threadSafeCout; 