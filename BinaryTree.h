#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Node.h"


template<class T>
class BinaryTree {

    template<typename Func>
    void applyFuncAux(Node<T>* node, Func func);

    template<typename Func>
    bool applyFuncBoolAux(Node<T>* node, Func func);

    template<class Condition>
    Node<T>* findFirstMatchingNodeAux(Node<T>* node, Condition cond);

public:
    Node<T>* root;

    BinaryTree();

    virtual ~BinaryTree();

    bool isEmpty() const;

    BinaryTree(const BinaryTree&) = delete;

    BinaryTree& operator=(const BinaryTree&) = delete;

    template<typename Func>
    void applyFunc(Func func);

    template<typename Func>
    bool applyFuncBool(Func func);

    template<class Condition>
    Node<T>* findFirstMatchingNode(Condition cond);

};

template<class T>
BinaryTree<T>::BinaryTree() : root(nullptr) {};


template<class T>
BinaryTree<T>::~BinaryTree() {
    delete root;
};

template<class T>
bool BinaryTree<T>::isEmpty() const {
    return root == nullptr;
}

template<class T>
template<typename Func>
void BinaryTree<T>::applyFunc(Func func) {
    applyFuncAux(root, func);
}

template<class T>
template<typename Func>
bool BinaryTree<T>::applyFuncBool(Func func) {
    return applyFuncBoolAux(root, func);
}

template<class T>
template<class Condition>
Node<T>* BinaryTree<T>::findFirstMatchingNode(Condition cond) {
    return findFirstMatchingNodeAux(root, cond);
}

// private methods

template<class T>
template<typename Func>
void BinaryTree<T>::applyFuncAux(Node<T>* node, Func func) {
    if (node) {
        func(*(node->data));
        applyFuncAux(node->left, func);
        applyFuncAux(node->right, func);
    }
}

template<class T>
template<typename Func>
bool BinaryTree<T>::applyFuncBoolAux(Node<T>* node, Func func) {
    if (!node) {
        return true;
    }
//        auto a1 = func(*(node->data));
//        auto a2 = applyFuncBoolAux(node->left, func);
//        auto a3 = applyFuncBoolAux(node->right, func);
//        return a1 && a2 && a3;
    return func(*(node->data)) && applyFuncBoolAux(node->left, func) &&
           applyFuncBoolAux(node->right, func);
}

template<class T>
template<class Condition>
Node<T>* BinaryTree<T>::findFirstMatchingNodeAux(Node<T>* node, Condition cond) {
    if (!node) {
        return nullptr;
    }
    if (cond(*(node->data))) {
        return node;
    }
    Node<T>* leftResult = findFirstMatchingNodeAux(node->left, cond);
    if (leftResult) {
        return leftResult;
    }
    return findFirstMatchingNodeAux(node->right, cond);
}


#endif
