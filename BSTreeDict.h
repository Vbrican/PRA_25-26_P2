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

public:
    // Constructor: crea ABB vacío con memoria dinámica
    BSTreeDict() {
        tree = new BSTree<TableEntry<V>>();
    }

    // Destructor: libera la memoria del ABB
    ~BSTreeDict() {
        delete tree;
        tree = nullptr;
    }

    // entries(): devuelve todas las entradas del diccionario
    // OJO: esto depende de cómo lo tengáis en BSTree.
    // Variante típica A: BSTree tiene entries() / inOrder() que devuelve List<T>* o vector<T>
    // Variante típica B: Dict exige List<TableEntry<V>>* y BSTree tiene inOrder()
    auto entries() -> decltype(this->tree->entries()) {
        return tree->entries();
    }

    // insert(key, value): delega en tree->insert(TableEntry)
    void insert(std::string key, V value) override {
        TableEntry<V> e(key, value);
        tree->insert(e);
    }

    // search(key): devuelve el valor asociado o lanza excepción si no está
    V search(std::string key) override {
        TableEntry<V> probe(key, V{});

        // --- Ajusta ESTO a tu BSTree.h ---
        // Caso 1 (muy común): bool BSTree::search(const T& x, T& outFound)
        // TableEntry<V> found;
        // if (!tree->search(probe, found)) throw std::out_of_range("Key not found");
        // return found.value;

        // Caso 2: T* BSTree::search(const T& x)  (devuelve puntero a elemento)
        auto ptr = tree->search(probe);
        if (ptr == nullptr) {
            throw std::out_of_range("Key not found");
        }
        return ptr->value;
    }

    // remove(key): elimina la entrada o lanza excepción si no existe
    void remove(std::string key) override {
        TableEntry<V> probe(key, V{});

        // Caso típico: bool remove(const T&)
        bool ok = tree->remove(probe);
        if (!ok) {
            throw std::out_of_range("Key not found");
        }
    }

    // operator[]: interfaz rápida a search(key)
    V operator[](std::string key) {
        return search(key);
    }

    // Imprimir el diccionario: normalmente delegas en el << del BSTree o imprimes entries()
    friend std::ostream& operator<<(std::ostream& out, const BSTreeDict<V>& bs) {
        // Si BSTree ya tiene operator<<:
        out << *(bs.tree);
        return out;

        // Alternativa si NO hay << en BSTree:
        // auto ents = bs.tree->entries();
        // for (...) out << ...;
        // return out;
    }
};

#endif

