// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>

template<typename T>
class LinkedList {
    struct Node {
        T data;
        Node* next;
    };

    Node* pHead;
    Node* pTail;
    size_t size;

    public:
        LinkedList();
        LinkedList(const LinkedList& other);
        LinkedList(LinkedList&& other);
        LinkedList& operator=(const LinkedList& other);
        LinkedList& operator=(LinkedList&& other);
        
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
                friend class LinkedList;
        };

        Iterator begin();
        Iterator end();

        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const LinkedList<U>& ll); 
        std::vector<T> toVector();
        bool isEmpty() const;
        void reverse();
        void clear();

        ~LinkedList();
};

template<typename T>
LinkedList<T>::LinkedList() : pHead{nullptr}, pTail{nullptr}, size{0} {}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : pHead{nullptr}, pTail{nullptr}, size{0} {
    Node* otherTraverser = other.pHead;
    Node* thisTraverser = nullptr;
    Node* thisNextTraverser = nullptr;

    if (otherTraverser) {
        pHead = new Node{otherTraverser->data, nullptr};
        thisTraverser = pHead;
    }

    while (otherTraverser) {
        ++size;
        otherTraverser = otherTraverser->next;
        thisNextTraverser = otherTraverser? new Node{otherTraverser->data, nullptr} : otherTraverser;
        thisTraverser->next = thisNextTraverser;
        if (thisNextTraverser) thisTraverser = thisTraverser->next;
    }

    pTail = thisTraverser;
}

template<typename T>
LinkedList<T>::LinkedList(LinkedList&& other) : pHead{other.pHead}, pTail{other.pTail}, size{other.size} {
    other.pHead = nullptr;
    other.pTail = nullptr;
    other.size = 0;
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this == &other) return *this;
    clear();

    Node* otherTraverser = other.pHead;
    Node* thisTraverser = nullptr;
    Node* thisNextTraverser = nullptr;

    if (otherTraverser) {
        pHead = new Node{otherTraverser->data, nullptr};
        thisTraverser = pHead;
    }

    while (otherTraverser) {
        ++size;
        otherTraverser = otherTraverser->next;
        thisNextTraverser = otherTraverser? new Node{otherTraverser->data, nullptr} : otherTraverser;
        thisTraverser->next = thisNextTraverser;
        if (thisNextTraverser) thisTraverser = thisTraverser->next;
    }

    pTail = thisTraverser;

    return *this;
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) {
    std::swap(pHead, other.pHead);
    std::swap(pTail, other.pTail);
    std::swap(size, other.size);

    return *this;
}

template<typename T>
constexpr size_t LinkedList<T>::length() const noexcept { return size; }

template<typename T>
T& LinkedList<T>::head() {
    if (!pHead) throw std::invalid_argument("List head is NULL");
    return pHead->data;
}

template<typename T>
const T& LinkedList<T>::head() const {
    if (!pHead) throw std::invalid_argument("List head is NULL");
    return pHead->data;
}

template<typename T>
T& LinkedList<T>::tail() {
    if (!pTail) throw std::invalid_argument("List tail is NULL");
    return pTail->data;
}

template<typename T>
const T& LinkedList<T>::tail() const {
    if (!pTail) throw std::invalid_argument("List tail is NULL");
    return pTail->data;
}

template<typename T>
void LinkedList<T>::add_to_front(const T& elem) {
    Node* newNode = new Node{elem, pHead};
    pHead = newNode;
    ++size;
}

template<typename T>
void LinkedList<T>::add_to_front(T&& elem) {
    Node* newNode = new Node{std::move(elem), pHead};
    pHead = newNode;
    ++size;
}

