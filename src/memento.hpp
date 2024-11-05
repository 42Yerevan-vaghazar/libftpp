#pragma once
#include "data_buffer.hpp"

class Memento
{
public:
    virtual ~Memento();
    class Snapshot : public DataBuffer {};
    Memento::Snapshot save();
    void load(const Memento::Snapshot& state);
protected:
    virtual void _saveToSnapshot(Memento::Snapshot& snapshot) = 0;
    virtual void _loadFromSnapshot(Memento::Snapshot& snapshot) = 0;
};