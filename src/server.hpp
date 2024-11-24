#pragma once
#include "message.hpp"
#include <functional>
#include <vector>
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
private:
    ServerComponents _servComponents;
    std::unordered_map<Message::Type, std::function<void(long long&, const Message&)>> _actions;
};