#pragma once
#include <iostream>
#include "data_buffer.hpp"

class Message
{
public:
    template <typename T>
    friend const Message& operator<<(const Message& message, const T& p_object);
    template <typename T>
    friend const Message& operator>>(const Message& message, T& p_object);
    using Type = int;
    Message() = delete;
    Message(const Message&) = delete;
    Message(Type type);
    ~Message();

    Type getType() const;
private:
    int _type;
    std::stringstream _sstream;
};

template <typename T>
const Message& operator<<(const Message& message, const T& obj) {
    const_cast<Message&>(message)._sstream << obj;

    return (message);
}

template <typename T>
const Message& operator>>(const Message& message, T& obj) {

    const_cast<Message&>(message)._sstream >> obj;
    return (message);
}