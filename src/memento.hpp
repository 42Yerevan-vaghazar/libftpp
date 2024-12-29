#pragma once
#include "data_buffer.hpp"

class Memento
{
public:
    virtual ~Memento();
    using Snapshot = DataBuffer;
    Memento::Snapshot save();
    void load(const Memento::Snapshot& state);
private:
    virtual void _saveToSnapshot(Memento::Snapshot& snapshot) const = 0;
    virtual void _loadFromSnapshot(Memento::Snapshot& snapshot) = 0;
};