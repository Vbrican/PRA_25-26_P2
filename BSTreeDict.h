#ifndef BSTREEDICT_H
#define BSTREEDICT_H

#include <ostream>
#include <stdexcept>
#include <string>

#include "Dict.h"
#include "BSTree.h"
#include "TableEntry.h"

template <typename V>
class BSTreeDict : public Dict<V> {

private:
    BSTree<TableEntry<V>>* tree;
    int nEntries;

public:
    BSTreeDict() : tree(new BSTree<TableEntry<V>>()), nEntries(0) {}

    ~BSTreeDict() {
        delete tree;
        tree = nullptr;
    }

    int entries() override {
        return nEntries;
    }

    void insert(std::string key, V value) override {
        TableEntry<V> probe(key);

        bool exists = true;
        try {
            tree->search(probe);
        } catch (const std::runtime_error&) {
            exists = false;
        }

        if (exists) {
            throw std::runtime_error("Duplicated element!");
        }

        tree->insert(TableEntry<V>(key, value));
        nEntries++;
    }

    V search(std::string key) override {
        TableEntry<V> probe(key);

        try {
            return tree->search(probe).value;
        } catch (const std::runtime_error&) {
            throw std::runtime_error("Element not found!");
        }
    }

    V remove(std::string key) override {
        TableEntry<V> probe(key);

        V removedValue;
        try {
            removedValue = tree->search(probe).value;
        } catch (const std::runtime_error&) {
            throw std::runtime_error("Element not found!");
        }

        try {
            tree->remove(probe);
        } catch (const std::runtime_error&) {
            throw std::runtime_error("Element not found!");
        }

        nEntries--;
        return removedValue;
    }

    V operator[](std::string key) {
        return search(key);
    }

    friend std::ostream& operator<<(std::ostream &out, const BSTreeDict<V> &bs) {
        out << "[ " << *(bs.tree) << " ]";
        return out;
    }
};

#endif

