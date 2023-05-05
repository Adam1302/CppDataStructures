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
class BinarySearchTree {
    struct BSTNode {
        T data;
        BSTNode* left;
        BSTNode* right;
    };

    BSTNode* root;
    size_t nodeCount;

        BSTNode* insert(BSTNode* node, const T& elem);
        std::ostream& printInOrder(std::ostream& out, const BSTNode* node);
        std::ostream& printPreOrder(std::ostream& out, const BSTNode* node);
        std::ostream& printPostOrder(std::ostream& out, const BSTNode* node);
        void inOrderVector(std::vector<T>& vec, const BSTNode* node);
        void preOrderVector(std::vector<T>& vec, const BSTNode* node);
        void postOrderVector(std::vector<T>& vec, const BSTNode* node);
        BSTNode* remove(BSTNode* node, const T& elem);
        int height(BSTNode* node) const;
        void clear(BSTNode* node);
        BSTNode* deepCopy(BSTNode* other);
    public:
        BinarySearchTree();
        BinarySearchTree(const BinarySearchTree& other);
        BinarySearchTree(BinarySearchTree&& other);
        BinarySearchTree& operator=(const BinarySearchTree& other);
        BinarySearchTree& operator=(BinarySearchTree&& other);
        void insert(const T& elem);
        void insert(T&& elem);
        bool search(const T& elem);
        bool search(T&& elem);
        void remove(const T& elem);
        void remove(T&& elem);
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
        friend std::ostream& operator<<(std::ostream& out, const BinarySearchTree<U>& bst);
        int height() const;
        ~BinarySearchTree();
};

template <typename T>
typename BinarySearchTree<T>::BSTNode* BinarySearchTree<T>::insert(BSTNode* node, const T& elem) {
    if (!node) {
        BSTNode* newNode = (BSTNode*) malloc(sizeof(BSTNode));
        newNode->data = std::move(elem);
        newNode->left = newNode->right = nullptr;
        ++nodeCount;
        return newNode;
    }

    if (elem < node->data) {
        node->left = insert(node->left, elem);
    } else if (elem > node->data) {
        node->right = insert(node->right, elem);
    }

    return node;
}

template <typename T>
std::ostream& BinarySearchTree<T>::printInOrder(std::ostream& out, const BSTNode* node) {
    if (node) {
        printInOrder(out, node->left);
        out << "" << node->data << " ";
        printInOrder(out, node->right);
    }

    return out;
}

template <typename T>
std::ostream& BinarySearchTree<T>::printPreOrder(std::ostream& out, const BSTNode* node) {
    if (node) {
        out << "" << node->data << " ";
        printInOrder(out, node->left);
        printInOrder(out, node->right);
    }

    return out;
}

template <typename T>
std::ostream& BinarySearchTree<T>::printPostOrder(std::ostream& out, const BSTNode* node) {
    if (node) {
        printInOrder(out, node->left);
        printInOrder(out, node->right);
        out << "" << node->data << " ";
    }

    return out;
}

template <typename T>
void BinarySearchTree<T>::inOrderVector(std::vector<T>& vec, const BSTNode* node) {
    if (node) {
        inOrderVector(vec, node->left);
        vec.push_back(std::move(node->data));
        inOrderVector(vec, node->right);
    }
}

template <typename T>
void BinarySearchTree<T>::preOrderVector(std::vector<T>& vec, const BSTNode* node) {
    if (node) {
        vec.push_back(std::move(node->data));
        inOrderVector(vec, node->left);
        inOrderVector(vec, node->right);
    }
}

template <typename T>
void BinarySearchTree<T>::postOrderVector(std::vector<T>& vec, const BSTNode* node) {
    if (node) {
        inOrderVector(vec, node->left);
        inOrderVector(vec, node->right);
        vec.push_back(std::move(node->data));
    }
}

template <typename T>
typename BinarySearchTree<T>::BSTNode* BinarySearchTree<T>::remove(BSTNode* node, const T& elem) {
    if (node) {
        if (elem < node->data) node->left = remove(node->left, elem);
        else if (elem > node->data) node->right = remove(node->right, elem);
        else {
            if (!node->left && !node->right) {
                --nodeCount;
                delete node;
                return nullptr;
            } else if (!node->right) {
                --nodeCount;
                BSTNode* toBeDeleted = node;
                node = node->left;
                delete toBeDeleted;
            } else if (!node->left) {
                --nodeCount;
                BSTNode* toBeDeleted = node;
                node = node->right;
                delete toBeDeleted;
            } else {
                BSTNode* leftSubtreeMax = node->left;
                while (leftSubtreeMax->right) leftSubtreeMax = leftSubtreeMax->right;

                node->data = leftSubtreeMax->data;
                node->left = remove(node->left, leftSubtreeMax->data);
            }
        }
    }

    return node;
}

template <typename T>
int BinarySearchTree<T>::height(BSTNode* node) const {
    if (!node) return 0;
    return 1 + std::max(height(node->left), height(node->right));
}

