#pragma once
#include <iostream>

template<typename TType>
class Pool
{
public:
    template <typename T>
    class Object
    {
    public:
        Object(T &rhs, size_t &i) : _obj(rhs), _relatedIndex(i) {
        }

        TType* operator->() {
            return (&_obj);
        }

        ~Object() {
            _obj.~TType();
            --_relatedIndex;
        }

    private:
        TType &_obj;
        size_t &_relatedIndex;
    };
    Pool() {
        _arr = nullptr;
        _size = 0;
        _index = 0;
    }

    ~Pool() {
        delete [] _arr;
    }

    void resize(const size_t& numberOfObjectStored) {
        TType *tmp = new TType[numberOfObjectStored];

        for (size_t i = 0; i < _size; i++) {
            tmp[i] = _arr[i];
        }
        delete [] _arr;
        _arr = tmp;
        _size = numberOfObjectStored;
    }

    template<typename ... TArgs>
    Pool::Object<TType> acquire(TArgs&& ...p_args) {
        if (_index == _size) {
            throw std::out_of_range("out of range");
        }
        _arr[_index++] = TType(p_args...);
        Pool::Object<TType> tmp(_arr[_index], _index);
        return (tmp);
    }

   
private:
    TType *_arr;
    size_t _size;
    size_t _index;
};