template<typename T>
void LinkedList<T>::push_back(const T& elem) {
    Node* newNode = new Node{elem, nullptr};

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
void LinkedList<T>::push_back(T&& elem) {
    Node* newNode = new Node{std::move(elem), nullptr};

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
void LinkedList<T>::insert(const T& elem, size_t index) {
    if (index < 0 || index > size) throw std::out_of_range("Invalid index");
    if (index == 0) return add_to_front(elem);
    if (index == size) return push_back(elem);
    Node* traverser = pHead;
    size_t i = 1;

    while (i < index) {
        traverser = traverser->next;
        ++i;
    }

    Node* newNode = new Node{elem, traverser->next};
    traverser->next = newNode;

    ++size;
}

template<typename T>
void LinkedList<T>::insert(T&& elem, size_t index) {
    if (index < 0 || index > size) throw std::out_of_range("Invalid index");
    if (index == 0) return add_to_front(elem);
    if (index == size) return push_back(elem);
    Node* traverser = pHead;
    size_t i = 1;

    while (i < index) {
        traverser = traverser->next;
        ++i;
    }

    Node* newNode = new Node{std::move(elem), traverser->next};
    traverser->next = newNode;

    ++size;
}

template<typename T>
void LinkedList<T>::erase(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Invalid index");
    }
    Node* toBeDeleted = pHead;

    if (index == 0) {
        pHead = pHead->next;
    } else {
        int i = 0;
        Node* traverser = pHead;
        while (i < index - 1) {
            traverser = traverser->next;
            ++i;
        }
        toBeDeleted = traverser->next;

        traverser->next= toBeDeleted->next;
    }

    --size;
    delete toBeDeleted;
}

template<typename T>
int LinkedList<T>::search(const T& elem) {
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
int LinkedList<T>::search(T&& elem) {
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
T& LinkedList<T>::operator[](int index) {
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
T& LinkedList<T>::Iterator::operator*() {
    // if n is null, default construct T
    return n->data;
}

template<typename T>
bool LinkedList<T>::Iterator::operator!=(const Iterator& other) const {
    return other.n!= n;
}

template<typename T>
typename LinkedList<T>::Iterator& LinkedList<T>::Iterator::operator++() {
    n = n->next;
    return *this;
}

template<typename T>
LinkedList<T>::Iterator::Iterator(Node* n) : n{n} {}

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::begin() { return Iterator{pHead}; }

template<typename T>
typename LinkedList<T>::Iterator LinkedList<T>::end() { return Iterator{nullptr}; }

template<typename T>
std::vector<T> LinkedList<T>::toVector() {
    std::vector<T> vec{};

    Node* traverser = pHead;

    while (traverser) {
        vec.push_back(traverser->data);
        traverser = traverser->next;
    }

    return vec;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const LinkedList<T>& ll) {
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
bool LinkedList<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void LinkedList<T>::reverse() {
    Node* prev, *curr, *next;
    curr = pHead;
    pTail = pHead;
    prev = nullptr;

    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    pHead = prev;
}

template<typename T>
void LinkedList<T>::clear() {
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
LinkedList<T>::~LinkedList() {
    Node* traverser = pHead;
    Node* tempNode = nullptr;

    while(traverser) {
        tempNode = traverser->next;
        delete traverser;
        traverser = tempNode;
    }
}

template<typename T>
LinkedList<T> getLinkedList() { // Testing Move operations
    LinkedList<T> ll;
    ll.push_back(T{});
    ll.push_back(T{});
    ll.push_back(T{});
    return ll;
}

void testLinkedList() {
        LinkedList<std::string> friends;
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

    LinkedList<std::string> sameFriends = friends;
    std::cout << sameFriends << std::endl;
    LinkedList<std::string> sameFriends2 = getLinkedList<std::string>();
    for (auto& myFriend : sameFriends2) {
        myFriend = "Sing";
    }
    std::cout << sameFriends2 << std::endl;

    friends.push_back("Yahya");
    friends.push_back("Zendaya");
    sameFriends = friends;
    std::cout << sameFriends << std::endl;

    sameFriends = getLinkedList<std::string>();
    for (auto& myFriend : sameFriends) {
        myFriend = "Sing";
    }
    std::cout << sameFriends << std::endl;

    std::cout << friends << std::endl;
    friends.reverse();
    std::cout << friends << std::endl;
}

int main() {
    testLinkedList();
}