template <typename T>
void BinarySearchTree<T>::clear(BSTNode* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template <typename T>
typename BinarySearchTree<T>::BSTNode* BinarySearchTree<T>::deepCopy(BSTNode* other) {
    if (!other) {
        return nullptr;
    }
    return new BSTNode{
        other->data,
        deepCopy(other->left),
        deepCopy(other->right)
    };
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree() : root{nullptr}, nodeCount{0} {}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree& other) : root{deepCopy(other.root)}, nodeCount{other.count()} {}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree&& other) : root{other.root}, nodeCount{other.count()} {
    other.root = nullptr;
}

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree& other) {
    nodeCount = other.nodeCount;
    clear();
    deepCopy(other.root);
}

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree&& other) {
    std::swap(root, other.root);
    std::swap(nodeCount, other.nodeCount);

    return *this;
}

template <typename T>
void BinarySearchTree<T>::insert(const T& elem) {
    if (!root) {
        root = insert(nullptr, elem);
    }

    insert(root, elem);
}

template <typename T>
void BinarySearchTree<T>::insert(T&& elem) {
    if (!root) {
        root = insert(nullptr, elem);
    }

    insert(root, elem);
}

template <typename T>
bool BinarySearchTree<T>::search(const T& elem) {
    BSTNode* curr = root;

    while (curr) {
        if (curr->data > elem) curr = curr->left;
        else if (curr->data == elem) return true;
        else  curr = curr->right;
    }

    return false;
}

template <typename T>
bool BinarySearchTree<T>::search(T&& elem) {
    BSTNode* curr = root;

    while (curr) {
        if (curr->data > elem) curr = curr->left;
        else if (curr->data == elem) return true;
        else  curr = curr->right;
    }

    return false;
}

template <typename T>
void BinarySearchTree<T>::remove(const T& elem) {
    root = remove(root, elem);
}

template <typename T>
void BinarySearchTree<T>::remove(T&& elem) {
    root = remove(root, elem);
}

template <typename T>
int BinarySearchTree<T>::depth(const T& elem) const {
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
int BinarySearchTree<T>::depth(T&& elem) const {
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
const T& BinarySearchTree<T>::min() const {
    BSTNode* curr = root;
    if (!curr) throw std::out_of_range("Empty array");

    while (curr->left) curr = curr->left;

    return curr->data;
}

template <typename T>
const T& BinarySearchTree<T>::max() const {
    BSTNode* curr = root;
    if (!curr) throw std::out_of_range("Empty array");

    while (curr->right) curr = curr->right;

    return curr->data;
}

template <typename T>
std::ostream& BinarySearchTree<T>::printInOrder(std::ostream& out) {
    out << "{ ";
    printInOrder(out, root);
    out << "}" << "\n";
    return out;
}

template <typename T>
std::ostream& BinarySearchTree<T>::printPreOrder(std::ostream& out) {
    out << "{ ";
    printPreOrder(out, root);
    out << "}" << "\n";
    return out;
}

template <typename T>
std::ostream& BinarySearchTree<T>::printPostOrder(std::ostream& out) {
    out << "{ ";
    printPostOrder(out, root);
    out << "}" << "\n";
    return out;
}

template <typename T>
std::ostream& BinarySearchTree<T>::printLevelOrder(std::ostream& out) {
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
std::vector<T> BinarySearchTree<T>::toInOrderVector() {
    std::vector<T> vec;
    inOrderVector(vec, root);
}

template <typename T>
std::vector<T> BinarySearchTree<T>::toPreOrderVector() {
    std::vector<T> vec;
    preOrderVector(vec, root);
}

template <typename T>
std::vector<T> BinarySearchTree<T>::toPostOrderVector() {
    std::vector<T> vec;
    postOrderVector(vec, root);
}

template <typename T>
std::vector<T> BinarySearchTree<T>::toLevelOrderVector() {
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
constexpr size_t BinarySearchTree<T>::count() const { return nodeCount; }

template <typename T>
std::ostream& operator<<(std::ostream& out, const BinarySearchTree<T>& bst) {
    bst.printInOrder(out);
}

template <typename T>
int BinarySearchTree<T>::height() const {
    return height(root) - 1;
}

template <typename T>
BinarySearchTree<T>::~BinarySearchTree() {
    clear(root);
}

void testBST() {
    BinarySearchTree<int> myTree;
    LOG("HEIGHT: " + std::to_string(myTree.height()))
    myTree.insert(22);
    LOG("HEIGHT: " + std::to_string(myTree.height()))
    myTree.insert(10);
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
    LOG("DEPTH OF 5: " + std::to_string(myTree.depth(5)))

    myTree.printInOrder(std::cout);
    myTree.printPreOrder(std::cout);
    myTree.printPostOrder(std::cout);
    myTree.printLevelOrder(std::cout);
    std::cout << std::endl;

    myTree.remove(7);
    myTree.printInOrder(std::cout);
    myTree.printPreOrder(std::cout);
    myTree.printPostOrder(std::cout);
    myTree.printLevelOrder(std::cout);
    std::cout << std::endl;

    myTree.remove(2);
    myTree.printInOrder(std::cout);
    myTree.printPreOrder(std::cout);
    myTree.printPostOrder(std::cout);
    myTree.printLevelOrder(std::cout);
    std::cout << std::endl;
}

int main() {
    testBST();
}
