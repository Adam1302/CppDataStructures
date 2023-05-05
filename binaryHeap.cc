// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>
#include <vector>

/*
Structural Property: all spots are filled from top-down, left-to-right
Heap-Order Property: all children values are less than their parent values (max-oriented)
*/
template<typename T>
class BinaryHeap {
    std::vector<T> pItems;
    size_t pSize;
    bool minOriented;

    size_t parentIndex(size_t s) const;
    size_t leftChildIndex(size_t s) const noexcept;
    size_t rightChildIndex(size_t s) const noexcept;
    void fixUp(size_t idx);
    void fixDown(size_t idx);

    public:
        BinaryHeap(bool minOriented = false);
        BinaryHeap(std::vector<T> vec, bool minOriented = false);
        constexpr size_t size() const;
        T& top();
        void insert(const T& elem);
        void insert(T&& elem);
        void deleteMax();
        void deleteMin();
        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const BinaryHeap<U>& bh);
};

template <typename T>
size_t BinaryHeap<T>::parentIndex(size_t s) const {
    if (pSize == 0) throw std::invalid_argument("Root of heap has no parent");

    return (s - 1) / 2;
}

template <typename T>
size_t BinaryHeap<T>::leftChildIndex(size_t s) const noexcept {
    return (s * 2) + 1;
}

template <typename T>
size_t BinaryHeap<T>::rightChildIndex(size_t s) const noexcept {
    return (s * 2) + 2;
}

template <typename T>
void BinaryHeap<T>::fixUp(size_t idx) {
    while (idx != 0 && (!minOriented && pItems[idx] > pItems[parentIndex(idx)]) || (minOriented && pItems[idx] < pItems[parentIndex(idx)])) {
        std::swap(pItems[idx], pItems[parentIndex(idx)]);
        idx = parentIndex(idx);
    }
}

template <typename T>
void BinaryHeap<T>::fixDown(size_t idx) {
    while (pSize > leftChildIndex(idx)) { // idx isn't leaf
        int largestChildIndex = leftChildIndex(idx);

        if (pSize > rightChildIndex(idx) && 
        (!minOriented && pItems[largestChildIndex] < pItems[rightChildIndex(idx)]) || (minOriented && pItems[largestChildIndex] > pItems[rightChildIndex(idx)])) {
            largestChildIndex = rightChildIndex(idx);
        }

        if ((!minOriented && pItems[idx] >= pItems[largestChildIndex]) || (minOriented && pItems[idx] >= pItems[largestChildIndex])) break;

        std::swap(pItems[idx], pItems[largestChildIndex]);
        idx = largestChildIndex;
    }
}

template <typename T>
BinaryHeap<T>::BinaryHeap(bool minOriented) : pItems{}, pSize{0}, minOriented{minOriented} {}

template <typename T>
BinaryHeap<T>::BinaryHeap(std::vector<T> vec, bool minOriented) : pItems{vec}, pSize{vec.size()}, minOriented{minOriented} {
    for (int i = parentIndex(pSize - 1); i >= 0; --i) {
        fixDown(i);
    }
}

template <typename T>
constexpr size_t BinaryHeap<T>::size() const { return pSize; }

template <typename T>
T& BinaryHeap<T>::top() {
    if (pSize == 0) throw std::out_of_range("Heap is empty");

    return pItems[0];
}

template <typename T>
void BinaryHeap<T>::insert(const T& elem) {
    pItems.push_back(elem);
    fixUp(pSize);
    ++pSize;
}

template <typename T>
void BinaryHeap<T>::insert(T&& elem) {
    pItems.push_back(elem);
    fixUp(pSize);
    ++pSize;
}

template <typename T>
void BinaryHeap<T>::deleteMax() {
    if (minOriented) throw std::invalid_argument("Error: this heap is min-oriented");

    std::swap(pItems[0], pItems[pSize - 1]);
    --pSize;

    fixDown(0);
}

template <typename T>
void BinaryHeap<T>::deleteMin() {
    if (!minOriented) throw std::invalid_argument("Error: this heap is max-oriented");

    std::swap(pItems[0], pItems[pSize - 1]);
    --pSize;

    fixDown(0);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const BinaryHeap<T>& bh) {
    out << "{";
    for (size_t i = 0; i < bh.pSize - 1; ++i) out << bh.pItems[i] << ", ";

    if (bh.pSize != 0) out << bh.pItems[bh.pSize - 1];
    out << "}";

    return out;
}

void testBinaryHeap() {
    BinaryHeap<int> b;

    b.insert(5);
    b.insert(9);
    b.insert(12);
    b.insert(8);
    b.insert(15);
    b.insert(3);
    b.insert(2);
    b.insert(20);
    b.insert(14);
    b.insert(10);

    std::cout << b << "\n";

    b.deleteMax();

    std::cout << b << "\n";

    std::vector mike{10, 80, 50, 30, 20, 60, 10, 40, 70};
    BinaryHeap mikesHeap = mike;
    std::cout << mikesHeap << "\n";

    b = mikesHeap;
    b.deleteMax();
    std::cout << b << "\n";
    std::cout << mikesHeap << "\n";

}

int main() {
    testBinaryHeap();
}
