#include "client.hpp"
#include <iostream>

void Client::connect(const std::string& address, const size_t &port) {
    std::cout << "Connecting to " << address << ":" << port << std::endl;
    std::string addressTmp = address;
    if ((_clientCompoments.sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    _clientCompoments.serv_addr.sin_family = AF_INET;
    _clientCompoments.serv_addr.sin_port = htons(port);

    if (addressTmp == "localhost") {
        addressTmp = "127.0.0.1";
    }

    if (inet_pton(AF_INET, addressTmp.c_str(), &_clientCompoments.serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported: " << address << std::endl;
        close(_clientCompoments.sock);
        return;
    }

    if (::connect(_clientCompoments.sock, (struct sockaddr*)&_clientCompoments.serv_addr, sizeof(_clientCompoments.serv_addr)) < 0) {
        std::cerr << "Connection to " << address << ":" << port << " failed." << std::endl;
        close(_clientCompoments.sock);
        return;
    }

    std::cout << "Successfully connected to " << address << ":" << port << std::endl;
}

void Client::disconnect() {
    std::cout << "Disconnecting..." << std::endl;
}

void Client::defineAction(const Message::Type &messageType,
    const std::function<void(const Message& msg)>& action) {
    std::cout << "defined action for messageType " << messageType << std::endl;
    _actions[messageType] = action;
}

void Client::send(const Message& message) {
    std::string serializedMessage = message.serialize();
    size_t messageSize = serializedMessage.size();
    std::string sizeStr = std::to_string(messageSize) + "\r\n";

    if (::send(_clientCompoments.sock, sizeStr.c_str(), sizeStr.size(), 0) == -1) {
        std::cerr << "Failed to send message size: " << strerror(errno) << std::endl;
        return;
    }
    
    if (::send(_clientCompoments.sock, serializedMessage.data(), messageSize, 0) == -1) {
        std::cerr << "Failed to send message: " << strerror(errno) << std::endl;
        return;
    }

    std::cout << "Sent message of type " << message.getType() << " with size " << messageSize << " bytes" << std::endl;
}


void Client::update() {
    std::cout << "Client updating..." << std::endl;

    char buffer[1024];
    ssize_t bytesRead = ::recv(_clientCompoments.sock, buffer, sizeof(buffer) - 1, 0);

    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        std::string receivedData(buffer, bytesRead);
        this->receiveMessage(receivedData);;
        auto messagesQueue = this->getMessages();
        while (messagesQueue->empty() == false) {
            auto message = messagesQueue->front();
            messagesQueue->pop();

            auto actionIt = _actions.find(message->getType());
            if (actionIt != _actions.end()) {
                actionIt->second(*message);
            } else {
                std::cerr << "No action defined for message type: " << message->getType() << std::endl;
            }
        }
    } else if (bytesRead == 0) {
        std::cerr << "Server closed the connection." << std::endl;
        disconnect();
    } else {
        std::cerr << "Error receiving data: " << strerror(errno) << std::endl;
    }
}