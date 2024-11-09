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
    ThreadSafeIOStream &operator<<(const T &str) {
        _buffer << str;
        return (*this);
    }

    ThreadSafeIOStream &operator<<(std::ostream& (*manip)(std::ostream&)) {
    const std::lock_guard<std::mutex> lock(_mutex);

    std::cout << threadLocalPrefix << _buffer.str() << manip << std::flush;
    _buffer.str("");
    _buffer.clear();
    return (*this);
}
private:

    std::ostringstream _buffer;
    std::string threadLocalPrefix;
    std::mutex _mutex;
};

extern thread_local ThreadSafeIOStream threadSafeCout; 