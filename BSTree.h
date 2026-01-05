#ifndef BSTREE_H
#define BSTREE_H

#include <ostream>
#include <stdexcept>
#include "BSNode.h"

template <typename T>
class BSTree {
private:
    int nelem;
    BSNode<T>* root;

    BSNode<T>* search(BSNode<T>* n, T e) const {
        if (n == nullptr) throw std::runtime_error("Element not found");
        if (n->elem < e) return search(n->right, e);
        if (n->elem > e) return search(n->left, e);
        return n;
    }

    BSNode<T>* insert(BSNode<T>* n, T e) {
        if (n == nullptr) {
            ++nelem;
            return new BSNode<T>(e);
        }
        if (n->elem < e) n->right = insert(n->right, e);
        else if (n->elem > e) n->left = insert(n->left, e);
        else throw std::runtime_error("Element already exists");
        return n;
    }

    void print_inorder(std::ostream &out, BSNode<T>* n) const {
        if (n == nullptr) return;
        print_inorder(out, n->left);
        out << " " << n->elem;
        print_inorder(out, n->right);
    }

    T max(BSNode<T>* n) const {
        if (n == nullptr) throw std::runtime_error("Element not found");
        if (n->right != nullptr) return max(n->right);
        return n->elem;
    }

    BSNode<T>* remove_max(BSNode<T>* n) {
        if (n->right == nullptr) {
            BSNode<T>* left = n->left;
            delete n;
            return left;
        }
        n->right = remove_max(n->right);
        return n;
    }

    BSNode<T>* remove(BSNode<T>* n, T e) {
        if (n == nullptr) throw std::runtime_error("Element not found");

        if (n->elem < e) {
            n->right = remove(n->right, e);
        } else if (n->elem > e) {
            n->left = remove(n->left, e);
        } else {
            --nelem;
            if (n->left != nullptr && n->right != nullptr) {
                n->elem = max(n->left);
                n->left = remove_max(n->left);
            } else {
                BSNode<T>* tmp = n;
                n = (n->left != nullptr) ? n->left : n->right;
                delete tmp;
            }
        }
        return n;
    }

    void delete_cascade(BSNode<T>* n) {
        if (n == nullptr) return;
        delete_cascade(n->left);
        delete_cascade(n->right);
        delete n;
    }

public:
    BSTree() : nelem(0), root(nullptr) {}

    int size() const {
        return nelem;
    }

    T search(T e) const {
        return search(root, e)->elem;
    }

    T operator[](T e) const {
        return search(e);
    }

    void insert(T e) {
        root = insert(root, e);
    }

    void remove(T e) {
        root = remove(root, e);
    }

    ~BSTree() {
        delete_cascade(root);
        root = nullptr;
        nelem = 0;
    }

    friend std::ostream& operator<<(std::ostream &out, const BSTree<T> &bst) {
        out << "[";
        bst.print_inorder(out, bst.root);
        out << " ]";
        return out;
    }
};

#endif

