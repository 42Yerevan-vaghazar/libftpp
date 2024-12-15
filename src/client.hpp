#pragma once
#include "message.hpp"
#include <functional>
#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <unordered_map>
#include <queue>

class Client
{
public:
    Client() : _messageSize(0) {} ;
    void connect(const std::string& address, const size_t& port);

    void disconnect();
    void defineAction(const Message::Type& messageType,
        const std::function<void(const Message& msg)>& action);
    void send(const Message& message);
    void update();

    struct ClientComponets
    {
        int sock = 0;
        struct sockaddr_in serv_addr;
    };
private:
    bool receiveMessage(std::string const &buffer)
    {
        _buffer.append(buffer.c_str(), buffer.size());
        while (true) {
            if (_messageSize == 0) {
                auto sizeEndIt = _buffer.find("\r\n");
                if (sizeEndIt != std::string::npos) {
                    _messageSize = std::stoi(_buffer);
                    _buffer.erase(0, (sizeEndIt + 2));
                }
            }
            if (_messageSize != 0 && _buffer.size() >= _messageSize) {
                std::string message = _buffer.substr(0, _messageSize);
                _buffer.erase(0, _messageSize);
                std::stringstream ss;
                ss << message;
                _messageQueue.push(new Message(Message::deserialize(ss)));
                this->_messageSize = 0;
                continue;
            }
            break;
        }
        return true; // Wait for more data
    }
    std::queue<Message *> *getMessages() {
        return (&_messageQueue);
    };
private:
    size_t _messageSize;
    std::string _buffer;
    std::queue<Message *> _messageQueue;
    ClientComponets _clientCompoments;
    std::unordered_map<Message::Type, std::function<void(const Message&)>> _actions;

};