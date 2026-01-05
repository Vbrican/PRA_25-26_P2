#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <ostream>
#include <stdexcept>
#include <string>

#include "Dict.h"
#include "TableEntry.h"

#include "../../P1/PRA_25-26_P1/ListLinked.h"

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

public:
    HashTable(int size) : n(0), max(size), table(nullptr) {
        table = new ListLinked<TableEntry<V>>*[max];
        for (int i = 0; i < max; ++i) table[i] = new ListLinked<TableEntry<V>>();
    }

    ~HashTable() {
        if (table) {
            for (int i = 0; i < max; ++i) delete table[i];
            delete[] table;
        }
    }

    void insert(std::string key, V value) override {
        int idx = h(key);
        TableEntry<V> probe(key);
        int pos = table[idx]->search(probe);
        if (pos != -1) throw std::runtime_error("key already exists");
        table[idx]->add(TableEntry<V>(key, value));
        ++n;
    }

    V search(std::string key) override {
        int idx = h(key);
        TableEntry<V> probe(key);
        int pos = table[idx]->search(probe);
        if (pos == -1) throw std::runtime_error("key not found");
        return table[idx]->get(pos).value;
    }

    V remove(std::string key) override {
        int idx = h(key);
        TableEntry<V> probe(key);
        int pos = table[idx]->search(probe);
        if (pos == -1) throw std::runtime_error("key not found");
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
        for (int i = 0; i < th.max; ++i) {
            out << i << ":";
            int sz = th.table[i]->size();
            for (int j = 0; j < sz; ++j) {
                out << " " << th.table[i]->get(j);
            }
            out << "\n";
        }
        return out;
    }
};

#endif

