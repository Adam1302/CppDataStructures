// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>

template<typename T>
class Queue {
    struct Node {
        T data;
        Node* next;
    };

    Node* pHead;
    Node* pTail;
    size_t pSize;

    public:
        Queue();
        Queue(const Queue& other);
        Queue(Queue&& other);
        Queue& operator=(const Queue& other);
        Queue& operator=(Queue&& other);
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
        void push_back(const T& elem);
        void push_back(T&& elem);
        void pop_front();
        constexpr size_t size() const;
        bool isEmpty() const;

        class Iterator {
                Node* n;
                explicit Iterator(Node* n);
            public:
                T& operator*();
                bool operator!=(const Iterator& other) const;
                Iterator& operator++();
                friend class Queue;
        };

        Iterator begin();
        Iterator end();

        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const Queue<U>& qu);
        void clear();
        ~Queue();
};

template<typename T>
Queue<T>::Queue() : pHead{nullptr}, pTail{nullptr}, pSize{0} {}

template<typename T>
Queue<T>::Queue(const Queue& other) : pHead{nullptr}, pTail{nullptr}, pSize{0} {
    Node* otherTraverser = other.pHead;
    Node* thisTraverser = nullptr;
    Node* thisNextTraverser = nullptr;

    if (otherTraverser) {
        pHead = new Node{otherTraverser->data, nullptr};
        thisTraverser = pHead;
    }

    while (otherTraverser) {
        ++pSize;
        otherTraverser = otherTraverser->next;
        thisNextTraverser = otherTraverser? new Node{otherTraverser->data, nullptr} : otherTraverser;
        thisTraverser->next = thisNextTraverser;
        if (thisNextTraverser) thisTraverser = thisTraverser->next;
    }

    pTail = thisTraverser;
}

template<typename T>
Queue<T>::Queue(Queue&& other) : pHead{other.pHead}, pTail{other.pTail}, pSize{other.pSize} {
    other.pHead = nullptr;
    other.pTail = nullptr;
    other.pSize = 0;
}

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
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
        ++pSize;
        otherTraverser = otherTraverser->next;
        thisNextTraverser = otherTraverser? new Node{otherTraverser->data, nullptr} : otherTraverser;
        thisTraverser->next = thisNextTraverser;
        if (thisNextTraverser) thisTraverser = thisTraverser->next;
    }

    pTail = thisTraverser;

    return *this;
}

template<typename T>
Queue<T>& Queue<T>::operator=(Queue&& other) {
    std::swap(pHead, other.pHead);
    std::swap(pTail, other.pTail);
    std::swap(pSize, other.pSize);

    return *this;
}

template<typename T>
T& Queue<T>::front() {
    if (!pHead) throw std::invalid_argument("Queue head is NULL");
    return pHead->data;
}

template<typename T>
const T& Queue<T>::front() const {
    if (!pHead) throw std::invalid_argument("Queue head is NULL");
    return pHead->data;
}

template<typename T>
T& Queue<T>::back() {
    if (!pTail) throw std::invalid_argument("Queue tail is NULL");
    return pTail->data;
}

template<typename T>
const T& Queue<T>::back() const {
    if (!pTail) throw std::invalid_argument("Queue tail is NULL");
    return pTail->data;
}

template<typename T>
void Queue<T>::push_back(const T& elem) {
    Node* newNode = new Node{elem, nullptr};

    if (pSize == 0) {
        pHead = newNode;
        pTail = newNode;
    } else {
        pTail->next = newNode;
        pTail = newNode;
    }

    ++pSize;
}

template<typename T>
void Queue<T>::push_back(T&& elem) {
    Node* newNode = new Node{std::move(elem), nullptr};

    if (pSize == 0) {
        pHead = newNode;
        pTail = newNode;
    } else {
        pTail->next = newNode;
        pTail = newNode;
    }

    ++pSize;
}

template<typename T>
void Queue<T>::pop_front() {
    Node* toBeDeleted = pHead;
    pHead = pHead->next;

    --pSize;
    delete toBeDeleted;
}

template<typename T>
constexpr size_t Queue<T>::size() const { return pSize; }

template<typename T>
bool Queue<T>::isEmpty() const { return pSize == 0; }

template<typename T>
Queue<T>::Iterator::Iterator(Node* n) : n{n} {}

template<typename T>
T& Queue<T>::Iterator::operator*() {
        // if n is null, default construct T
    return n->data;
}

template<typename T>
bool Queue<T>::Iterator::operator!=(const Iterator& other) const {
    return other.n != n;
}

template<typename T>
typename Queue<T>::Iterator& Queue<T>::Iterator::operator++() {
    n = n->next;
    return *this;
}

template<typename T>
typename Queue<T>::Iterator Queue<T>::begin() { return Iterator{pHead}; }
template<typename T>
typename Queue<T>::Iterator Queue<T>::end() { return Iterator{nullptr}; }

template<typename T>
std::ostream& operator<<(std::ostream& out, const Queue<T>& qu) {
    out << "{";
    auto traverser = qu.pHead;

    while (true) {
        out << traverser->data;
        if (traverser == qu.pTail) {
            break;
        }
        traverser = traverser->next;
        out << ", ";
    }

    out << "}";
    return out;
}

template<typename T>
void Queue<T>::clear() {
    Node* traverser = pHead;
    Node* tempNode = nullptr;

    while(traverser) {
        tempNode = traverser->next;
        delete traverser;
        traverser = tempNode;
    }

    pSize = 0;
    pHead = nullptr;
    pTail = nullptr;
}

template<typename T>
Queue<T>::~Queue() {
    Node* traverser = pHead;
    Node* tempNode = nullptr;

    while(traverser) {
        tempNode = traverser->next;
        delete traverser;
        traverser = tempNode;
    }
}

Queue<std::string> getNewQueue() {
    Queue<std::string> st;
    st.push_back("Jared");
    st.push_back("Kaya");
    st.push_back("Kevin");
    st.push_back("Matt");
    return st;
}

void testQueueClass() {
    Queue<std::string> qu;

    qu.push_back("Tina");
    qu.push_back("Vanessa");
    qu.push_back("Charles");
    qu.push_back("Sam");
    std::cout << qu << std::endl;

    qu.pop_front();
    std::cout << qu << std::endl;

    LOG(qu.front())
    LOG(qu.back())

    qu.clear();

    qu.push_back("Jim");
    qu.push_back("James");
    qu.push_back("Jayme");

    std::cout << qu << std::endl;

    Queue<std::string> examsHandedIn = qu;
    std::cout << examsHandedIn << std::endl;
    Queue<std::string> drillLine = getNewQueue();
    std::cout << drillLine << std::endl;

    drillLine = qu;
    examsHandedIn = getNewQueue();
    std::cout << examsHandedIn << std::endl;
    std::cout << drillLine << std::endl;

}

int main() {
    testQueueClass();
}
