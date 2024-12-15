#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

class Message {
public:
    using Type = int;

    Message() = delete;
    Message(const Message&);
    explicit Message(Type type);
    ~Message();

    Type getType() const;

    std::string serialize() const;
    static Message deserialize(std::stringstream &ss);

    template <typename T>
    Message& operator<<(const T& obj) {
        const_cast<Message &>(*this)._sstream << obj;
        return *this;
    }

    template <typename T>
    const Message& operator>>(T& obj) const {
        const_cast<Message &>(*this)._sstream >> obj;
        return *this;
    }

private:
    Type _type;
    std::stringstream _sstream;
};
