#pragma once
#include <iostream>
#include <queue>
#include <sstream>

class DataBuffer
{
public:

    template <typename T>
    void push(const T& p_object) {
        std::stringstream s;
        s << p_object;

        _streams.push(std::move(s));
    }

    template <typename T>
    void pop(T& p_object) {
        if (_streams.empty() == true) {
            throw std::runtime_error("There is no obj in buffer");
        }
        _streams.front() >> p_object;
        _streams.pop();
    }
private:
    std::queue<std::stringstream> _streams;
};

template <typename T>
DataBuffer& operator<<(DataBuffer& p_buffer, const T& p_object) {
    p_buffer.push(p_object);
    return (p_buffer);
}

template <typename T>
DataBuffer& operator>>(DataBuffer& p_buffer, T& p_object) {
    p_buffer.pop(p_object);
    return (p_buffer);
}