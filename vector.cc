// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>


template<typename T>
class Vector {
    T* data;
    size_t vecSize;
    size_t capacity;

        void reallocateMemory(size_t newCap);
    public:
        Vector();
        Vector(const Vector& other);
        Vector(Vector&& other);
        Vector& operator=(const Vector& other);
        Vector& operator=(Vector&& other);
        // constexpr: possible to evaluate value at compile-time
        constexpr size_t size() const;
        T* getData(); // allows us to memset the memory to a defualt value
        const T* getData() const;
        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        void push_back(const T& elem);
        void push_back(T&& elem);
        void pop_back();
        template<typename... args> // variadic template: taking variable # of args
        void emplace_back(args&&... myArgs);
        void insert(const T& elem, size_t n);
        void insert(T&& elem, size_t n);
        void erase(size_t n);
        void clear();
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
        bool isEmpty() const;
        void sort();
        void swap(int a, int b);
        typedef T* Iterator; // typedef for std::iterator_traits
        Iterator begin() const { return Iterator{data}; }
        Iterator end() const { return Iterator{data + vecSize}; }
        Iterator rbegin() const { return Iterator{data + vecSize - 1}; }
        Iterator rend() const { return Iterator{data - 1}; }
        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const Vector<U>& v);
        ~Vector();
};

template<typename T>
void Vector<T>::reallocateMemory(size_t newCap) {
    T* tempData = (T*) ::operator new(sizeof(T) * newCap); // allocates enough memory to hold [newCap] T objects

    if (newCap < vecSize) vecSize = newCap;

    for (size_t i = 0; i < vecSize; ++i)
        tempData[i] = std::move(data[i]);

    for (size_t i = 0; i < vecSize; ++i)
        data[i].~T();

    ::operator delete(data, capacity * sizeof(T)); // deallocating mem
    data = tempData;
    capacity = newCap;
}

template<typename T>
Vector<T>::Vector() : data{nullptr}, vecSize{0}, capacity{0} {
    reallocateMemory(1);
}

template<typename T>
Vector<T>::Vector(const Vector& other) : 
    data{nullptr}, vecSize{other.size()}, capacity{vecSize} {
        data = (T*) ::operator new(sizeof(T) * vecSize);

        for (size_t i = 0; i < vecSize; ++i)
            data[i] = std::move(other[i]);
    }

template<typename T>
Vector<T>::Vector(Vector&& other) : 
    data{nullptr}, vecSize{other.size()}, capacity{vecSize} {
        /*
        data = (T*) ::operator new(sizeof(T) * vecSize);

        for (size_t i = 0; i < vecSize; ++i) {
            data[i] = std::move(other[i]);
        }*/
        data = other.data;

        other.data = nullptr;
        other.capacity = 0;
        other.vecSize = 0;
    }

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    for (size_t i = 0; i < vecSize; ++i)
        data[i].~T();

    ::operator delete(data, vecSize * sizeof(T)); // deallocating mem

    vecSize = other.size();
    capacity = vecSize;
    
    data = (T*) ::operator new(sizeof(T) * vecSize);

    for (size_t i = 0; i < vecSize; ++i)
        data[i] = std::move(other[i]);

    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) {
    std::swap(vecSize, other.vecSize);
    std::swap(capacity, other.capacity);
    std::swap(data, other.data);

    return *this;
}

template<typename T>
constexpr size_t Vector<T>::size() const { return vecSize; }

template<typename T>
T* Vector<T>::getData() { return data; } // allows us to memset the memory to a defualt value
template<typename T>
const T* Vector<T>::getData() const { return data; }

template<typename T>
T& Vector<T>::operator[](size_t index) {
    assert(index >= 0 && index < vecSize);
    return data[index];
}

template<typename T>
const T& Vector<T>::operator[](size_t index) const {
    assert(index >= 0 && index < vecSize);
    return data[index];
}

template<typename T>
void Vector<T>::push_back(const T& elem) {
    if (vecSize == capacity) {
        reallocateMemory(capacity * 2);
    }

    data[vecSize] = elem;
    ++vecSize;
}

template<typename T>
void Vector<T>::push_back(T&& elem) { // avoids unnecessary copies
    if (vecSize == capacity) {
        reallocateMemory(capacity * 2);
    }

    data[vecSize] = std::move(elem); // avoids unnecessary copies
    ++vecSize;
}

template<typename T>
void Vector<T>::pop_back() {
    if (vecSize == 0) return;
    --vecSize;
    data[vecSize].~T(); // manually destroy the object
}

template<typename T>
template<typename... args> // variadic template: taking variable # of args
void Vector<T>::emplace_back(args&&... myArgs) {
    if (vecSize == capacity) {
        reallocateMemory(capacity * 2);
    }
    new(&data[vecSize]) T(std::forward<args>(myArgs)...); //getting location and allocation, writing in directly
    ++vecSize;
}

