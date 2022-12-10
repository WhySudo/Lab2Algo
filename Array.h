#ifndef LAB2ALGO_ARRAY_H
#define LAB2ALGO_ARRAY_H

#include <iostream>
#include <utility>


using namespace std;

template<typename T>
class Array final {
private:

    const size_t _defaultCapacity = 8;
    const size_t _capacityExtension = 2;


    T *_dataPtr;
    size_t _dataSize;
    size_t _capacity;
public:
    Array();
    Array(int capacity);
    Array(const Array &other);
    Array(Array &&other) noexcept;
    Array &operator=(const Array &other);
    Array &operator=( Array &&other) noexcept;
    ~Array();
    const T &operator[](int index) const;

    T &operator[](int index);

    int insert(const T &value);
    int insert(int index, const T &value);
    void remove(int index);
    int size() const;

    class Iterator {
    protected:
        int _curId;
        int _direction;
        Array<T> *arrayPtr;
    public:
        Iterator(Array<T> *array, int startIdx, int direction);

        const T &get() const;

        void set(const T &value);

        void next();

        bool hasNext() const;
    };

    class ConstIterator {
    protected:
        int _curId;
        int _direction;
        Array<T> *arrayPtr;
    public:
        ConstIterator(Array<T> *array, int startIdx, int direction);
        const T &get() const;
        void next();
        bool hasNext() const;
    };
public:
    Iterator iterator();
    ConstIterator iterator() const;
    Iterator reverseIterator();
    ConstIterator reverseIterator() const;


};

#pragma region Array
template<typename T>
Array<T>::Array() {
    _capacity = _defaultCapacity;
    _dataPtr = (T *) malloc(_capacity * sizeof(T));
    _dataSize = 0;
}

template<typename T>
Array<T>::Array(int capacity) {
    _capacity = capacity;
    _dataPtr = (T *) malloc(capacity * sizeof(T));
    _dataSize = 0;
}

template<typename T>
Array<T>::~Array() {
    for (int i = 0; i < _dataSize; ++i) {
        _dataPtr[i].~T();
    }
    if(_dataPtr != nullptr)
        free(_dataPtr);
}
template<typename T>
Array<T>::Array(const Array &other) {
    _dataSize = other._dataSize;
    _capacity = other._capacity;
    _dataPtr = (T*)malloc(sizeof(T)*_capacity);
    for (int i = 0; i < _dataSize; ++i) {
        new(_dataPtr + i) T(other._dataPtr[i]);
    }
}
template<typename T>
Array<T>::Array(Array &&other) noexcept{
    _dataPtr = other._dataPtr;
    _capacity = other._capacity;
    _dataSize = other._dataSize;
    other._dataPtr = nullptr;
    other._dataSize = 0;
    other._capacity = 0;
}
template<typename T>
Array<T> &Array<T>::operator=(const Array& other) {
    Array temp = other;
    swap(_dataPtr, temp._dataPtr);
    swap(_dataSize, temp._dataSize);
    swap(_capacity, temp._capacity);
    return *this;
}
template<typename T>
Array<T> &Array<T>::operator=(Array &&other)  noexcept {
    if (this == &other)
        return *this;
    for (int i = 0; i < _dataSize; ++i) {
        _dataPtr[i].~T();
    }
    if(_dataPtr != nullptr){
        free(_dataPtr);
    }

    _dataSize = other._dataSize;
    _dataPtr = other._dataPtr;
    _capacity = other._capacity;
    other._dataPtr = nullptr;
    other._dataSize = 0;
    other._capacity = 0;

    return *this;
}

