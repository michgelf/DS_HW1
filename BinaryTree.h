#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Node.h"


template<class T>
class BinaryTree {


    /**
     * Apply a function to each node in the tree.
     */
    template<typename Func>
    void traverseAndApplyAux(Node<T>* node, Func func);

    /**
     * Apply a boolean function to each node in the tree.
     * @return True if the function returns true for all nodes, false otherwise.
     */
    template<typename Func>
    bool allNodesSatisfyAux(Node<T>* node, Func func);

    /**
     * Find the first node that matches a given condition.
     * @return The first node that matches the condition, or nullptr if no match is found.
     */
    template<class Condition>
    Node<T>* findFirstMatchingNodeAux(Node<T>* node, Condition cond);

public:
    Node<T>* root;

    BinaryTree();

    virtual ~BinaryTree();

    /**
     * Check if the tree is empty.
     */
    bool isEmpty() const;

    BinaryTree(const BinaryTree&) = delete;

    BinaryTree& operator=(const BinaryTree&) = delete;

    /**
     * Apply a function to each node in the tree.
     */
    template<typename Func>
    void traverseAndApply(Func func);

    /**
     * Apply a boolean function to each node in the tree.
     * @return True if the function returns true for all nodes, false otherwise.
     */
    template<typename Func>
    bool allNodesSatisfy(Func func);

    /**
     * Find the first node that matches a given condition.
     * @return The first node that matches the condition, or nullptr if no match is found.
     */
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
void BinaryTree<T>::traverseAndApply(Func func) {
    traverseAndApplyAux(root, func);
}

template<class T>
template<typename Func>
bool BinaryTree<T>::allNodesSatisfy(Func func) {
    return allNodesSatisfyAux(root, func);
}

template<class T>
template<class Condition>
Node<T>* BinaryTree<T>::findFirstMatchingNode(Condition cond) {
    return findFirstMatchingNodeAux(root, cond);
}

// private methods

template<class T>
template<typename Func>
void BinaryTree<T>::traverseAndApplyAux(Node<T>* node, Func func) {
    if (node) {
        func(*(node->data));
        traverseAndApplyAux(node->left, func);
        traverseAndApplyAux(node->right, func);
    }
}

template<class T>
template<typename Func>
bool BinaryTree<T>::allNodesSatisfyAux(Node<T>* node, Func func) {
    if (!node) {
        return true;
    }
    return func(*(node->data)) && allNodesSatisfyAux(node->left, func) &&
           allNodesSatisfyAux(node->right, func);
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
