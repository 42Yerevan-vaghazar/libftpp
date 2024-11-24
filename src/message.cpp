
#include "message.hpp"
#include <iostream>

// Message::Message() : _type() {
//     std::cout << "Message created with type " << _type << std::endl;
// }

Message::Message(Type type) :
        _type(type),
        _sstream(std::ios_base::in | std::ios_base::out | std::ios_base::binary) {
    std::cout << "Message created with type " << _type << std::endl;
}

Message::~Message() {
    std::cout << "Message of type " << _type << " destroyed" << std::endl;
}

Message::Type Message::getType() const {
    return _type;
}
