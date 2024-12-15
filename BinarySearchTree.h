#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "BinaryTree.h"

template<class T>
class BinarySearchTree : public BinaryTree<T> {

private:
    static Node<T>* findAux(Node<T>* currNode, T data) {
        if (currNode == nullptr || currNode->data == data) {
            return currNode;
        } else if (data < currNode->data) {
            return findAux(currNode->left, data);
        } else { // currNode->data < data
            return findAux(currNode->right, data);
        }
    }

    Node<T>* insertAux(Node<T>* currNode, T data) {
        if (data < currNode->data) {
            if (currNode->hasLeft()) {
                return insertAux(currNode->left, data);
            } else {
                return currNode->left = createNode(data, currNode);
            }
        } else if (currNode->data < data) {
            if (currNode->hasRight()) {
                return insertAux(currNode->right, data);
            } else {
                return currNode->right = createNode(data, currNode);
            }
        }
        return nullptr;
    }


    void removeLeaf(Node<T>* leaf) {
        switch (leaf->sonKind()) {
            case SonKind::NO_PARENT:
                this->root = nullptr;
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

    void removeSingleSonNode(Node<T>* singleSonNode) {
        Node<T>* childNode = singleSonNode->hasLeft() ? singleSonNode->left : singleSonNode->right;

        switch (singleSonNode->sonKind()) {
            case SonKind::NO_PARENT:
                this->root = childNode;
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

    void removeTwoSonsNode(Node<T>* nodeToRemove) {
        Node<T>* temp = nodeToRemove->right;
        while (temp->hasLeft()) {
            temp = temp->left;
        }
        nodeToRemove->data = temp->data;
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

protected:
    int numNodes;

    virtual Node<T>* createNode(const T& data, Node<T>* parent) {
        return new Node<T>(data, parent);
    }

public:
    BinarySearchTree() : BinaryTree<T>(), numNodes(0) {};

    Node<T>* find(const T& data) {
        return findAux(this->root, data);
    }

    virtual Node<T>* insert(const T& data) {
        Node<T>* newNode;
        if (this->isEmpty()) {
            newNode = this->root = createNode(data, nullptr);
        } else {
            newNode = insertAux(this->root, data);
        }

        if (newNode != nullptr) {
            this->numNodes++;
        }
        return newNode;
    }


    virtual Node<T>* remove(Node<T>* nodeToRemove) {
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
};


#endif
