// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>
#include <deque>

template<typename K, typename V>
class SkipList {
    struct SLNode {
        K* key;
        V* value;
        SLNode* next;
        SLNode* below;
    };

    SLNode* startingSentinel;
    int kvPairCount;
    int levels;

    mutable std::deque<SLNode*> predecessorStack;

        // This function returns a stack of the predecessor node at each level
        void getPredecessors(const K& key) const;
        void deleteSLNode(SLNode* node);
        void clear();
    public:
        SkipList();
        SkipList(const SkipList& other);
        SkipList(SkipList&& other);
        SkipList& operator=(const SkipList& other);
        SkipList& operator=(SkipList&& other);
        constexpr int height() const;
        constexpr int count() const;
        void insert(const K& key, const V& value);
        void remove(const K& key);
        bool search(const K& key);
        bool search(K&& key);
        template <typename X, typename Y>
        friend std::ostream& operator<<(std::ostream& out, const SkipList<X, Y>& sl);
        ~SkipList();
};

template<typename K, typename V>
void SkipList<K, V>::getPredecessors(const K& key) const {
    predecessorStack = {};

    SLNode* traverser = startingSentinel;
    predecessorStack.push_front(traverser);

    while (traverser->below) {
        traverser = traverser->below;
        while (traverser->next && *(traverser->next->key) < key) {
            traverser = traverser->next;
        }
        predecessorStack.push_front(traverser);
    }
}

template<typename K, typename V>
void SkipList<K, V>::deleteSLNode(SLNode* node) {
    if (!node->below) {
        delete node->key;
        if (node->value) delete node->value;
    }
    delete node;
}

template<typename K, typename V>
void SkipList<K, V>::clear() {
    SLNode* listTraverser = startingSentinel;
    SLNode* nodeTraverser = nullptr;
    SLNode* toBeDeleted = nullptr;
    while (listTraverser) {
        nodeTraverser = listTraverser;
        listTraverser = listTraverser->below;
        while (nodeTraverser) {
            toBeDeleted = nodeTraverser;
            nodeTraverser = nodeTraverser->next;
            deleteSLNode(toBeDeleted);
        }
    }
}

template<typename K, typename V>
SkipList<K, V>::SkipList() : startingSentinel{nullptr}, kvPairCount{0}, levels{0} {
    startingSentinel = new SLNode{
        new K{std::numeric_limits<K>::min()},
        nullptr, nullptr, nullptr
    };
}

template<typename K, typename V>
SkipList<K, V>::SkipList(const SkipList& other) : startingSentinel{nullptr}, kvPairCount{other.kvPairCount}, levels{other.levels} {
    SLNode* otherTraverser = other.startingSentinel;
    SLNode* tempNode = nullptr;
    SLNode* currTraverser = new SLNode{
        new K{std::numeric_limits<K>::min()},
        nullptr, nullptr, nullptr
    };
    startingSentinel = currTraverser;

    for (int i = 0; i < levels; ++i) {
        tempNode = startingSentinel;
        startingSentinel = new SLNode{
            tempNode->key,
            nullptr, nullptr,
            tempNode
        };
    }

    while (otherTraverser->below) otherTraverser = otherTraverser->below;

    while (otherTraverser->next) {
        otherTraverser = otherTraverser->next;

        other.getPredecessors(*(otherTraverser->key));
        getPredecessors(*(otherTraverser->key));

        SLNode* otherNode = other.predecessorStack.front();
        SLNode* thisNode = predecessorStack.front();
        other.predecessorStack.pop_front();
        predecessorStack.pop_front();

        currTraverser->next = new SLNode{
            new K{*(otherTraverser->key)},
            new V{*(otherTraverser->value)},
            nullptr,
            nullptr
        };
        currTraverser = currTraverser->next;
        SLNode* bottomNode = currTraverser;

        while (!other.predecessorStack.empty()) {
            otherNode = other.predecessorStack.front();
            thisNode = predecessorStack.front();
            other.predecessorStack.pop_front();
            predecessorStack.pop_front();

            LOG(*(thisNode->key))

            if (otherNode->next && *(otherNode->next->key) == *(otherTraverser->key)) {
                thisNode->next = new SLNode{
                    currTraverser->key,
                    nullptr,
                    nullptr,
                    bottomNode
                };
                bottomNode = thisNode;
            } else {
                break;
            }
        }
    }
}

