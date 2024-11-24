#include "server.hpp"
#include <iostream>
#include <unordered_map>


void Server::start(const size_t &p_port) {

    // Creating socket file descriptor
    if ((_servComponents.server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    // Attach socket to port
    if (setsockopt(_servComponents.server_fd, SOL_SOCKET,
            SO_REUSEADDR | SO_REUSEPORT, &_servComponents.opt, sizeof(_servComponents.opt))) {
        std::cerr << "Failed to set socket options." << std::endl;
        close(_servComponents.server_fd);
        return;
    }

    // Bind the socket to the port
    _servComponents.address.sin_family = AF_INET;
    _servComponents.address.sin_addr.s_addr = INADDR_ANY; // Use localhost
    _servComponents.address.sin_port = htons(p_port);

    if (bind(_servComponents.server_fd, (struct sockaddr*)&_servComponents.address, sizeof(_servComponents.address)) < 0) {
        std::cerr << "Failed to bind socket to port " << p_port << "." << std::endl;
        close(_servComponents.server_fd);
        return;
    }

    // Listen for incoming connections
    if (listen(_servComponents.server_fd, 3) < 0) {
        std::cerr << "Failed to listen on port " << p_port << "." << std::endl;
        close(_servComponents.server_fd);
        return;
    }

    std::cout << "Server started on localhost:" << p_port << std::endl;
}

void Server::defineAction(const Message::Type &messageType,
    const std::function<void(long long& clientID, const Message& msg)>& action) {
    _actions[messageType] = action;
}

void Server::sendTo(const Message& message, long long clientID) {
    std::cout << "Sending message of type " << message.getType()
              << " to client " << clientID << std::endl;
}

void Server::sendToArray(const Message& message, std::vector<long long> clientIDs) {
    for (const auto& clientID : clientIDs) {
        sendTo(message, clientID);
    }
}

void Server::sendToAll(const Message& message) {
    std::cout << "Sending message of type " << message.getType()
              << " to all connected clients" << std::endl;
}

void Server::update() {
    // std::cout << "Server updating..." << std::endl;
    // int new_socket;
    // if ((new_socket = accept(_servComponents.server_fd, (struct sockaddr*)&_servComponents.address, (socklen_t*)&_servComponents.addrlen)) < 0) {
    //     std::cerr << "Failed to accept connection." << std::endl;
    //     // close(_servComponents.server_fd);
    //     return;
    // }
    // std::cout << "Connection accepted." << std::endl;
}
