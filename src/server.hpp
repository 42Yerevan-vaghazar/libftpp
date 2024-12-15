#pragma once
#include "message.hpp"
#include <functional>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class Server
{
public:
    void start(const size_t& p_port);

    void defineAction(const Message::Type& messageType,
        const std::function<void(long long& clientID, const Message& msg)>& action);
    void sendTo(const Message& message, long long clientID);

    void sendToArray(const Message& message, std::vector<long long> clientIDs);

    void sendToAll(const Message& message);

    void update();
    
    struct ServerComponents
    {
        int server_fd;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
    };

    class Client
    {
    public:
        Client(Client const &) = delete;
        Client& operator=(Client const &) = delete;
        Client(int clientSocket) 
            :   _messageSize(0),
                _socket_fd(clientSocket) {}

        ~Client()
        {
            if (_socket_fd != -1)
            {
                close(_socket_fd);
            }
        }

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

        int getSocket() const { return _socket_fd; }
        std::queue<Message *> *getMessages() {
            return (&_messageQueue);
        };
    private:
        // bool isMessageReady;
        size_t _messageSize;
        int _socket_fd;
        std::string _buffer;
        std::queue<Message *> _messageQueue;
    };

private:
    std::unordered_map<long long, Client *> _clientSockets;
    ServerComponents _servComponents;
    std::unordered_map<Message::Type, std::function<void(long long&, const Message&)>> _actions;
};