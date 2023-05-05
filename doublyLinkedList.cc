// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>


/* Doubly Linked List
- A linked list with a pointer to both the previous and the next item
*/

template<typename T>
class DoublyLinkedList {
    struct Node {
        T data;
        Node* next;
        Node* prev;
    };

    Node* pHead;
    Node* pTail;
    size_t size;

    public:
        DoublyLinkedList();
        DoublyLinkedList(const DoublyLinkedList& other);
        DoublyLinkedList(DoublyLinkedList&& other);
        DoublyLinkedList& operator=(const DoublyLinkedList& other);
        DoublyLinkedList& operator=(DoublyLinkedList&& other);
        
        constexpr size_t length() const noexcept;
        T& head();
        const T& head() const;
        T& tail();
        const T& tail() const;

        void add_to_front(const T& elem);
        void add_to_front(T&& elem);
        void push_back(const T& elem);
        void push_back(T&& elem);
        void insert(const T& elem, size_t index);
        void insert(T&& elem, size_t index);
        void erase(int index);
        int search(const T& elem);
        int search(T&& elem);
        T& operator[](int index);

        class Iterator {
                Node* n;
                explicit Iterator(Node* n);
            public:
                T& operator*();
                bool operator!=(const Iterator& other) const;
                Iterator& operator++();
                Iterator& operator--();
                friend class DoublyLinkedList;
        };

        Iterator begin();
        Iterator end();
        Iterator rbegin();
        Iterator rend();

        std::vector<T> toVector();

        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const DoublyLinkedList<U>& ll);
        
        bool isEmpty() const;
        void reverse();
        void clear();
        ~DoublyLinkedList();
};

template<typename T>
DoublyLinkedList<T>::DoublyLinkedList() : pHead{nullptr}, pTail{nullptr}, size{0} {}

template<typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList& other) : pHead{nullptr}, pTail{nullptr}, size{0} {
    Node* otherTraverser = other.pHead;
    Node* thisTraverser = nullptr;
    Node* thisNextTraverser = nullptr;

    if (otherTraverser) {
        pHead = new Node{otherTraverser->data, nullptr, nullptr};
        thisTraverser = pHead;
    }

    while (otherTraverser) {
        ++size;
        otherTraverser = otherTraverser->next;
        thisNextTraverser = otherTraverser? new Node{otherTraverser->data, nullptr, thisTraverser} : otherTraverser;
        thisTraverser->next = thisNextTraverser;
        if (thisNextTraverser) thisTraverser = thisTraverser->next;
    }

    pTail = thisTraverser;
}

template<typename T>
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList&& other) : pHead{other.pHead}, pTail{other.pTail}, size{other.size} {
    other.pHead = nullptr;
    other.pTail = nullptr;
    other.size = 0;
}

template<typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList& other) {
    if (this == &other) return *this;
    clear();

    Node* otherTraverser = other.pHead;
    Node* thisTraverser = nullptr;
    Node* thisNextTraverser = nullptr;

    if (otherTraverser) {
        pHead = new Node{otherTraverser->data, nullptr, nullptr};
        thisTraverser = pHead;
    }

    while (otherTraverser) {
        ++size;
        otherTraverser = otherTraverser->next;
        thisNextTraverser = otherTraverser? new Node{otherTraverser->data, nullptr, thisTraverser} : otherTraverser;
        thisTraverser->next = thisNextTraverser;
        if (thisNextTraverser) thisTraverser = thisTraverser->next;
    }

    pTail = thisTraverser;

    return *this;
}

template<typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(DoublyLinkedList&& other) {
    std::swap(pHead, other.pHead);
    std::swap(pTail, other.pTail);
    std::swap(size, other.size);

    return *this;
}

template<typename T>
constexpr size_t DoublyLinkedList<T>::length() const noexcept { return size; }

template<typename T>
T& DoublyLinkedList<T>::head() {
    if (!pHead) throw std::invalid_argument("List head is NULL");
    return pHead->data;
}

template<typename T>
const T& DoublyLinkedList<T>::head() const {
    if (!pHead) throw std::invalid_argument("List head is NULL");
    return pHead->data;
}

template<typename T>
T& DoublyLinkedList<T>::tail() {
    if (!pTail) throw std::invalid_argument("List tail is NULL");
    return pTail->data;
}

template<typename T>
const T& DoublyLinkedList<T>::tail() const {
    if (!pTail) throw std::invalid_argument("List tail is NULL");
    return pTail->data;
}

