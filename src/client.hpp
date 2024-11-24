#pragma once
#include "message.hpp"
#include <functional>
#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class Client
{
public:
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
    ClientComponets _clientCompoments;
};