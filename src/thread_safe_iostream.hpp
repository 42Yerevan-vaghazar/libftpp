#pragma once
#include <iostream>

class ThreadSafeIOStream
{
public:
    void setPrefix(const std::string& prefix) {_prefix = prefix;};
    std::string _prefix;
};

template <typename T>
ThreadSafeIOStream &operator<<(ThreadSafeIOStream &obj, const T &str) {
    std::cout << str;
    return (obj);
}


inline ThreadSafeIOStream &operator<<(ThreadSafeIOStream &obj, std::ostream& (*manip)(std::ostream&)) {
    std::cout << obj._prefix << manip;
    return (obj);
}