template<typename K, typename V>
SkipList<K, V>::SkipList(SkipList&& other) : startingSentinel{other.startingSentinel}, kvPairCount{other.kvPairCount}, levels{other.levels} {
    other.startingSentinel = nullptr;
}

template<typename K, typename V>
SkipList<K, V>& SkipList<K, V>::operator=(const SkipList& other) {
    clear();

    kvPairCount = other.kvPairCount;
    levels = other.levels;

    SLNode* otherTraverser = other.startingSentinel;
    SLNode* tempNode = nullptr;
    SLNode* currTraverser = new SLNode{
        new K{std::numeric_limits<K>::min()},
        nullptr, nullptr, nullptr
    };
    startingSentinel = currTraverser;

    for (int i = 0; i < levels; ++i) {
        tempNode = startingSentinel;
        startingSentinel = new SLNode{
            tempNode->key,
            nullptr, nullptr,
            tempNode
        };
    }

    while (otherTraverser->below) otherTraverser = otherTraverser->below;

    while (otherTraverser->next) {
        otherTraverser = otherTraverser->next;

        other.getPredecessors(*(otherTraverser->key));
        getPredecessors(*(otherTraverser->key));

        SLNode* otherNode = other.predecessorStack.front();
        SLNode* thisNode = predecessorStack.front();
        other.predecessorStack.pop_front();
        predecessorStack.pop_front();

        currTraverser->next = new SLNode{
            new K{*(otherTraverser->key)},
            new V{*(otherTraverser->value)},
            nullptr,
            nullptr
        };
        currTraverser = currTraverser->next;
        SLNode* bottomNode = currTraverser;

        while (!other.predecessorStack.empty()) {
            otherNode = other.predecessorStack.front();
            thisNode = predecessorStack.front();
            other.predecessorStack.pop_front();
            predecessorStack.pop_front();

            if (otherNode->next && *(otherNode->next->key) == *(otherTraverser->key)) {
                thisNode->next = new SLNode{
                    currTraverser->key,
                    nullptr,
                    nullptr,
                    bottomNode
                };
                bottomNode = thisNode;
            } else {
                break;
            }
        }
    }

    return *this;
}

template<typename K, typename V>
SkipList<K, V>& SkipList<K, V>::operator=(SkipList&& other) {
    std::swap(startingSentinel, other.startingSentinel);
    std::swap(kvPairCount, other.kvPairCount);
    std::swap(levels, other.levels);

    return *this;
}

template<typename K, typename V>
constexpr int SkipList<K, V>::height() const { return levels; }

template<typename K, typename V>
constexpr int SkipList<K, V>::count() const { return kvPairCount; }

template<typename K, typename V>
void SkipList<K, V>::insert(const K& key, const V& value) {
    getPredecessors(key);

    int i = 1;
    while ((double) rand()/RAND_MAX >= 0.5) {
        ++i;
    }

    SLNode* item = nullptr;
    while (i >= predecessorStack.size()) {
        item = new SLNode{
            startingSentinel->key,
            nullptr, nullptr,
            startingSentinel
        };
        startingSentinel = item;

        predecessorStack.push_back(item);

        ++levels;
    }

    SLNode* bottomItem = predecessorStack.front();
    predecessorStack.pop_front();

    SLNode* bottomNext = new SLNode{
        new K{key},
        new V{value},
        bottomItem->next,
        nullptr
    };
    bottomItem->next = bottomNext;

    SLNode* beingAdded = nullptr;
    while (i > 0) {
        SLNode* bottomItem = predecessorStack.front();
        predecessorStack.pop_front();

        beingAdded = new SLNode{
            bottomNext->key,
            bottomNext->value,
            bottomItem->next,
            bottomNext
        };
        bottomItem->next = beingAdded;
        bottomNext = beingAdded;

        --i;
    }

    ++kvPairCount;
}

