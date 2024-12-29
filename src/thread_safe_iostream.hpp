#pragma once
#include <iostream>
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
        std::cout << threadLocalPrefix;
        
        std::cin >> obj;
        return (*this);
    }

    template<typename T>
    void prompt(const std::string& question, T& dest) {
        const std::lock_guard<std::mutex> lock(_mutex);
        std::cout << threadLocalPrefix;

        std::cout << question;
        std::cin >> dest;
    };

private:

    std::ostringstream _buffer;
    std::string threadLocalPrefix;
    static std::mutex _mutex;
};

extern thread_local ThreadSafeIOStream threadSafeCout; 