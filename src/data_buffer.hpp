#pragma once
#include <iostream>
#include <queue>
#include <sstream>

class DataBuffer
{
public:
    template <typename T>
    DataBuffer& operator<<(const T& p_object) {
        _streams << p_object << " ";
        return (*this);
    }
    template <typename T>
    DataBuffer& operator>>(T& p_object) {
        if (_streams.eof() == true) {
            throw std::runtime_error("stream is empty");
        }

        _streams >> p_object;
        return (*this);
    }
private:
    std::stringstream _streams;
};