template<typename T>
void DoublyLinkedList<T>::add_to_front(const T& elem) {
    Node* newNode = new Node{elem, pHead, nullptr};
    pHead = newNode;
    ++size;
}

template<typename T>
void DoublyLinkedList<T>::add_to_front(T&& elem) {
    Node* newNode = new Node{std::move(elem), pHead, nullptr};
    pHead = newNode;
    ++size;
}

template<typename T>
void DoublyLinkedList<T>::push_back(const T& elem) {
    Node* newNode = new Node{elem, nullptr, pTail};

    if (size == 0) {
        pHead = newNode;
        pTail = newNode;
    } else {
        pTail->next = newNode;
        pTail = newNode;
    }

    ++size;
}

template<typename T>
void DoublyLinkedList<T>::push_back(T&& elem) {
    Node* newNode = new Node{std::move(elem), nullptr, pTail};

    if (size == 0) {
        pHead = newNode;
        pTail = newNode;
    } else {
        pTail->next = newNode;
        pTail = newNode;
    }

    ++size;
}

template<typename T>
void DoublyLinkedList<T>::insert(const T& elem, size_t index) {
    if (index < 0 || index > size) throw std::out_of_range("Invalid index");
    if (index == 0) return add_to_front(elem);
    if (index == size) return push_back(elem);
    Node* traverser = pHead;
    size_t i = 1;

    while (i < index) {
        traverser = traverser->next;
        ++i;
    }

    Node* newNode = new Node{elem, traverser->next, traverser};
    traverser->next = newNode;

    ++size;
}

template<typename T>
void DoublyLinkedList<T>::insert(T&& elem, size_t index) {
    if (index < 0 || index > size) throw std::out_of_range("Invalid index");
    if (index == 0) return add_to_front(elem);
    if (index == size) return push_back(elem);
    Node* traverser = pHead;
    size_t i = 1;

    while (i < index) {
        traverser = traverser->next;
        ++i;
    }

    Node* newNode = new Node{std::move(elem), traverser->next, traverser};
    traverser->next = newNode;

    ++size;
}

template<typename T>
void DoublyLinkedList<T>::erase(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Invalid index");
    }
    Node* toBeDeleted = pHead;

    if (index == 0) {
        pHead = pHead->next;
        pHead->prev = nullptr;
    } else {
        int i = 0;
        Node* traverser = pHead;
        while (i < index - 1) {
            traverser = traverser->next;
            ++i;
        }
        toBeDeleted = traverser->next;

        traverser->next = toBeDeleted->next;
        traverser->next->prev = traverser;
    }

    --size;
    delete toBeDeleted;
}

template<typename T>
int DoublyLinkedList<T>::search(const T& elem) {
    Node* traverser = pHead;
    int i = 0;

    while (traverser) {
        if (traverser->data == elem) return i;
        ++i;
        traverser = traverser->next;
    }

    return -1;
}

template<typename T>
int DoublyLinkedList<T>::search(T&& elem) {
    Node* traverser = pHead;
    int i = 0;

    while (traverser) {
        if (traverser->data == elem) return i;
        ++i;
        traverser = traverser->next;
    }

    return -1;
}

template<typename T>
T& DoublyLinkedList<T>::operator[](int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Invalid index");
    }
    Node* traverser = pHead;
    int i = 0;
    while (i < index) {
        traverser = traverser->next;
        ++i;
    }
    return traverser->data;
}

template<typename T>
DoublyLinkedList<T>::Iterator::Iterator(Node* n) : n{n} {}

template<typename T>
T& DoublyLinkedList<T>::Iterator::operator*() {
    // if n is null, default construct T
    return n->data;
}

template<typename T>
bool DoublyLinkedList<T>::Iterator::operator!=(const Iterator& other) const {
    return other.n != n;
}

template<typename T>
typename DoublyLinkedList<T>::Iterator& DoublyLinkedList<T>::Iterator::operator++() {
    n = n->next;
    return *this;
}

template<typename T>
typename DoublyLinkedList<T>::Iterator& DoublyLinkedList<T>::Iterator::operator--() {
    n = n->prev;
    return *this;
}

template<typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::begin() { return Iterator{pHead}; }

template<typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::end() { return Iterator{nullptr}; }

template<typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::rbegin() { return Iterator{pTail}; }

template<typename T>
typename DoublyLinkedList<T>::Iterator DoublyLinkedList<T>::rend() { return Iterator{nullptr}; }

/* SORTING: 
    My issue is that the Iterator sort won't automatically change head/tail values
    */

