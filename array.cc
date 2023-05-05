// C++ Data Structures

#define DEBUG_MODE 1
#if DEBUG_MODE
#define LOG(x) std::cout << x << std::endl;
#else
#define LOG(x)
#endif

#include <iostream>


template<typename T>
class Array {
    T* data;
    size_t s;

    public:
        constexpr size_t size() const;
        Array(size_t size = 1);
        Array(const Array& other);
        Array& operator=(const Array& other);
        Array(const Array&& other);
        Array& operator=(const Array&& other);
        T* getData(); // allows us to memset the memory to a defualt value
        const T* getData() const;
        T& operator[](int index);
        const T& operator[](int index) const;
        T& at(int index);
        const T& at(int index) const;
        T& front();
        const T& front() const;
        T& back();
        const T& back() const;
        bool isEmpty() const;
        void sort();
        void swap(int a, int b);

        typedef T* Iterator;

        Iterator begin() const { return Iterator{data}; }
        Iterator end() const { return Iterator{data + s}; }
        Iterator rbegin() const { return Iterator{data + s - 1}; }
        Iterator rend() const { return Iterator{data - 1}; }

        template <typename U>
        friend std::ostream& operator<<(std::ostream& out, const Array<U>& arr);
        template <typename U>
        friend std::istream& operator>>(std::istream& in, const Array<U>& arr);
        ~Array();
};

template <typename T>
constexpr size_t Array<T>::size() const { return s; }

template <typename T>
Array<T>::Array(size_t size) : data{new T[size]}, s{size} {}

template <typename T>
Array<T>::Array(const Array& other) : data{new T[other.size()]}, s{s} {
    for (int i = 0; i < s; ++i) {
        data[i] = other[i];
    }
}

template <typename T>
Array<T>& Array<T>::operator=(const Array& other) {
    if (&other == this) return *this;

    delete[] data;
    s = other.s;
    data = new T[s];

    for (int i = 0; i < s; ++i) {
        data[i] = other[i];
    }

    return *this;
}

template <typename T>
Array<T>::Array(const Array&& other) : data{nullptr}, s{0} {
    std::swap(s, other.s);
    std::swap(data, other.data);
}

template <typename T>
Array<T>& Array<T>::operator=(const Array&& other) {
    std::swap(s, other.s);
    std::swap(data, other.data);
}

template <typename T>
T* Array<T>::getData() { return data; } // allows us to memset the memory to a defualt value
template <typename T>
const T* Array<T>::getData() const { return data; }

template <typename T>
T& Array<T>::operator[](int index) {
    return data[index];
}

template <typename T>
const T& Array<T>::operator[](int index) const {
    return data[index];
}

template <typename T>
T& Array<T>::at(int index) {
    if (index < 0 || index >= s) {
        throw std::out_of_range("Invalid index");
    }
    return data[index];
}

template <typename T>
const T& Array<T>::at(int index) const {
    if (index < 0 || index >= s) {
        throw std::out_of_range("Invalid index");
    }
    return data[index];
}

template <typename T>
T& Array<T>::front() {
    if (s == 0) throw std::out_of_range("Empty array");
    return data[0];
}

template <typename T>
const T& Array<T>::front() const {
    if (s == 0) throw std::out_of_range("Empty array");
    return data[0];
}

template <typename T>
T& Array<T>::back() {
    if (s == 0) throw std::out_of_range("Empty array");
    return data[s - 1];
}

template <typename T>
const T& Array<T>::back() const {
    if (s == 0) throw std::out_of_range("Empty array");
    return data[s - 1];
}

template <typename T>
bool Array<T>::isEmpty() const {
    return s == 0;
}

template <typename T>
void Array<T>::sort() {
    std::sort(begin(), end());
}

template <typename T>
void Array<T>::swap(int a, int b) {
    std::swap(data[a], data[b]);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Array<T>& arr) {
    out << '{';
    for (size_t i = 0; i < arr.s; ++i) {
        out << arr[i];
        if (i != arr.s - 1) out << ", ";
    }
    out << '}' << std::endl;
    return out;
}

template <typename T>
std::istream& operator>>(std::istream& in, const Array<T>& arr) {
    for (size_t i = 0; i < arr.s; ++i) {
        in >> arr.data[i];
    }
    return in;
}

template <typename T>
Array<T>::~Array() {
    delete[] data;
}

void testArrayClass() {
    Array<int> arr2{5};

    for (int i = 0; i < arr2.size(); ++i) {
        arr2.at(i) = arr2.size() - i;
    }

    std::cout << arr2;
    arr2.sort();
    std::cout << arr2;
    arr2.swap(0, 3);
    std::cout << arr2;

    Array<char> name{3};
    Array<char> anotherName{3};

    name[0] = 'P';
    name[1] = 'a';
    name[2] = 't';

    std::cout << name;

    anotherName = name;
    anotherName.at(2) = 'm';

    std::cout << anotherName;

   // for (auto& element : names) LOG(element)

    std::cout << "Enter three chars: " << std::endl;
    std::cout << name;

    Array<int> numList{4};

    for (int i = 0; i < numList.size(); ++i) {
        numList[i] = i + 1;
    }

    for (auto& n : numList) LOG(n)

    for (auto it = numList.rbegin(); it != numList.rend(); --it) {
        LOG(*it)
    }

    std::cout << "Enter " << numList.size() << " numbers:" << std::endl;
    std::cin >> numList;
    std::cout << numList;

    Array<std::string> nameList{4};

    std::cout << "Enter " << nameList.size() << " names:" << std::endl;
    std::cin >> nameList;
    std::cout << nameList;
}

int main() {
    testArrayClass();
}
