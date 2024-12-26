#ifndef NODE_H
#define NODE_H

#include <cassert>
#include <memory>

using namespace std;


enum struct SonKind {
    LEFT, RIGHT, NO_PARENT
};

template<class T>
class Node {
public:
    int key;
    unique_ptr<T> data;
    Node *left;
    Node *right;
    Node *parent;
    int height;

    Node(int key, unique_ptr<T> data, Node<T> *parent);

    virtual ~Node();

    Node(const Node &) = delete;

    Node &operator=(const Node &) = delete;

    bool hasLeft() const;

    bool hasRight() const;

    bool hasParent() const;

    SonKind sonKind() const;

    int numSons() const;


};


template<class T>
Node<T>::Node(int key, unique_ptr<T> data, Node<T> *parent) : key(key), data(std::move(data)),
                                                              left(nullptr), right(nullptr),
                                                              parent(parent), height(0) {};

template<class T>
Node<T>::~Node() {
    delete this->left;  // Recursively delete left child
    delete this->right;  // Recursively delete right child
};

/**
 * // Check if the node has a left child
 */
template<class T>
bool Node<T>::hasLeft() const {
    return this->left != nullptr;
}

/**
 * // Check if the node has a right child
 */
template<class T>
bool Node<T>::hasRight() const {
    return this->right != nullptr;
}

/**
 * // Check if the node has a right parent
 */
template<class T>
bool Node<T>::hasParent() const {
    return this->parent != nullptr;
}


/**
 * Determine the kind of son (left or right) this node is.
 * @return SonKind enum representing the kind of son.
 */
template<class T>
SonKind Node<T>::sonKind() const {
    if (!hasParent()) {
        return SonKind::NO_PARENT;
    } else if (this->parent->right == this) {
        return SonKind::RIGHT;
    }
    assert(this->parent->left == this);
    return SonKind::LEFT;
}

/**
 * Count the number of children this node has.
 * @return The number of children (0, 1, or 2).
 */
template<class T>
int Node<T>::numSons() const {
    return hasLeft() + hasRight();
}

#endif