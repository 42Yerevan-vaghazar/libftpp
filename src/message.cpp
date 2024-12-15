
#include "message.hpp"
#include <iostream>

Message::Message(Type type)
    : _type(type),
      _sstream(std::ios_base::in | std::ios_base::out | std::ios_base::binary) {
    std::cout << "Message created with type " << _type << std::endl;
}

Message::Message(const Message& other)
    : _type(other._type),
      _sstream(std::ios_base::in | std::ios_base::out | std::ios_base::binary) {
    _sstream << other._sstream.rdbuf();
}

Message::~Message() {
    std::cout << "Message of type " << _type << " destroyed" << std::endl;
}

Message::Type Message::getType() const {
    return _type;
}

std::string Message::serialize() const {
    std::ostringstream oss;

    oss << _type << " ";
    oss << _sstream.rdbuf();
    return oss.str();
}

Message Message::deserialize(std::stringstream &ss) {
    Type type;
    ss >> type;

    Message message(type);
    std::string str;
    message._sstream << ss.rdbuf();
    return message;
}