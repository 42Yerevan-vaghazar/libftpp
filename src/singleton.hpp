#pragma once
#include <iostream>

template <typename TType>
class Singleton
{
public:
    Singleton() = delete;
    Singleton(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;
    Singleton &operator=(const Singleton &) = delete;
    Singleton &operator=(Singleton &&) = delete;
    ~Singleton() = delete;
    static TType* instance() {
        if (_obj == nullptr) {
            throw std::runtime_error("Exception: Instance not yet created");
        }
        return _obj;
    };
    template<typename ... TArgs>
    static void instantiate(TArgs&& ...p_args) {
        if (_obj) {
            throw std::runtime_error("Exception: The instance is already created");
        }
        _obj = new TType(p_args...);
    };
private:
    static TType *_obj;
};

template <typename TType>
TType* Singleton<TType>::_obj = nullptr;