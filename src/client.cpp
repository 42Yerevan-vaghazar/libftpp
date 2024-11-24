#include "client.hpp"
#include <iostream>
#include <unordered_map>

std::unordered_map<Message::Type, std::function<void(const Message&)>> clientActions;

void Client::connect(const std::string& address, const size_t &port) {
    std::cout << "Connecting to " << address << ":" << port << std::endl;

    // Create socket
    if ((_clientCompoments.sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    // Configure server address structure
    _clientCompoments.serv_addr.sin_family = AF_INET;
    _clientCompoments.serv_addr.sin_port = htons(port);

    // Convert address (IP) from string to binary form
    if (inet_pton(AF_INET, address.c_str(), &_clientCompoments.serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported: " << address << std::endl;
        close(_clientCompoments.sock);
        return;
    }

    // Attempt to connect to the server
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
    clientActions[messageType] = action;
}

void Client::send(const Message& message) {
    std::cout << "Sending message of type " << message.getType() << std::endl;
}

void Client::update() {
    std::cout << "Client updating..." << std::endl;
}
