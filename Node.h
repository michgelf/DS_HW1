#ifndef NODE_H
#define NODE_H

#include <cassert>


enum struct SonKind {
    LEFT, RIGHT, NO_PARENT
};

template<class T>
class Node {
public:
    T data;
    Node* left;
    Node* right;
    Node* parent;
    int height;

    Node(const T& data, Node<T>* parent);

    virtual ~Node();

    Node(const Node&) = delete;

    Node& operator=(const Node&) = delete;

    static int nodeHeight(Node<T>* node);

    bool hasLeft() const;

    bool hasRight() const;

    bool hasParent() const;

    SonKind sonKind() const;

    int numSons() const;

    int bf() const;


};


template<class T>
Node<T>::Node(const T& data, Node<T>* parent) : data(data), left(nullptr), right(nullptr),
                                                parent(parent), height(0) {};

template<class T>
Node<T>::~Node() {
    delete this->left;
    delete this->right;
};

template<class T>
bool Node<T>::hasLeft() const {
    return this->left != nullptr;
}

template<class T>
bool Node<T>::hasRight() const {
    return this->right != nullptr;
}

template<class T>
bool Node<T>::hasParent() const {
    return this->parent != nullptr;
}

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

template<class T>
int Node<T>::numSons() const {
    return hasLeft() + hasRight();
}

template<class T>
int Node<T>::nodeHeight(Node<T>* node) {
    return node ? node->height : -1;
}

template<class T>
int Node<T>::bf() const {
    return nodeHeight(this->left) - nodeHeight(this->right);
}

#endif