template<typename T>
int Array<T>::insert(const T &value) {
    if (_dataSize >= _capacity) {
        _capacity *= _capacityExtension;
        T *tempPtr = (T *) malloc(_capacity * sizeof(T));
        for (int i = 0; i < _dataSize; i++) {
            new(tempPtr + i) T(std::move(_dataPtr[i]));
            //tempPtr[i]
            _dataPtr[i].~T();
        }
        free(_dataPtr);
        _dataPtr = tempPtr;
    }
    new(_dataPtr + _dataSize) T(value);
    _dataSize++;
    return _dataSize - 1;
}
template<typename T>
int Array<T>::insert(int index, const T &value) {
    if (_dataSize >= _capacity) {
        _capacity *= _capacityExtension;
        T *tempPtr = (T*)malloc(_capacity * sizeof(T));
        for (int i = 0; i < index; i++) {
            new(tempPtr+i) T(std::move(_dataPtr[i]));
            _dataPtr[i].~T();
        }
        new(tempPtr+index) T(value);
        for (int i = index + 1; i < _dataSize + 1; i++) {
            new(tempPtr+i) T(std::move(_dataPtr[i-1]));
            _dataPtr[i-1].~T();
        }
        free(_dataPtr);
        _dataPtr = tempPtr;
    } else {
        for (int i = _dataSize; i >= index + 1; i--) {
            new(_dataPtr+i) T(std::move(_dataPtr[i - 1]));
        }
        if(_dataSize!=index){
            _dataPtr[index].~T();
        }
        new(_dataPtr + index) T(value);
    }
    _dataSize++;
    return index;
}
template<typename T>
void Array<T>::remove(int index) {
    if (_dataSize == 0) {
        throw runtime_error("Can't remove from empty array");
    }
    if (index >= _dataSize) {
        throw runtime_error("Out of bounds");
    }
    for (int i = index; i < _dataSize - 1; ++i) {
        _dataPtr[i] = std::move(_dataPtr[i + 1]);
    }
    _dataSize -= 1;
}

template<typename T>
const T& Array<T>::operator[](int index) const {
    return _dataPtr[index];
}

template<typename T>
T& Array<T>::operator[](int index) {
    return _dataPtr[index];
}

template<typename T>
int Array<T>::size() const {
    return _dataSize;
}

template<typename T>
typename Array<T>::Iterator Array<T>::iterator() {
    return Iterator(this, 0, 1);
}
template<typename T>
typename Array<T>::ConstIterator Array<T>::iterator() const{
    return ConstIterator(this, 0, 1);
}

template<typename T>
typename Array<T>::Iterator Array<T>::reverseIterator() {
    return Iterator(this, _dataSize -1, -1);
}

template<typename T>
typename Array<T>::ConstIterator Array<T>::reverseIterator() const {
    return ConstIterator(this, _dataSize -1, -1);
}



#pragma endregion Array

#pragma region Iterator

template<typename T>
Array<T>::Iterator::Iterator(Array<T> *array, int startIdx, int direction) {
    arrayPtr = array;
    _curId = startIdx;
    _direction = direction;
}

template<typename T>
void Array<T>::Iterator::next() {
    if (!hasNext()) throw std::runtime_error("Iterator end");
    _curId += _direction;
}

template<typename T>
const T &Array<T>::Iterator::get() const {
    return arrayPtr->_dataPtr[_curId];
}

template<typename T>
void Array<T>::Iterator::set(const T &value) {
    arrayPtr->_dataPtr[_curId] = value;
}

template<typename T>
bool Array<T>::Iterator::hasNext() const {
    int targetId = _curId + _direction;
    return (targetId >= 0 && targetId < arrayPtr->size());
}


#pragma endregion Iterator

#pragma region ConstIterator

template<typename T>
Array<T>::ConstIterator::ConstIterator(Array<T> *array, int startIdx, int direction) {
    arrayPtr = array;
    _curId = startIdx;
    _direction = direction;
}

template<typename T>
const T &Array<T>::ConstIterator::get() const {
    return arrayPtr->_dataPtr[_curId];
}
template<typename T>
void Array<T>::ConstIterator::next() {
    if (!hasNext()) throw std::runtime_error("Iterator end");
    _curId += _direction;
}
template<typename T>
bool Array<T>::ConstIterator::hasNext() const {
    int targetId = _curId + _direction;
    return (targetId >= 0 && targetId < arrayPtr->size());
}

#pragma endregion ConstIterator
#endif //LAB2ALGO_ARRAY_H
