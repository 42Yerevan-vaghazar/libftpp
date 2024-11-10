#pragma once
#include <string>
#include <thread>
#include <functional>

class Thread
{
public:
    Thread(const std::string& name, std::function<void()> functToExecute);
    void start();
    void stop();
private:
    static void _start(const std::string& name, const std::function<void()> &functToExecute);
private:
    std::string _name;
    std::function<void()> _functToExecute;
    std::thread _thread;
};