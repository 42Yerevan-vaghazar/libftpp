#include "memento.hpp"

Memento::Snapshot Memento::save() {
    Memento::Snapshot tmp;
    _saveToSnapshot(tmp);
    return (tmp);
}

void Memento::load(const Memento::Snapshot& state) {
    _loadFromSnapshot(const_cast<Memento::Snapshot&>(state));
}
