// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>
#include <queue>

template<typename T>
class AVLTree {
    struct BSTNode {
        T data;
        int height;
        BSTNode* parent;
        BSTNode* left;
        BSTNode* right;
    };

    BSTNode* root;
    size_t nodeCount;
        // modify these three to updata the parents children and the parents
        BSTNode* rotateRight(BSTNode* z);
        BSTNode* rotateLeft(BSTNode* z);
        BSTNode* restructure(BSTNode* x, BSTNode* y, BSTNode* z);
        BSTNode* BSTinsert(const T& elem);
        std::ostream& printInOrder(std::ostream& out, const BSTNode* node);
        std::ostream& printPreOrder(std::ostream& out, const BSTNode* node);
        std::ostream& printPostOrder(std::ostream& out, const BSTNode* node);
        void inOrderVector(std::vector<T>& vec, const BSTNode* node);
        void preOrderVector(std::vector<T>& vec, const BSTNode* node);
        void postOrderVector(std::vector<T>& vec, const BSTNode* node);
        BSTNode* BSTremove(const T& elem);
        int height(BSTNode* node) const;
        void clear(BSTNode* node);
        BSTNode* deepCopy(BSTNode* other, BSTNode* parent);
        void setHeight(BSTNode* node);
    public:
        AVLTree();
        AVLTree(const AVLTree& other);
        AVLTree(AVLTree&& other);
        AVLTree& operator=(const AVLTree& other);
        AVLTree& operator=(AVLTree&& other);
        void insert(const T& elem);
        bool search(const T& elem);
        bool search(T&& elem);
        void remove(const T& elem);
        int depth(const T& elem) const;
        int depth(T&& elem) const;
        const T& min() const;
        const T& max() const;
        std::ostream& printInOrder(std::ostream& out);
        std::ostream& printPreOrder(std::ostream& out);
        std::ostream& printPostOrder(std::ostream& out);
        std::ostream& printLevelOrder(std::ostream& out);
        std::vector<T> toInOrderVector();
        std::vector<T> toPreOrderVector();
        std::vector<T> toPostOrderVector();
        std::vector<T> toLevelOrderVector();
        constexpr size_t count() const;
        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const AVLTree<U>& avlt);
        int height() const;
        ~AVLTree();
};

template <typename T>
typename AVLTree<T>::BSTNode* AVLTree<T>::rotateRight(BSTNode* z) {
    BSTNode* y = z->left;

    z->left = y->right;
    y->right = z;

    setHeight(z);
    setHeight(y);

    y->parent = z->parent;
    z->parent = y;

    return y;
}

template <typename T>
typename AVLTree<T>::BSTNode* AVLTree<T>::rotateLeft(BSTNode* z) {
    BSTNode* y = z->right;

    z->right = y->left;
    y->left = z;

    setHeight(z);
    setHeight(y);

    y->parent = z->parent;
    z->parent = y;

    return y;
}

template <typename T>
typename AVLTree<T>::BSTNode* AVLTree<T>::restructure(BSTNode* x, BSTNode* y, BSTNode* z) {
    BSTNode* node = nullptr;
    if (z->left == y && y->left == x) {
        node = rotateRight(z);
    } else if (z->left == y && y->right == x) {
        z->left = rotateLeft(y);
        node = rotateRight(z);
    } else if (z->right == y && y->right == x) {
        node = rotateLeft(z);
    } else {
        z->right = rotateRight(y);
        node = rotateLeft(z);
    }
    if (root == z) {
        root = node;
        node->parent = nullptr;
    } else {
        if (node->parent->left == z) {
            node->parent->left = node;
        } else if (node->parent->right == z) {
            node->parent->right = node;
        }
    }
    return node;
}

template <typename T>
typename AVLTree<T>::BSTNode* AVLTree<T>::BSTinsert(const T& elem) {
    if (!root) {
        root = new BSTNode{elem, 0, nullptr, nullptr, nullptr};
        return root;
    }
    BSTNode* node = root;
    while (true) {
        if (node->data == elem) {
            return node;
        } else if (node->data < elem) {
            if (!node->right) {
                node->right = new BSTNode{elem, 0, node, nullptr, nullptr};
                return node->right;
            } else {
                node = node->right;
            }
        } else { // elem < node->data
            if (!node->left) {
                node->left = new BSTNode{elem, 0, node, nullptr, nullptr};
            } else {
                node = node->left;
            }
        }
    }
}

