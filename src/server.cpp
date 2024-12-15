#include "server.hpp"
#include <iostream>
#include <unordered_map>
#include "data_buffer.hpp"

void logError(const std::string& message) {
    std::cerr << message << ": " << strerror(errno) << std::endl;
}

void Server::start(const size_t &p_port) {

    if ((_servComponents.server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    if (setsockopt(_servComponents.server_fd, SOL_SOCKET,
            SO_REUSEADDR, &_servComponents.opt, sizeof(_servComponents.opt))) {
        logError("Failed to set socket options.");
        close(_servComponents.server_fd);
        return;
    }

    _servComponents.address.sin_family = AF_INET;
    _servComponents.address.sin_addr.s_addr = INADDR_ANY; // Use localhost
    _servComponents.address.sin_port = htons(p_port);

    if (bind(_servComponents.server_fd, (struct sockaddr*)&_servComponents.address, sizeof(_servComponents.address)) < 0) {
        std::cerr << "Failed to bind socket to port " << p_port << "." << std::endl;
        close(_servComponents.server_fd);
        return;
    }

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
    auto it = _clientSockets.find(clientID);
    if (it == _clientSockets.end()) {
        std::cerr << "Client ID " << clientID << " not connected." << std::endl;
        return;
    }

    int socket_fd = it->second->getSocket();
    std::string serializedMessage = message.serialize();
    std::string messageSize = std::to_string(serializedMessage.size()) + "\r\n";

    if (send(socket_fd, messageSize.c_str(), messageSize.size(), 0) == -1) {
        logError("Failed to send messageSize to client " + std::to_string(clientID));
    }
    
    if (send(socket_fd, serializedMessage.c_str(), serializedMessage.size(), 0) == -1) {
        logError("Failed to send message to client " + std::to_string(clientID));
    }
}

void Server::sendToArray(const Message& message, std::vector<long long> clientIDs) {
    for (const auto& clientID : clientIDs) {
        sendTo(message, clientID);
    }
}

void Server::sendToAll(const Message& message) {
    std::cout << "Sending message of type " << message.getType()
              << " to all connected clients" << std::endl;

    std::string serializedMessage = message.serialize();

    for (const auto& [clientID, client] : _clientSockets) {
        if (send(client->getSocket(), serializedMessage.c_str(), serializedMessage.size(), 0) == -1) {
            logError("Failed to send message to client " + std::to_string(clientID));
        }
    }
}
void Server::update() {
    fd_set readfds;
    FD_ZERO(&readfds);

    int max_fd = _servComponents.server_fd;
    FD_SET(_servComponents.server_fd, &readfds);

    for (const auto& [clientID, client] : _clientSockets) {
        int socket_fd = client->getSocket();
        FD_SET(socket_fd, &readfds);
        if (socket_fd > max_fd) {
            max_fd = socket_fd;
        }
    }
    fd_set tmpReadfds = readfds;
    // std::cout << "select start" << std::endl;
    int activity = select(max_fd + 1, &tmpReadfds, nullptr, nullptr, nullptr);
    // std::cout << "select end" << std::endl;

    if (activity < 0 && errno != EINTR) {
        logError("Select error");
        return;
    }

    if (FD_ISSET(_servComponents.server_fd, &tmpReadfds)) {
        int new_socket = accept(_servComponents.server_fd,
                                (struct sockaddr*)&_servComponents.address,
                                (socklen_t*)&_servComponents.addrlen);
        if (new_socket < 0) {
            logError("Failed to accept new connection");
            return;
        }

        long long clientID = new_socket;
        _clientSockets[clientID] = new Client(new_socket);

        std::cout << "New client connected with ID: " << clientID << std::endl;
    }

    for (auto it = _clientSockets.begin(); it != _clientSockets.end(); ) {
        long long clientID = it->first;
        int socket_fd = it->second->getSocket();

        if (FD_ISSET(socket_fd, &readfds)) {
            char buffer[1024] = {0};
            ssize_t bytesReceived = recv(socket_fd, buffer, sizeof(buffer), 0);

            if (bytesReceived <= 0) {
                if (bytesReceived == 0) {
                    std::cout << "Client " << clientID << " disconnected." << std::endl;
                } else {
                    logError("Recv error for client " + std::to_string(clientID));
                }

                close(socket_fd);
                delete it->second;
                it = _clientSockets.erase(it);
                continue;
            }
            it->second->receiveMessage(buffer);

            auto messagesQueue = it->second->getMessages();

            while (messagesQueue->empty() == false) {
                auto message = messagesQueue->front();
                messagesQueue->pop();
                auto actionIt = _actions.find(message->getType());
                if (actionIt != _actions.end()) {
                    actionIt->second(clientID, *message);
                } else {
                    std::cerr << "No action defined for message type: " << message->getType() << std::endl;
                }
            }
        }
        ++it;
    }
}