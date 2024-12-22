#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "BinaryTree.h"

template<class T>
class BinarySearchTree : public BinaryTree<T> {

private:
    int numNodes;

    static Node<T>* findAux(Node<T>* currNode, int key);

    static Node<T>* insertAux(Node<T>* currNode, int key, unique_ptr<T> data);

    void removeLeaf(Node<T>* leaf); //assumes the given node is a leaf

    void removeSingleSonNode(Node<T>* singleSonNode); // assumes the given node has single son

    void removeTwoSonsNode(Node<T>* nodeToRemove); // assumes the given node has 2 sons

public:
    BinarySearchTree();

    Node<T>* find(int key) const;

    virtual Node<T>* insert(int key, unique_ptr<T> data);

    virtual Node<T>* remove(Node<T>* nodeToRemove);

};

template<class T>
BinarySearchTree<T>::BinarySearchTree() : BinaryTree<T>(), numNodes(0) {};


template<class T>
Node<T>* BinarySearchTree<T>::findAux(Node<T>* currNode, int key) {
    if (currNode == nullptr || currNode->key == key) {
        return currNode;
    } else if (key < currNode->key) {
        return findAux(currNode->left, key);
    } else { // currNode->data < data
        return findAux(currNode->right, key);
    }
}

template<class T>
Node<T>* BinarySearchTree<T>::insertAux(Node<T>* currNode, int key, unique_ptr<T> data) {
    if (key < currNode->key) {
        if (currNode->hasLeft()) {
            return insertAux(currNode->left, key, std::move(data));
        } else {
            return currNode->left = new Node<T>(key, std::move(data), currNode);
        }
    } else if (currNode->key < key) {
        if (currNode->hasRight()) {
            return insertAux(currNode->right, key, std::move(data));
        } else {
            return currNode->right = new Node<T>(key, std::move(data), currNode);
        }
    }
    return nullptr;
}

template<class T>
void BinarySearchTree<T>::removeLeaf(Node<T>* leaf) {
    // assumes the given node is a leaf!
    switch (leaf->sonKind()) {
        case SonKind::NO_PARENT:
            this->root = nullptr;
            break;
        case SonKind::LEFT:
            leaf->parent->left = nullptr;
            break;
        case SonKind::RIGHT:
            leaf->parent->right = nullptr;
            break;
    }
    leaf->parent = nullptr;
    delete leaf;
}

template<class T>
void BinarySearchTree<T>::removeSingleSonNode(Node<T>* singleSonNode) {
    Node<T>* childNode = singleSonNode->hasLeft() ? singleSonNode->left : singleSonNode->right;

    switch (singleSonNode->sonKind()) {
        case SonKind::NO_PARENT:
            this->root = childNode;
            break;
        case SonKind::LEFT:
            singleSonNode->parent->left = childNode;
            break;
        case SonKind::RIGHT:
            singleSonNode->parent->right = childNode;
            break;
    }

    childNode->parent = singleSonNode->parent;
    singleSonNode->left = nullptr;
    singleSonNode->right = nullptr;
    singleSonNode->parent = nullptr;
    delete singleSonNode;
}

template<class T>
void BinarySearchTree<T>::removeTwoSonsNode(Node<T>* nodeToRemove) {
    Node<T>* temp = nodeToRemove->right;
    while (temp->hasLeft()) {
        temp = temp->left;
    }
    nodeToRemove->key = temp->key;
    nodeToRemove->data = std::move(temp->data);
    switch (temp->numSons()) {
        case 0:
            removeLeaf(temp);
            break;
        case 1:
            removeSingleSonNode(temp);
            break;
        default:
            assert(false);
    }
}

template<class T>
Node<T>* BinarySearchTree<T>::find(int key) const {
    return findAux(this->root, key);
}

template<class T>
Node<T>* BinarySearchTree<T>::insert(int key, unique_ptr<T> data) {
    Node<T>* newNode;
    if (this->isEmpty()) {
        newNode = this->root = new Node<T>(key, std::move(data), nullptr);
    } else {
        newNode = insertAux(this->root, key, std::move(data));
    }

    if (newNode != nullptr) {
        this->numNodes++;
    }
    return newNode;
}

template<class T>
Node<T>* BinarySearchTree<T>::remove(Node<T>* nodeToRemove) {
    Node<T>* parent = nodeToRemove->parent;
    switch (nodeToRemove->numSons()) {
        case 0:
            removeLeaf(nodeToRemove);
            break;
        case 1:
            removeSingleSonNode(nodeToRemove);
            break;
        case 2:
            removeTwoSonsNode(nodeToRemove);
            break;
    }
    this->numNodes--;
    return parent;
}

#endif