template <typename T>
std::ostream& AVLTree<T>::printInOrder(std::ostream& out, const BSTNode* node) {
    if (node) {
        printInOrder(out, node->left);
        out << "" << node->data << " ";
        printInOrder(out, node->right);
    }

    return out;
}

template <typename T>
std::ostream& AVLTree<T>::printPreOrder(std::ostream& out, const BSTNode* node) {
    if (node) {
        out << "" << node->data << " ";
        printInOrder(out, node->left);
        printInOrder(out, node->right);
    }

    return out;
}

template <typename T>
std::ostream& AVLTree<T>::printPostOrder(std::ostream& out, const BSTNode* node) {
    if (node) {
        printInOrder(out, node->left);
        printInOrder(out, node->right);
        out << "" << node->data << " ";
    }

    return out;
}

template <typename T>
void AVLTree<T>::inOrderVector(std::vector<T>& vec, const BSTNode* node) {
    if (node) {
        inOrderVector(vec, node->left);
        vec.push_back(std::move(node->data));
        inOrderVector(vec, node->right);
    }
}

template <typename T>
void AVLTree<T>::preOrderVector(std::vector<T>& vec, const BSTNode* node) {
    if (node) {
        vec.push_back(std::move(node->data));
        inOrderVector(vec, node->left);
        inOrderVector(vec, node->right);
    }
}

template <typename T>
void AVLTree<T>::postOrderVector(std::vector<T>& vec, const BSTNode* node) {
    if (node) {
        inOrderVector(vec, node->left);
        inOrderVector(vec, node->right);
        vec.push_back(std::move(node->data));
    }
}

template <typename T>
typename AVLTree<T>::BSTNode* AVLTree<T>::BSTremove(const T& elem) {
    BSTNode* node = root;
    BSTNode* toBeDeleted = nullptr;

    while (node) {
        if (elem < node->data) node = node->left;
        else if (elem > node->data) node = node->right;
        else {
            if (!node->left && !node->right) {
                toBeDeleted = node;
                node = node->parent;

                if (toBeDeleted == root) {
                    root = nullptr;
                } else if (toBeDeleted == node->left) {
                    node->left = nullptr;
                } else {
                    node->right = nullptr;
                }
            } else if (!node->right) {
                toBeDeleted = node;
                node = node->parent;
                toBeDeleted->left->parent = node;

                if (toBeDeleted == root) {
                    root = toBeDeleted->left;
                } else if (toBeDeleted == node->left) {
                    node->left = toBeDeleted->left;
                } else {
                    node->right = toBeDeleted->left;
                }
            } else if (!node->left) {
                toBeDeleted = node;
                node = node->parent;
                toBeDeleted->right->parent = node;

                if (toBeDeleted == root) {
                    root = toBeDeleted->right;
                } else if (toBeDeleted == node->left) {
                    node->left = toBeDeleted->right;
                } else {
                    node->right = toBeDeleted->right;
                }
            } else {
                BSTNode* rightSubtreeMin = node->right;
                while (rightSubtreeMin->left) rightSubtreeMin = rightSubtreeMin->left;

                std::swap(node->data, rightSubtreeMin->data);

                node = rightSubtreeMin->parent;
                
                if (!rightSubtreeMin->right) {
                    toBeDeleted = rightSubtreeMin;
                    rightSubtreeMin = rightSubtreeMin->parent;

                    if (toBeDeleted == root) {
                        root = nullptr;
                    } else if (toBeDeleted == rightSubtreeMin->left) {
                        rightSubtreeMin->left = nullptr;
                    } else {
                        rightSubtreeMin->right = nullptr;
                    }
                } else {
                    toBeDeleted = rightSubtreeMin;
                    rightSubtreeMin = rightSubtreeMin->parent;
                    toBeDeleted->right->parent = rightSubtreeMin;

                    if (toBeDeleted == root) {
                        root = toBeDeleted->left;
                    } else if (toBeDeleted == rightSubtreeMin->left) {
                        rightSubtreeMin->left = toBeDeleted->right;
                    } else {
                        rightSubtreeMin->right = toBeDeleted->right;
                    }
                }
            }
            --nodeCount;
            break;
        }
    }

    delete(toBeDeleted);

    return node;
}