template<typename T>
std::vector<T> DoublyLinkedList<T>::toVector() {
    std::vector<T> vec{};

    Node* traverser = pHead;

    while (traverser) {
        vec.push_back(traverser->data);
        traverser = traverser->next;
    }

    return vec;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const DoublyLinkedList<T>& ll) {
    out << "{";
    auto traverser = ll.pHead;

    while (true) {
        out << traverser->data;
        if (traverser == ll.pTail) {
            break;
        }
        traverser = traverser->next;
        out << ", ";
    }

    out << "}";
    return out;
}

template<typename T>
bool DoublyLinkedList<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void DoublyLinkedList<T>::reverse() {
    Node* prev, *curr, *next;
    curr = pHead;
    pTail = pHead;
    prev = nullptr;

    while (curr) {
        next = curr->next;
        curr->next = prev;
        curr->prev = next;
        prev = curr;
        curr = next;
    }

    pHead = prev;
}

template<typename T>
void DoublyLinkedList<T>::clear() {
    Node* traverser = pHead;
    Node* tempNode = nullptr;

    while(traverser) {
        tempNode = traverser->next;
        delete traverser;
        traverser = tempNode;
    }

    size = 0;
    pHead = nullptr;
    pTail = nullptr;
}

template<typename T>
DoublyLinkedList<T>::~DoublyLinkedList() {
    Node* traverser = pHead;
    Node* tempNode = nullptr;

    while(traverser) {
        tempNode = traverser->next;
        delete traverser;
        traverser = tempNode;
    }
}

template<typename T>
DoublyLinkedList<T> getDoublyLinkedList() { // Testing Move operations
    DoublyLinkedList<T> ll;
    ll.push_back(T{});
    ll.push_back(T{});
    ll.push_back(T{});
    return ll;
}

void testDoublyLinkedList() {
    DoublyLinkedList<std::string> friends;
    friends.push_back("Aaron");
    friends.push_back("Jonah");
    friends.push_back("Sahara");

    LOG(friends.length())
    LOG(friends.head())
    LOG(friends.tail())

    friends.insert("Raya", 2);
    std::cout << friends << std::endl;
    friends.add_to_front("Aabhinav");
    std::cout << friends << std::endl;

    LOG(friends.search(std::string{"Raya"}))
    LOG(friends[3])
    LOG(friends.search(std::string{"Adam"}))

    friends.push_back("Wendy");
    friends.push_back("Xie");
    std::cout << friends << std::endl;
    friends.erase(0);
    std::cout << friends << std::endl;
    friends.erase(1);
    std::cout << friends << std::endl;
    friends.erase(3);
    std::cout << friends << std::endl;

    for (auto& myFriend : friends) {
        LOG(myFriend)
    }

    DoublyLinkedList<std::string> sameFriends = friends;
    std::cout << sameFriends << std::endl;

    DoublyLinkedList<std::string> sameFriends2 = getDoublyLinkedList<std::string>();
    for (auto& myFriend : sameFriends2) {
        myFriend = "Sing";
    }
    std::cout << sameFriends2 << std::endl;

    friends.push_back("Yahya");
    friends.push_back("Zendaya");
    sameFriends = friends;
    std::cout << sameFriends << std::endl;

    sameFriends = getDoublyLinkedList<std::string>();
    for (auto& myFriend : sameFriends) {
        myFriend = "Sing";
    }
    std::cout << sameFriends << std::endl;

    std::cout << friends << std::endl;
    friends.reverse();
    std::cout << friends << std::endl;


    DoublyLinkedList<std::string> friends3;
    friends3.push_back("Aaron");
    friends3.push_back("Jonah");
    friends3.push_back("Sahara");
    friends3.push_back("Thoraya");
    friends3.push_back("Wendy");
    friends3.insert("Maya", 2);
    friends3.add_to_front("Adam");
    friends3.push_back("Xenya");
    friends3.push_back("Yarna");
    std::cout << friends3 << std::endl;
    friends3.erase(0);
    friends3.erase(1);
    friends3.erase(3);
    std::cout << friends3 << std::endl;
    friends3.reverse();
    friends3.reverse();
    LOG("FORWARDS TEST")
    for (auto it = friends3.begin(); it != friends3.end(); ++it) {
        LOG(*it)
    }
    LOG("REVERSE TEST")
    for (auto it = friends3.rbegin(); it != friends3.rend(); --it) {
        LOG(*it)
    }
}

int main() {
    testDoublyLinkedList();
}
