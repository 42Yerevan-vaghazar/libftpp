#include "thread_safe_iostream.hpp"

thread_local ThreadSafeIOStream threadSafeCout;
// thread_local std::ostringstream ThreadSafeIOStream::_buffer;
// thread_local std::string ThreadSafeIOStream::threadLocalPrefix;

void ThreadSafeIOStream::setPrefix(const std::string& prefix) {
    const std::lock_guard<std::mutex> lock(_mutex);
    threadLocalPrefix = prefix;
}

