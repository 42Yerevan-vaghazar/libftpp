#pragma once
#include <iostream>
#include <memory>

template<typename TType>
class Pool
{
public:
    template <typename T>
    class Object
    {
    public:
        Object(T &rhs, std::function<void()> callBack) : _obj(rhs), _callBack(callBack) {
        }

        TType* operator->() {
            return (&_obj);
        }

        // for checking reasons
        TType* operator()() {
            return (&_obj);
        }

        ~Object() {
            _callBack();
        }

    private:
        TType &_obj;
        std::function<void()> _callBack;
    };
    Pool() : _arr(nullptr), _size(0), _index(0) {}

    ~Pool() {
        traits_t1::deallocate(_alloc, _arr, _size);
    }

    void resize(const size_t& numberOfObjectStored) {
        TType *tmp = traits_t1::allocate(_alloc, numberOfObjectStored);
        // TType *tmp = new TType[numberOfObjectStored];

        for (size_t i = 0; i < _size; i++) {
            tmp[i] = _arr[i];
        }
        traits_t1::deallocate(_alloc, _arr, _size);
        _arr = tmp;
        _size = numberOfObjectStored;
    }

    template<typename ... TArgs>
    Pool::Object<TType> acquire(TArgs&& ...p_args) {
        if (_index == _size) {
            throw std::out_of_range("out of range");
        }
        traits_t1::construct(_alloc, &_arr[_index], p_args...);
        Pool::Object<TType> tmp(_arr[_index], [&]() {
            traits_t1::destroy(_alloc, &_arr[_index]);
            --_index;
        });
        _index++;
        return (tmp);
    }

   
private:
    std::allocator<TType> _alloc;
    using traits_t1 = std::allocator_traits<decltype(_alloc)>;
    TType *_arr;
    size_t _size;
    size_t _index;
};