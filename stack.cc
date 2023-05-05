// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>

template<typename T>
class Stack {
    struct Node {
        T data;
        Node* next;
    };

    Node* pTop;
    size_t pSize;

    public:
        Stack();
        Stack(const Stack& other);
        Stack(Stack&& other);
        Stack& operator=(const Stack& other);
        Stack& operator=(Stack&& other);
        constexpr size_t size() const noexcept;
        T& top();
        const T& top() const;
        void push(const T& elem);
        void push(T&& elem);
        void pop();
        bool isEmpty();
        class Iterator {
                Node* n;
                explicit Iterator(Node* n);
            public:
                T& operator*();
                bool operator!=(const Iterator& other) const;
                Iterator& operator++();
                friend class Stack;
        };

        Iterator begin();
        Iterator end();
        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const Stack<U>& st);
        void clear();
        ~Stack();
};

template<typename T>
Stack<T>::Stack() : pTop{nullptr}, pSize{0} {}

template<typename T>
Stack<T>::Stack(const Stack& other) : pTop{nullptr}, pSize{0} {
    Node* otherTraverser = other.pTop;
    Node* thisTraverser = nullptr;
    Node* thisNextTraverser = nullptr;

    if (otherTraverser) {
        pTop = new Node{otherTraverser->data, nullptr};
        thisTraverser = pTop;
    }

    while (otherTraverser) {
        ++pSize;
        otherTraverser = otherTraverser->next;
        thisNextTraverser = otherTraverser? new Node{otherTraverser->data, nullptr} : otherTraverser;
        thisTraverser->next = thisNextTraverser;
        if (thisNextTraverser) thisTraverser = thisTraverser->next;
    }
}

template<typename T>
Stack<T>::Stack(Stack&& other) : pTop{other.pTop}, pSize{other.pSize} {
    other.pTop = nullptr;
    other.pSize = 0;
}

template<typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
    if (this == &other) return *this;
    clear();

    Node* otherTraverser = other.pTop;
    Node* thisTraverser = nullptr;
    Node* thisNextTraverser = nullptr;

    if (otherTraverser) {
        pTop = new Node{otherTraverser->data, nullptr};
        thisTraverser = pTop;
    }

    while (otherTraverser) {
        ++pSize;
        otherTraverser = otherTraverser->next;
        thisNextTraverser = otherTraverser? new Node{otherTraverser->data, nullptr} : otherTraverser;
        thisTraverser->next = thisNextTraverser;
        if (thisNextTraverser) thisTraverser = thisTraverser->next;
    }

    return *this;
}

template<typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) {
    std::swap(pTop, other.pTop);
    std::swap(pSize, other.pSize);

    return *this;
}

template<typename T>
constexpr size_t Stack<T>::size() const noexcept { return pSize; }

template<typename T>
T& Stack<T>::top() {
    if (!pTop) throw std::invalid_argument("Stack TOP is NULL");
    return pTop->data;
}

template<typename T>
const T& Stack<T>::top() const {
    if (!pTop) throw std::invalid_argument("List head is NULL");
    return pTop->data;
}

template<typename T>
void Stack<T>::push(const T& elem) {
    Node* newTop = new Node{elem, pTop};
    pTop = newTop;

    ++pSize;
}

template<typename T>
void Stack<T>::push(T&& elem) {
    Node* newTop = new Node{elem, pTop};
    pTop = newTop;

    ++pSize;
}

template<typename T>
void Stack<T>::pop() {
    Node* toBeDeleted = pTop;
    pTop = pTop->next;

    --pSize;
    delete toBeDeleted;
}

template<typename T>
bool Stack<T>::isEmpty() { return pSize == 0; }

template<typename T>
Stack<T>::Iterator::Iterator(Node* n) : n{n} {}

template<typename T>
T& Stack<T>::Iterator::operator*() {
    // if n is null, default construct T
    return n->data;
}

template<typename T>
bool Stack<T>::Iterator::operator!=(const Iterator& other) const {
    return other.n!= n;
}

template<typename T>
typename Stack<T>::Iterator& Stack<T>::Iterator::operator++() {
    n = n->next;
    return *this;
}

template<typename T>
typename Stack<T>::Iterator Stack<T>::begin() { return Iterator{pTop}; }
template<typename T>
typename Stack<T>::Iterator Stack<T>::end() { return Iterator{nullptr}; }

template<typename T>
std::ostream& operator<<(std::ostream& out, const Stack<T>& st) {
    out << "{";
    auto traverser = st.pTop;

    while (true) {
        out << traverser->data;
        traverser = traverser->next;
        if (!traverser) {
            break;
        }
        out << ", ";
    }

    out << "}";
    return out;
}

template<typename T>
void Stack<T>::clear() {
    Node* traverser = pTop;
    Node* tempNode = nullptr;

    while(traverser) {
        tempNode = traverser->next;
        delete traverser;
        traverser = tempNode;
    }

    pSize = 0;
    pTop = nullptr;
}

template<typename T>
Stack<T>::~Stack() {
    Node* traverser = pTop;
    Node* tempNode = nullptr;

    while(traverser) {
        tempNode = traverser->next;
        delete traverser;
        traverser = tempNode;
    }
}

Stack<std::string> getNewStack() {
    Stack<std::string> st;
    st.push("Jared");
    st.push("Kaya");
    st.push("Kevin");
    st.push("Matt");
    return st;
}


void testStackClass() {
    Stack<std::string> examsToMark;

    examsToMark.push("Aidan");
    examsToMark.push("Billie");
    examsToMark.push("Carol");

    std::cout << examsToMark << std::endl;
    examsToMark.pop();
    std::cout << examsToMark << std::endl;

    for (auto& exam : examsToMark) LOG("Marking " + exam + "'s exam")
    
    Stack<std::string> examsHandedIn = examsToMark;
    std::cout << examsHandedIn << std::endl;
    Stack<std::string> drillLine = getNewStack();
    std::cout << drillLine << std::endl;

    drillLine = examsToMark;
    examsHandedIn = getNewStack();
    std::cout << examsHandedIn << std::endl;
    std::cout << drillLine << std::endl;
}

int main() {
    testStackClass();
}
