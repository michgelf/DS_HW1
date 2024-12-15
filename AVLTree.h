#ifndef AVLTREE_H
#define AVLTREE_H

#include "BinarySearchTree.h"

#include <algorithm>
//
//template<class T>
//class AVLNode : public Node<T> {
//public:
//    int height;
//
//    AVLNode(T data, Node<T>* parent) : Node<T>(data, parent), height(0) {};
//
//    int bf() {
//        int leftHeight = this->hasLeft() ? this->left->height : -1;
//        int rightHeight = this->hasRight() ? this->right->height : -1;
//        return leftHeight - rightHeight;
//    }
//
//};

template<class T>
class AVLTree : public BinarySearchTree<T> {

private:

    static int nodeHeight(Node<T>* node) {
        return node ? node->height : -1; // Return -1 if the node is null
    }

//
//    void rollLL(Node<T>* node) {
//        bool isLL = true;
//        Node<T>* child;
//        Node<T>* grandchild;
//        isLL = true;
//        if (isLL) {
//            child = node->left;
//            grandchild = child->right;
//            node->left = grandchild;
//            child->right = node;
//        } else {
//            child = node->right;
//            grandchild = child->left;
//            node->right = grandchild;
//            child->left = node;
//        }
//
//
//        if (grandchild != nullptr) {
//            grandchild->parent = node;
//        }
//        Node<T>* tempNodeParent = node->parent;
//        SonKind nodeSonKind = node->sonKind();
//        node->parent = child;
//        child->parent = tempNodeParent;
//
//        if (node == this->root) {
//            this->root = child;
//        } else if (nodeSonKind == SonKind::RIGHT) {
//            tempNodeParent->right = child;
//        } else if (nodeSonKind == SonKind::LEFT) {
//            tempNodeParent->left = child;
//        }
//
//        node->height = 1 + std::max(nodeHeight(node->left), nodeHeight(node->right));
//        child->height = 1 + std::max(nodeHeight(child->left), nodeHeight(child->right));
//    }
//
//    void rollRR(Node<T>* node) {
//        bool isLL = true;
//        Node<T>* child;
//        Node<T>* grandchild;
//        isLL = false;
//        if (isLL) {
//            child = node->left;
//            grandchild = node->left->right;
//            node->left = grandchild;
//            child->right = node;
//        } else {
//            child = node->right;
//            grandchild = node->right->left;
//            node->right = grandchild;
//            child->left = node;
//        }
//
//        if (grandchild != nullptr) {
//            grandchild->parent = node;
//        }
//        Node<T>* tempNodeParent = node->parent;
//        SonKind nodeSonKind = node->sonKind();
//        node->parent = child;
//        child->parent = tempNodeParent;
//
//        if (node == this->root) {
//            this->root = child;
//        } else if (nodeSonKind == SonKind::RIGHT) {
//            tempNodeParent->right = child;
//        } else if (nodeSonKind == SonKind::LEFT) {
//            tempNodeParent->left = child;
//        }
//        node->height = 1 + std::max(nodeHeight(node->left), nodeHeight(node->right));
//        child->height = 1 + std::max(nodeHeight(child->left), nodeHeight(child->right));
//    }
//
    void roll(Node<T>* node, bool isLL) {
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

        node->height = 1 + std::max(nodeHeight(node->left), nodeHeight(node->right));
        child->height = 1 + std::max(nodeHeight(child->left), nodeHeight(child->right));
    }

    void rollLL(Node<T>* node) {
        roll(node, true);
    }

    void rollRR(Node<T>* node) {
        roll(node, false);
    }

    void rollLR(Node<T>* node) {
        this->rollRR(node->left);
        this->rollLL(node);
    }

    void rollRL(Node<T>* node) {
        this->rollLL(node->right);
        this->rollRR(node);
    }

//    Node<T>* createNode(const T& data, Node<T>* parent) override {
//        return new AVLNode<T>(data, parent);
//    }

public:

    Node<T>* insert(const T& data) override {

        Node<T>* insertedNode = BinarySearchTree<T>::insert(data);;
        Node<T>* v = insertedNode;

        while (v != this->root) {
            Node<T>* p = v->parent;
            if (p->height >= v->height + 1) {
                break;
            }
            p->height = v->height + 1;
            int bf = p->bf();
            if (bf == 2) {
                if (v->left->bf() == -1) {
                    rollLR(p);
                } else {
                    rollLL(p);
                }
                break;
            } else if (bf == -2) {
                if (v->right->bf() == 1) {
                    rollRL(p);
                } else {
                    rollRR(p);
                }
                break;
            }
            v = p;
        }
        return insertedNode;
    }


    Node<T>* remove(Node<T>* nodeToRemove) override {
        Node<T>* parent = BinarySearchTree<T>::remove(nodeToRemove);
        Node<T>* v = parent;
        while (v != nullptr) {
            int oldHeight = v->height;
            v->height = 1 + std::max(nodeHeight(v->left), nodeHeight(v->right));
            int bf = v->bf();
            if (bf == 2) {
                if (v->left->bf() == -1) {
                    rollLR(v);
                } else {
                    rollLL(v);
                }
            } else if (bf == -2) {
                if (v->right->bf() == 1) {
                    rollRL(v);
                } else {
                    rollRR(v);
                }
            }

            if (v->height == oldHeight) {
                break;
            }

            v = v->parent;
        }
        return parent;
    }

};


#endif
