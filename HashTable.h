#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <ostream>
#include <stdexcept>
#include <string>

#include "Dict.h"
#include "TableEntry.h"
#include "../PRA_25-26_P1/ListLinked.h"

template <typename V>
class HashTable : public Dict<V> {

private:
    int n;
    int max;
    ListLinked<TableEntry<V>>** table;

    int h(std::string key) {
        int sum = 0;
        for (size_t i = 0; i < key.size(); ++i) sum += int(key.at(i));
        return sum % max;
    }

    int findPosInBucket(int idx, const std::string& key) {
        TableEntry<V> probe(key);
        int sz = table[idx]->size();
        for (int i = 0; i < sz; ++i) {
            if (table[idx]->get(i) == probe) return i;
        }
        return -1;
    }

public:
    HashTable(int size) : n(0), max(size) {
        table = new ListLinked<TableEntry<V>>*[max];
        for (int i = 0; i < max; ++i) table[i] = new ListLinked<TableEntry<V>>();
    }

    ~HashTable() {
        for (int i = 0; i < max; ++i) delete table[i];
        delete[] table;
    }

    void insert(std::string key, V value) override {
        int idx = h(key);
        if (findPosInBucket(idx, key) != -1)
            throw std::runtime_error("Key '" + key + "' already exists!");
        table[idx]->insert(table[idx]->size(), TableEntry<V>(key, value));
        ++n;
    }

    V search(std::string key) override {
        int idx = h(key);
        int pos = findPosInBucket(idx, key);
        if (pos == -1) throw std::runtime_error("Key '" + key + "' not found!");
        return table[idx]->get(pos).value;
    }

    V remove(std::string key) override {
        int idx = h(key);
        int pos = findPosInBucket(idx, key);
        if (pos == -1) throw std::runtime_error("Key '" + key + "' not found!");
        TableEntry<V> removed = table[idx]->remove(pos);
        --n;
        return removed.value;
    }

    int entries() override {
        return n;
    }

    int capacity() {
        return max;
    }

    V operator[](std::string key) {
        return search(key);
    }

    friend std::ostream& operator<<(std::ostream &out, const HashTable<V> &th) {
        out << "HashTable [entries: " << th.n << ", capacity: " << th.max << "]\n";
        out << "==============\n\n";
        for (int i = 0; i < th.max; ++i) {
            out << "== Cubeta " << i << " ==\n\n";
            out << *(th.table[i]) << "\n\n";
        }
        out << "==============\n";
        return out;
    }
};

#endif