template <typename T>
int AVLTree<T>::height(BSTNode* node) const {
    if (!node) return 0;
    return 1 + std::max(height(node->left), height(node->right));
}

template <typename T>
void AVLTree<T>::clear(BSTNode* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template <typename T>
typename AVLTree<T>::BSTNode* AVLTree<T>::deepCopy(BSTNode* other, BSTNode* parent) {
    if (!other) {
        return nullptr;
    }
    BSTNode* node = new BSTNode{
        other->data,
        other->height,
        parent,
        nullptr,
        nullptr
    };

    if (!root) root = node;

    node->left = deepCopy(other->left, node);
    node->right = deepCopy(other->right, node);

    return node;
}

template <typename T>
void AVLTree<T>::setHeight(BSTNode* node) {
    if (node->left && node->right) {
        node->height = 1 + std::max(node->left->height, node->right->height);
    } else if (node->left) {
        node->height = 1 + node->left->height;
    } else if (node->right) {
        node->height = 1 + node->right->height;
    } else {
        node->height = 0;
    }
}

template <typename T>
AVLTree<T>::AVLTree() : root{nullptr}, nodeCount{0} {}

template <typename T>
AVLTree<T>::AVLTree(const AVLTree& other) : root{deepCopy(other.root, nullptr)}, nodeCount{other.count()} {}

template <typename T>
AVLTree<T>::AVLTree(AVLTree&& other) : root{other.root}, nodeCount{other.count()} {
    other.root = nullptr;
}

template <typename T>
AVLTree<T>& AVLTree<T>::operator=(const AVLTree& other) {
    nodeCount = other.nodeCount;
    clear(root);
    deepCopy(other.root, nullptr);

    return *this;
}

template <typename T>
AVLTree<T>& AVLTree<T>::operator=(AVLTree&& other) {
    std::swap(root, other.root);
    std::swap(nodeCount, other.nodeCount);

    return *this;
}

template <typename T>
void AVLTree<T>::insert(const T& elem) {
    BSTNode* addedNode = BSTinsert(elem);

    while(addedNode) {
        if (addedNode->left && addedNode->right && 
        abs(addedNode->left->height - addedNode->right->height) > 1) {
            BSTNode* y = nullptr;
            BSTNode* x = nullptr;
            if (addedNode->left->height >= addedNode->right->height) {
                y = addedNode->left;
            } else {
                y = addedNode->right;
            }
            if (y->left) {
                if (y->right) {
                    if (y->left->height >= y->right->height) {
                        x = y->left;
                    } else {
                        x = y->right;
                    }
                } else {
                    x = y->left;
                }
            } else {
                x = y->right;
            }

            addedNode = restructure(x, y, addedNode);
            break;
        } else if (addedNode->left && !addedNode->right && addedNode->left->height > 0) {
            BSTNode* y = addedNode->left;
            BSTNode* x = nullptr;
            
            if (y->left) {
                if (y->right) {
                    if (y->left->height >= y->right->height) {
                        x = y->left;
                    } else {
                        x = y->right;
                    }
                } else {
                    x = y->left;
                }
            } else {
                x = y->right;
            }

            addedNode = restructure(x, y, addedNode);
            break;
        } else if (addedNode->right && !addedNode->left && addedNode->right->height > 0) {
            BSTNode* y = addedNode->right;
            BSTNode* x = nullptr;
            
            if (y->left) {
                if (y->right) {
                    if (y->left->height >= y->right->height) {
                        x = y->left;
                    } else {
                        x = y->right;
                    }
                } else {
                    x = y->left;
                }
            } else {
                x = y->right;
            }

            addedNode = restructure(x, y, addedNode);
            break;
        }
        
        setHeight(addedNode);
        addedNode = addedNode->parent;
    }
}

template <typename T>
bool AVLTree<T>::search(const T& elem) {
    BSTNode* curr = root;

    while (curr) {
        if (curr->data > elem) curr = curr->left;
        else if (curr->data == elem) return true;
        else  curr = curr->right;
    }

    return false;
}

template <typename T>
bool AVLTree<T>::search(T&& elem) {
    BSTNode* curr = root;

    while (curr) {
        if (curr->data > elem) curr = curr->left;
        else if (curr->data == elem) return true;
        else  curr = curr->right;
    }

    return false;
}

template <typename T>
void AVLTree<T>::remove(const T& elem) {
    BSTNode* deletedNode = BSTremove(elem);

    while(deletedNode) {
        if (deletedNode->left && deletedNode->right && 
        abs(deletedNode->left->height - deletedNode->right->height) > 1) {
            BSTNode* y = nullptr;
            BSTNode* x = nullptr;
            if (deletedNode->left->height >= deletedNode->right->height) {
                y = deletedNode->left;
            } else {
                y = deletedNode->right;
            }
            if (y->left) {
                if (y->right) {
                    if (y->left->height >= y->right->height) {
                        x = y->left;
                    } else {
                        x = y->right;
                    }
                } else {
                    x = y->left;
                }
            } else {
                x = y->right;
            }

            deletedNode = restructure(x, y, deletedNode);
            break;
        } else if (deletedNode->left && !deletedNode->right && deletedNode->left->height > 0) {
            BSTNode* y = deletedNode->left;
            BSTNode* x = nullptr;

            if (y->left) {
                if (y->right) {
                    if (y->left->height >= y->right->height) {
                        x = y->left;
                    } else {
                        x = y->right;
                    }
                } else {
                    x = y->left;
                }
            } else {
                x = y->right;
            }

            deletedNode = restructure(x, y, deletedNode);
            break;
        } else if (deletedNode->right && !deletedNode->left && deletedNode->right->height > 0) {
            BSTNode* y = deletedNode->right;
            BSTNode* x = nullptr;

            if (y->left) {
                if (y->right) {
                    if (y->left->height >= y->right->height) {
                        x = y->left;
                    } else {
                        x = y->right;
                    }
                } else {
                    x = y->left;
                }
            } else {
                x = y->right;
            }

            deletedNode = restructure(x, y, deletedNode);
        }

        setHeight(deletedNode);
        deletedNode = deletedNode->parent;
    }
}

template <typename T>
int AVLTree<T>::depth(const T& elem) const {
    BSTNode* curr = root;
    int depth = 0;

    while (curr) {
        if (curr->data > elem) curr = curr->left;
        else if (curr->data == elem) return depth;
        else  curr = curr->right;
        ++depth;

    }

    return -1;
}

template <typename T>
int AVLTree<T>::depth(T&& elem) const {
    BSTNode* curr = root;
    int depth = 0;

    while (curr) {
        if (curr->data > elem) curr = curr->left;
        else if (curr->data == elem) return depth;
        else  curr = curr->right;
        ++depth;
    }

    return -1;
}

template <typename T>
const T& AVLTree<T>::min() const {
    BSTNode* curr = root;
    if (!curr) throw std::out_of_range("Empty array");

    while (curr->left) curr = curr->left;

    return curr->data;
}

template <typename T>
const T& AVLTree<T>::max() const {
    BSTNode* curr = root;
    if (!curr) throw std::out_of_range("Empty array");

    while (curr->right) curr = curr->right;

    return curr->data;
}

template <typename T>
std::ostream& AVLTree<T>::printInOrder(std::ostream& out) {
    out << "{ ";
    printInOrder(out, root);
    out << "}" << "\n";
    return out;
}

template <typename T>
std::ostream& AVLTree<T>::printPreOrder(std::ostream& out) {
    out << "{ ";
    printPreOrder(out, root);
    out << "}" << "\n";
    return out;
}

template <typename T>
std::ostream& AVLTree<T>::printPostOrder(std::ostream& out) {
    out << "{ ";
    printPostOrder(out, root);
    out << "}" << "\n";
    return out;
}

template <typename T>
std::ostream& AVLTree<T>::printLevelOrder(std::ostream& out) {
    out << "{";
    if (root) {
        BSTNode* curr = root;
        std::queue<BSTNode*> qu;
        qu.push(root);
        while (!qu.empty()) {
            curr = qu.front();
            qu.pop();
            out << curr->data;
            if (curr->left) qu.push(curr->left);
            if (curr->right) qu.push(curr->right);

            if (!qu.empty()) out << ", ";
        }
    }

    out << "}" << "\n";
    return out;
}

template <typename T>
std::vector<T> AVLTree<T>::toInOrderVector() {
    std::vector<T> vec;
    inOrderVector(vec, root);
}

template <typename T>
std::vector<T> AVLTree<T>::toPreOrderVector() {
    std::vector<T> vec;
    preOrderVector(vec, root);
}

template <typename T>
std::vector<T> AVLTree<T>::toPostOrderVector() {
    std::vector<T> vec;
    postOrderVector(vec, root);
}

template <typename T>
std::vector<T> AVLTree<T>::toLevelOrderVector() {
    if (!root) return {};

    std::vector<T> vec;

    BSTNode* curr = root;
    std::queue<BSTNode*> qu;
    qu.push(root);
    while (qu.front()) {
        curr = qu.front();
        qu.pop();
        vec.push_back(curr->data);
        if (curr->left) qu.push(curr->left);
        if (curr->right) qu.push(curr->right);    
    }

    return vec;
}

template <typename T>
constexpr size_t AVLTree<T>::count() const { return nodeCount; }

template <typename T>
std::ostream& operator<<(std::ostream& out, const AVLTree<T>& avlt) {
    out = avlt.printInOrder(out);
}

template <typename T>
int AVLTree<T>::height() const {
    return height(root) - 1;
}

template <typename T>
AVLTree<T>::~AVLTree() {
    clear(root);
}

template<typename T>
AVLTree<T> getAVLTree() {
    AVLTree<int> myTree;
    myTree.insert(10);
    myTree.insert(12);
    myTree.insert(14);
    return myTree;
}


void testAVL() {
    AVLTree<int> myTree;
    LOG("HEIGHT: " + std::to_string(myTree.height()))
    myTree.insert(10);
    LOG("HEIGHT: " + std::to_string(myTree.height()))
    myTree.insert(22);
    LOG("HEIGHT: " + std::to_string(myTree.height()))
    myTree.insert(31);
    LOG("COUNT: " + std::to_string(myTree.count()))
    myTree.insert(4);
    myTree.insert(14);
    myTree.insert(28);
    myTree.insert(37);
    myTree.insert(6);
    myTree.insert(13);
    myTree.insert(18);
    myTree.insert(46);
    myTree.insert(16);

    LOG(myTree.search(2))
    LOG(myTree.search(4))
    LOG("MIN: " + std::to_string(myTree.min()))
    LOG("MAX: " + std::to_string(myTree.max()))
    LOG("DEPTH OF 5: " + std::to_string(myTree.depth(5)));

    myTree.printInOrder(std::cout);
    myTree.printPreOrder(std::cout);
    myTree.printPostOrder(std::cout);
    myTree.printLevelOrder(std::cout);
    std::cout << std::endl;

    myTree.insert(8);
    myTree.printLevelOrder(std::cout);

    myTree.insert(45);
    myTree.printLevelOrder(std::cout);

    myTree.remove(45);
    myTree.printLevelOrder(std::cout);

    myTree.insert(2);
    myTree.printLevelOrder(std::cout);

    myTree.remove(22);
    myTree.printLevelOrder(std::cout);

    AVLTree<int> myTree2{myTree};

    AVLTree<int> myTree3;
    myTree3 = myTree;

    myTree2.insert(102);
    myTree3.insert(103);
    myTree.printLevelOrder(std::cout);
    myTree2.printLevelOrder(std::cout);
    myTree3.printLevelOrder(std::cout);

    AVLTree<int> myTree4{getAVLTree<int>()};
    AVLTree<int> myTree5;
    myTree5 = getAVLTree<int>();
    myTree4.insert(104);
    myTree5.insert(105);

    myTree4.printLevelOrder(std::cout);
    myTree5.printLevelOrder(std::cout);
}

int main() {
    testAVL();
}
