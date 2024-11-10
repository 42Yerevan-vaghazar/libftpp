#include "thread_safe_iostream.hpp"

thread_local ThreadSafeIOStream threadSafeCout;
// thread_local std::ostringstream ThreadSafeIOStream::_buffer;
// thread_local std::string ThreadSafeIOStream::threadLocalPrefix;

void ThreadSafeIOStream::setPrefix(const std::string& prefix) {
    const std::lock_guard<std::mutex> lock(_mutex);
    threadLocalPrefix = prefix;
}


ThreadSafeIOStream &ThreadSafeIOStream::operator<<(std::ostream& (*manip)(std::ostream&)) {
    const std::lock_guard<std::mutex> lock(_mutex);
    std::ostringstream buffer;

    buffer << threadLocalPrefix << _buffer.str() << manip;

    std::cout << buffer.str();
    _buffer.str("");
    _buffer.clear();
    return (*this);
}
