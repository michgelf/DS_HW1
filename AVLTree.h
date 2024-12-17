#ifndef AVLTREE_H
#define AVLTREE_H

#include "BinarySearchTree.h"

template<class T>
class AVLTree : public BinarySearchTree<T> {

private:

    static int max(int a, int b);

    static int nodeHeight(Node<T>* node);

    static int balanceFactor(Node<T>* node);

    static void updateHeight(Node<T>* node);

    void rollAux(Node<T>* node, bool isLL);

    void rollLL(Node<T>* node);

    void rollRR(Node<T>* node);

    void rollLR(Node<T>* node);

    void rollRL(Node<T>* node);

    bool rollIfNeeded(Node<T>* node);

public:

    Node<T>* insert(int key, const T& data) override;

    Node<T>* remove(Node<T>* nodeToRemove) override;

};


template<class T>
int AVLTree<T>::max(int a, int b) {
    return a < b ? b : a;
}

template<class T>
int AVLTree<T>::nodeHeight(Node<T>* node) {
    return node ? node->height : -1;
}

template<class T>
void AVLTree<T>::updateHeight(Node<T>* node) {
    node->height = 1 + max(nodeHeight(node->left), nodeHeight(node->right));
}

template<class T>
void AVLTree<T>::rollAux(Node<T>* node, bool isLL) {
    Node<T>* child;
    Node<T>* grandchild;
    if (isLL) {
        child = node->left;
        grandchild = node->left->right;
        node->left = grandchild;
        child->right = node;
    } else {
        child = node->right;
        grandchild = node->right->left;
        node->right = grandchild;
        child->left = node;
    }

    if (grandchild != nullptr) {
        grandchild->parent = node;
    }
    Node<T>* tempNodeParent = node->parent;
    SonKind nodeSonKind = node->sonKind();
    node->parent = child;
    child->parent = tempNodeParent;

    if (node == this->root) {
        this->root = child;
    } else if (nodeSonKind == SonKind::RIGHT) {
        tempNodeParent->right = child;
    } else if (nodeSonKind == SonKind::LEFT) {
        tempNodeParent->left = child;
    }

    updateHeight(node);
    updateHeight(child);
}

template<class T>
void AVLTree<T>::rollLL(Node<T>* node) {
    rollAux(node, true);
}

template<class T>
void AVLTree<T>::rollRR(Node<T>* node) {
    rollAux(node, false);
}

template<class T>
void AVLTree<T>::rollLR(Node<T>* node) {
    this->rollRR(node->left);
    this->rollLL(node);
}

template<class T>
void AVLTree<T>::rollRL(Node<T>* node) {
    this->rollLL(node->right);
    this->rollRR(node);
}

template<class T>
bool AVLTree<T>::rollIfNeeded(Node<T>* node) {
    int bf = balanceFactor(node);
    if (bf == 2) {
        if (balanceFactor(node->left) == -1) {
            rollLR(node);
        } else {
            rollLL(node);
        }
        return true;
    } else if (bf == -2) {
        if (balanceFactor(node->right) == 1) {
            rollRL(node);
        } else {
            rollRR(node);
        }
        return true;
    }
    return false;
}


template<class T>
Node<T>* AVLTree<T>::insert(int key, const T& data) {
    Node<T>* insertedNode = BinarySearchTree<T>::insert(key, data);;
    if (insertedNode == nullptr) {
        return nullptr;
    }
    Node<T>* v = insertedNode;

    while (v != this->root) {
        Node<T>* p = v->parent;
        if (nodeHeight(p) >= nodeHeight(v) + 1) {
            break;
        }
        p->height = nodeHeight(v) + 1;
        if (rollIfNeeded(p)) {
            break;
        }
        v = p;
    }
    return insertedNode;
}

template<class T>
Node<T>* AVLTree<T>::remove(Node<T>* nodeToRemove) {
    Node<T>* parent = BinarySearchTree<T>::remove(nodeToRemove);
    Node<T>* v = parent;
    while (v != nullptr) {
        int oldHeight = nodeHeight(v);
        updateHeight(v);
        rollIfNeeded(v);
        if (v->height == oldHeight) {
            break;
        }
        v = v->parent;
    }
    return parent;
}

template<class T>
int AVLTree<T>::balanceFactor(Node<T>* node) {
    return nodeHeight(node->left) - nodeHeight(node->right);
}


#endif
