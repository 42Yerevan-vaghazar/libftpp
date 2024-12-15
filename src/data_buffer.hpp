#pragma once
#include <iostream>
#include <queue>
#include <sstream>

class DataBuffer
{
public:

    // template <typename T>
    // void push(const T& p_object) {
    //     std::stringstream s(std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    //     s << p_object;

    //     _streams.push(std::move(s));
    // }

    // template <typename T>
    // void pop(T& p_object) {
    //     if (_streams.empty() == true) {
    //         throw std::runtime_error("There is no obj in buffer");
    //     }
    //     _streams.front() >> p_object;
    //     _streams.pop();
    // }
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
    std::stringstream _streams; // TODO is it requiered to have queue or stringstream handles all staff itself
};