template<typename T>
void Vector<T>::insert(const T& elem, size_t n) {
    if (n < 0 || n > vecSize) {
        throw std::out_of_range("Invalid index");
    }
    if (vecSize == capacity) {
        reallocateMemory(capacity * 2);
    }

    size_t i = vecSize;
    while (i > n) {
        data[i] = std::move(data[i - 1]);
        --i;
    }

    ++vecSize;
    data[i].~T();

    data[i] = elem;
}

template<typename T>
void Vector<T>::insert(T&& elem, size_t n) {
    if (n < 0 || n > vecSize) {
        throw std::out_of_range("Invalid index");
    }
    if (vecSize == capacity) {
        reallocateMemory(capacity * 2);
    }

    size_t i = vecSize;
    while (i > n) {
        data[i] = std::move(data[i - 1]);
        --i;
    }

    ++vecSize;
    data[i].~T();

    data[i] = std::move(elem);
}

template<typename T>
void Vector<T>::erase(size_t n) {
    if (n < 0 || n >= vecSize) {
        throw std::out_of_range("Invalid index");
    }

    size_t i = 0;
    while (i < n) ++i;

    --vecSize;
    data[i].~T();

    while (i < vecSize) {
        data[i] = std::move(data[i + 1]);
        ++i;
    }

    data[vecSize].~T();
}

template<typename T>
void Vector<T>::clear() {
    for (size_t i = 0; i < vecSize; ++i) {
        data[i].~T();
    }

    vecSize = 0;
    reallocateMemory(1);
}

template<typename T>
T& Vector<T>::front() {
    if (vecSize == 0) throw std::out_of_range("Empty array");
    return data[0];
}

template<typename T>
const T& Vector<T>::front() const {
    if (vecSize == 0) throw std::out_of_range("Empty array");
    return data[0];
}

template<typename T>
T& Vector<T>::back() {
    if (vecSize == 0) throw std::out_of_range("Empty array");
    return data[vecSize - 1];
}

template<typename T>
const T& Vector<T>::back() const {
    if (vecSize == 0) throw std::out_of_range("Empty array");
    return data[vecSize - 1];
}

template<typename T>
bool Vector<T>::isEmpty() const {
    return vecSize == 0;
}

template<typename T>
void Vector<T>::sort() {
    std::sort(begin(), end());
}

template<typename T>
void Vector<T>::swap(int a, int b) {
    std::swap(data[a], data[b]);
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Vector<T>& v) {
    out << "{";
    for (size_t i = 0; i < v.vecSize; ++i) {
        out << v[i];
        if (i != v.vecSize - 1) out << ", ";
    }
    out << "}" << std::endl;
    return out;
}

template<typename T>
Vector<T>::~Vector() {
    clear();

    ::operator delete(data, capacity * sizeof(T)); // deallocating mem
}

Vector<std::string> getNewVec() {
    Vector<std::string> myList;
    myList.emplace_back("Adam");
    myList.emplace_back("Annie");
    myList.emplace_back("Wally");
    myList.emplace_back("Wendy");
    return myList;
}

void testVectorClass() {
    Vector<std::string> studentList;

    studentList.push_back("Amy");
    studentList.push_back("Carlos");
    studentList.push_back("Jonathan");
    studentList.push_back("Marcia");

    std::cout << studentList;

    studentList.emplace_back("Octavio");
    studentList.emplace_back("Rebecca");

    std::cout << studentList;
    studentList.pop_back();
    std::cout << studentList;

    std::cout << studentList.front() << " " << studentList.back() << std::endl;
    studentList.swap(0, 4);
    std::cout << studentList;
    studentList.sort();
    std::cout << studentList;

    for (auto& student : studentList)
        std::cout << "Classmate: " << student << std::endl;

    studentList.clear();

    std::cout << studentList;
    studentList.push_back("Amy");
    std::cout << studentList;
    studentList.emplace_back("Octavio");
    std::cout << studentList;
    studentList.pop_back();
    std::cout << studentList;

    studentList.emplace_back("Jayme");
    studentList.emplace_back("Josue");
    studentList.emplace_back("Robert");
    studentList.emplace_back("Rhonda");
    std::cout << studentList;
    studentList.erase(0);
    std::cout << studentList;
    studentList.erase(2);
    std::cout << studentList;
    studentList.insert("Amy", 0);
    std::cout << studentList;
    std::string s = "Robert";
    studentList.insert(s, 2);
    std::cout << studentList;

    Vector<std::string> v2{studentList};
    std::cout << v2;

    Vector<std::string> v3;
    v3 = studentList;
    std::cout << v3;

    std::cout << studentList;
    std::cout << std::move(studentList);
    std::cout << std::move(studentList);

    Vector<std::string> v4;
    v4 = getNewVec();
    std::cout << v4;

    Vector<std::string> v5{getNewVec()};
    std::cout << v5;
}

int main() {
    testVectorClass();
}
