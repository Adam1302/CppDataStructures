// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>

template<typename K, typename V>
class Map {
        struct MapNode {
        K key;
        V value;
        int height;
        MapNode* parent;
        MapNode* left;
        MapNode* right;
    };

    MapNode* root;
    size_t nodeCount;
        // modify these three to updata the parents children and the parents
        MapNode* rotateRight(MapNode* z);
        MapNode* rotateLeft(MapNode* z);
        MapNode* restructure(MapNode* x, MapNode* y, MapNode* z);
        MapNode* mapInsert(const K& key, const V& value);
        std::ostream& printInOrder(std::ostream& out, const MapNode* node) const;
        void inOrderVector(std::vector<std::pair<K, V>>& vec, const MapNode* node);
        MapNode* mapRemove(const K& elem);
        int height(MapNode* node) const;
        void clear(MapNode* node);
        MapNode* deepCopy(MapNode* other, MapNode* parent);
        void setHeight(MapNode* node);
        MapNode* get(const K& key);
    public:
        Map();
        Map(const Map& other);
        Map(Map&& other);
        Map& operator=(const Map& other);
        Map& operator=(Map&& other);
        void insert(const K& key, const V& value);
        V& operator[](const K& key);
        const V& operator[](const K& key) const;
        V& at(const K& key);
        const V& at(const K& key) const;
        bool search(const K& elem);
        bool search(K&& elem);
        void remove(const K& elem);
        const std::pair<K&, V&> minKey() const;
        const std::pair<K&, V&> maxKey() const;
        void printInOrder(std::ostream& out) const;
        std::vector< std::pair<K, V> > toOrderedVector();
        constexpr size_t size() const;
        constexpr bool empty() const;
        template <typename X, typename Y>
        friend std::ostream& operator<<(std::ostream& out, const Map<X, Y>& mp);
        int height() const;
        ~Map();
};

template<typename K, typename V>
typename Map<K, V>::MapNode* Map<K, V>::rotateRight(MapNode* z) {
    MapNode* y = z->left;

    z->left = y->right;
    y->right = z;

    setHeight(z);
    setHeight(y);

    y->parent = z->parent;
    z->parent = y;

    return y;
}

template<typename K, typename V>
typename Map<K, V>::MapNode* Map<K, V>::rotateLeft(MapNode* z) {
    MapNode* y = z->right;

    z->right = y->left;
    y->left = z;

    setHeight(z);
    setHeight(y);

    y->parent = z->parent;
    z->parent = y;

    return y;
}

template<typename K, typename V>
typename Map<K, V>::MapNode* Map<K, V>::restructure(MapNode* x, MapNode* y, MapNode* z) {
    MapNode* node = nullptr;
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

template<typename K, typename V>
typename Map<K, V>::MapNode* Map<K, V>::mapInsert(const K& key, const V& value) {
    if (!root) {
        root = new MapNode{key, value, 0, nullptr, nullptr, nullptr};
        ++nodeCount;
        return root;
    }
    MapNode* node = root;
    while (true) {
        if (node->key == key) {
            return node;
        } else if (node->key < key) {
            if (!node->right) {
                node->right = new MapNode{key, value, 0, node, nullptr, nullptr};
                ++nodeCount;
                return node->right;
            } else {
                node = node->right;
            }
        } else { // key < node->data
            if (!node->left) {
                node->left = new MapNode{key, value, 0, node, nullptr, nullptr};
                ++nodeCount;
            } else {
                node = node->left;
            }
        }
    }
}

template<typename K, typename V>
std::ostream& Map<K, V>::printInOrder(std::ostream& out, const MapNode* node) const {
    if (node) {
        printInOrder(out, node->left);
        out << "{" << node->key << ", " << node->value << "} ";
        printInOrder(out, node->right);
    }

    return out;
}

template<typename K, typename V>
void Map<K, V>::inOrderVector(std::vector<std::pair<K, V>>& vec, const MapNode* node) {
    if (node) {
        inOrderVector(vec, node->left);
        vec.push_back(std::move(std::make_pair<K, V>(node->key, node->value)));
        inOrderVector(vec, node->right);
    }
}

template<typename K, typename V>
typename Map<K, V>::MapNode* Map<K, V>::mapRemove(const K& elem) {
    MapNode* node = root;
    MapNode* toBeDeleted = nullptr;

    while (node) {
        if (elem < node->key) node = node->left;
        else if (elem > node->key) node = node->right;
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
                MapNode* rightSubtreeMin = node->right;
                while (rightSubtreeMin->left) rightSubtreeMin = rightSubtreeMin->left;

                std::swap(node->key, rightSubtreeMin->key);
                std::swap(node->value, rightSubtreeMin->value);

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
            break;
        }
    }

    delete(toBeDeleted);
    --nodeCount;

    return node;
}

template<typename K, typename V>
int Map<K, V>::height(MapNode* node) const {
    if (!node) return 0;
    return 1 + std::max(height(node->left), height(node->right));
}

