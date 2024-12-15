#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "Node.h"
//
//
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <queue>

template<class T>
class BinaryTree {

public:
    Node<T>* root;

    BinaryTree();

    virtual ~BinaryTree();

    bool isEmpty() const;

    BinaryTree(const BinaryTree&) = delete;

    BinaryTree& operator=(const BinaryTree&) = delete;

    void print() const {
        if (!root) {
            std::cout << "(empty tree)" << std::endl;
            return;
        }

        struct PrintNode {
            Node<T>* node;
            int position;
        };

        std::queue<PrintNode> q;
        q.push({root, 40}); // Center the root node

        std::vector<std::string> levels;
        std::vector<std::string> connectors;
        int nodeSpacing = 4; // Space between nodes

        while (!q.empty()) {
            int levelSize = q.size();
            std::string line, connectorLine;

            for (int i = 0; i < levelSize; ++i) {
                PrintNode current = q.front();
                q.pop();

                if (current.node) {
                    size_t pos = static_cast<size_t>(current.position); // Cast to unsigned

                    // Add spaces for alignment
                    if (line.size() < pos) {
                        line.append(pos - line.size(), ' ');
                    }

                    line += std::to_string(current.node->data);

                    // Add connectors
                    if (current.node->left || current.node->right) {
                        if (connectorLine.size() < pos) {
                            connectorLine.append(pos - connectorLine.size(), ' ');
                        }
                        if (current.node->left) {
                            connectorLine += "/";
                        } else {
                            connectorLine += " ";
                        }
                        if (current.node->right) {
                            connectorLine += "\\";
                        } else {
                            connectorLine += " ";
                        }
                    }

                    q.push({current.node->left, current.position - nodeSpacing});
                    q.push({current.node->right, current.position + nodeSpacing});
                }
            }

            levels.push_back(line);
            if (!connectorLine.empty()) {
                connectors.push_back(connectorLine);
            }
        }

        // Print levels and connectors
        for (size_t i = 0; i < levels.size(); ++i) {
            std::cout << levels[i] << std::endl;
            if (i < connectors.size()) {
                std::cout << connectors[i] << std::endl;
            }
        }
    }

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


#endif