template<typename K, typename V>
void SkipList<K, V>::remove(const K& key) {
    getPredecessors(key);

    SLNode* node = nullptr;
    SLNode* toBeDeleted = nullptr;
    while (!predecessorStack.empty()) {
        node = predecessorStack.front();
        predecessorStack.pop_front();

        if (node->next && *(node->next->key) == key) {
            toBeDeleted = node->next;
            node->next = node->next->next;
            deleteSLNode(toBeDeleted);
        } else {
            break;
        }
    }
    --kvPairCount;

    SLNode* root = startingSentinel;
    while (root->below && !root->below->next) {
        toBeDeleted = root->below;
        root->below = root->below->below;
        delete toBeDeleted;
        --levels;
    }
}

template<typename K, typename V>
bool SkipList<K, V>::search(const K& key) {
    getPredecessors(key);
    SLNode* node = predecessorStack.front();
    if (node->next) return *(node->next->key) == key;
    return false;
}

template<typename K, typename V>
bool SkipList<K, V>::search(K&& key) {
    getPredecessors(key);
    SLNode* node = predecessorStack.front();
    if (node->next) return *(node->next->key) == key;
    return false;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const SkipList<K, V>& sl) {
    out << "---------------" << "\n";
    auto traverser = sl.startingSentinel;

    while (traverser->below) traverser = traverser->below;

    do {
        if (traverser->value) {
            out << *(traverser->key) << " : " << *(traverser->value) << "\n";
        }
        traverser = traverser->next;
    } while(traverser);

    out << "---------------" << "\n";

    return out;
}

template<typename K, typename V>
SkipList<K, V>::~SkipList() {
    clear();
}

template<typename K, typename V>
SkipList<K, V> getSkipList() {
    SkipList<int, std::string> celtics;

    celtics.insert(15, "Pierce");
    celtics.insert(4, "Garnett");
    celtics.insert(20, "Allen");
    celtics.insert(9, "Rondo");

    return celtics;
}

void testSkipList() {
    SkipList<int, std::string> raptors;

    raptors.insert(7, "Lowry");
    std::cout << raptors << std::endl;
    raptors.insert(43, "Siakam");
    std::cout << raptors << std::endl;
    raptors.insert(10, "DeRozan");
    std::cout << raptors << std::endl;
    raptors.insert(17, "Valanciunas");
    raptors.insert(8, "Calderon");
    LOG(raptors.height())
    LOG(raptors.count())
    raptors.insert(4, "Bosh");
    std::cout << raptors << std::endl;
    raptors.insert(15, "Johnson");
    std::cout << raptors << std::endl;
    raptors.insert(15, "Carter");
    std::cout << raptors << std::endl;
    raptors.insert(0, "Jack");
    std::cout << raptors << std::endl;
    raptors.insert(50, "Hansborough");
    std::cout << raptors << std::endl;

    raptors.remove(4);
    std::cout << raptors << std::endl;
    raptors.remove(0);
    std::cout << raptors << std::endl;
    raptors.remove(50);
    std::cout << raptors << std::endl;
    LOG(raptors.height())
    LOG(raptors.count())

/*
    SkipList<int, std::string> oldRaptors{raptors};
    std::cout << raptors << std::endl;
    std::cout << oldRaptors << std::endl;
    raptors.remove(8);
    std::cout << raptors << std::endl;
    std::cout << oldRaptors << std::endl;
    raptors.insert(4, "Barnes");
    std::cout << raptors << std::endl;
    std::cout << oldRaptors << std::endl;

    SkipList<int, std::string> newRaptors;
    newRaptors = raptors;
    raptors.insert(42, "Hollis-Jefferson");
    std::cout << raptors << std::endl;
    std::cout << newRaptors << std::endl;
    */

    SkipList<int, std::string> celtics{getSkipList<int, std::string>()};
    std::cout << celtics << std::endl;
    celtics.remove(9);
    std::cout << celtics << std::endl;

    celtics = getSkipList<int, std::string>();
    std::cout << celtics << std::endl;

}

int main() {
    testSkipList();
}