template<typename K, typename V>
void Map<K, V>::clear(MapNode* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

template<typename K, typename V>
typename Map<K, V>::MapNode* Map<K, V>::deepCopy(MapNode* other, MapNode* parent) {
    if (!other) {
        return nullptr;
    }
    MapNode* node = new MapNode{
        other->key,
        other->value,
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

template<typename K, typename V>
void Map<K, V>::setHeight(MapNode* node) {
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

template<typename K, typename V>
typename Map<K, V>::MapNode* Map<K, V>::get(const K& key) {
    MapNode* curr = root;

    while (curr) {
        if (curr->key > key) curr = curr->left;
        else if (curr->key == key) break;
        else  curr = curr->right;
    }

    return curr;
}

template<typename K, typename V>
Map<K, V>::Map() : root{nullptr}, nodeCount{0} {}

template<typename K, typename V>
Map<K, V>::Map(const Map& other) : root{deepCopy(other.root, nullptr)}, nodeCount{other.count()} {}

template<typename K, typename V>
Map<K, V>::Map(Map&& other) : root{other.root}, nodeCount{other.count()} {
    other.root = nullptr;
}

template<typename K, typename V>
Map<K, V>& Map<K, V>::operator=(const Map& other) {
    nodeCount = other.nodeCount;
    clear(root);
    deepCopy(other.root, nullptr);

    return *this;
}

template<typename K, typename V>
Map<K, V>& Map<K, V>::operator=(Map&& other) {
    std::swap(root, other.root);
    std::swap(nodeCount, other.nodeCount);

    return *this;
}

template<typename K, typename V>
void Map<K, V>::insert(const K& key, const V& value) {
    MapNode* addedNode = mapInsert(key, value);

    while(addedNode) {
        if (addedNode->left && addedNode->right && 
        abs(addedNode->left->height - addedNode->right->height) > 1) {
            MapNode* y = nullptr;
            MapNode* x = nullptr;
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
            MapNode* y = addedNode->left;
            MapNode* x = nullptr;
            
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
            MapNode* y = addedNode->right;
            MapNode* x = nullptr;
            
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

template<typename K, typename V>
V& Map<K, V>::operator[](const K& key) {
    MapNode* node = get(key);

    if (!node) {
        insert(key, V{});
        node = get(key);
    }

    return node->value;
}

template<typename K, typename V>
const V& Map<K, V>::operator[](const K& key) const {
    return get(key)->value;
}

template<typename K, typename V>
V& Map<K, V>::at(const K& key) {
    MapNode* node = get(key);

    if (!node) {
        insert(key, V{});
        node = get(key);
    }

    return node->value;
}

template<typename K, typename V>
const V& Map<K, V>::at(const K& key) const {
    MapNode* node = get(key);

    if (!node) throw std::invalid_argument("No such node exists");

    return node->value;
}

template<typename K, typename V>
bool Map<K, V>::search(const K& elem) {
    MapNode* curr = root;

    while (curr) {
        if (curr->key > elem) curr = curr->left;
        else if (curr->key == elem) return true;
        else  curr = curr->right;
    }

    return false;
}

template<typename K, typename V>
bool Map<K, V>::search(K&& elem) {
    MapNode* curr = root;

    while (curr) {
        if (curr->key > elem) curr = curr->left;
        else if (curr->key == elem) return true;
        else  curr = curr->right;
    }

    return false;
}

template<typename K, typename V>
void Map<K, V>::remove(const K& elem) {
    MapNode* deletedNode = mapRemove(elem);

    while(deletedNode) {
        if (deletedNode->left && deletedNode->right && 
        abs(deletedNode->left->height - deletedNode->right->height) > 1) {
            MapNode* y = nullptr;
            MapNode* x = nullptr;
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
            MapNode* y = deletedNode->left;
            MapNode* x = nullptr;

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
            MapNode* y = deletedNode->right;
            MapNode* x = nullptr;

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

template<typename K, typename V>
const std::pair<K&, V&> Map<K, V>::minKey() const {
    MapNode* curr = root;
    if (!curr) throw std::out_of_range("Empty array");

    while (curr->left) curr = curr->left;

    return std::make_pair<K&, V&>(curr->key, curr->value);
}

template<typename K, typename V>
const std::pair<K&, V&> Map<K, V>::maxKey() const {
    MapNode* curr = root;
    if (!curr) throw std::out_of_range("Empty array");

    while (curr->right) curr = curr->right;

    return std::make_pair<K&, V&>(curr->key, curr->value);
}

template<typename K, typename V>
void Map<K, V>::printInOrder(std::ostream& out) const {
    out << "{ ";
    printInOrder(out, root);
    out << "}";
}

template<typename K, typename V>
std::vector< std::pair<K, V> > Map<K, V>::toOrderedVector() {
    std::vector< std::pair<K, V> > vec;
    inOrderVector(vec, root);
}

template<typename K, typename V>
constexpr size_t Map<K, V>::size() const { return nodeCount; }

template<typename K, typename V>
constexpr bool Map<K, V>::empty() const { return nodeCount == 0; }

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const Map<K, V>& mp) {
    mp.printInOrder(out);
    return out;
}

template<typename K, typename V>
int Map<K, V>::height() const {
    return height(root) - 1;
}

template<typename K, typename V>
Map<K, V>::~Map() {
    clear(root);
}


void testMap() {
    Map<int, std::string> raptors;

    LOG(raptors.empty())

    raptors.insert(43, "Siakam");
    std::cout << raptors << std::endl;
    raptors.insert(7, "Lowry");
    std::cout << raptors << std::endl;
    raptors.insert(10, "Derozan");
    std::cout << raptors << std::endl;
    raptors[15] = "Johnson";
    std::cout << raptors << std::endl;
    raptors[15] = "Carter";
    std::cout << raptors << std::endl;
    std::cout << raptors[15] << std::endl;
    LOG(raptors.size())

    raptors.remove(15);
    std::cout << raptors << std::endl;
    LOG(raptors.size())
    LOG(raptors.empty())
}

int main() {
    testMap();
}
