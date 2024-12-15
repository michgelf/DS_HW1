#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "assert.h"

enum struct SonKind {
    LEFT,
    RIGHT,
    NO_PARENT
};

template<class T>
class Node {
public:
    T data;
    Node* left;
    Node* right;
    Node* parent;

//    explicit Node(const T& data) : data(data), left(nullptr), right(nullptr), parent(nullptr) {};

    Node(const T& data, Node<T>* parent) : data(data), left(nullptr), right(nullptr),
                                           parent(parent) {};

    Node(const Node&) = delete;

    Node& operator=(const Node&) = delete;

    virtual ~Node() {
        delete this->left;
        delete this->right;
    };

    bool hasLeft() const {
        return this->left != nullptr;
    }

    bool hasRight() const {
        return this->right != nullptr;
    }

    bool hasParent() const {
        return this->parent != nullptr;
    }

    SonKind sonKind() const {
        if (!hasParent()) {
            return SonKind::NO_PARENT;
        } else if (this->parent->right == this) {
            return SonKind::RIGHT;
        }
        assert(this->parent->left == this);
        return SonKind::LEFT;
    }

    int numSons() const {
        return hasLeft() + hasRight();
    }

    int height = 0;

    int bf() {
        int leftHeight = hasLeft() ? this->left->height : -1;
        int rightHeight = hasRight() ? this->right->height : -1;
        return leftHeight - rightHeight;
    }


};

template<class T>
class BinaryTree {

public:
    Node<T>* root;

    BinaryTree() : root(nullptr) {};

    virtual ~BinaryTree() {
        delete root;
    };

    bool isEmpty() const {
        return root == nullptr;
    }

    BinaryTree(const BinaryTree&) = delete;

    BinaryTree& operator=(const BinaryTree&) = delete;


};

#endif
