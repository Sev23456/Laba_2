#pragma once
#include "DynamicArray.h"
#include "LinkedList.h"
#include "Option.h"
#include <functional>

template<typename T>
class Sequence {
public:
    virtual ~Sequence() = default;
    
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(int index) const = 0;
    virtual int GetLength() const = 0;
    
    virtual Sequence<T>* Append(T item) = 0;
    virtual Sequence<T>* Prepend(T item) = 0;
    virtual Sequence<T>* InsertAt(T item, int index) = 0;
    virtual Sequence<T>* Concat(Sequence<T>* other) = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) = 0;
    
    // Map-Reduce
    virtual Sequence<T>* Map(std::function<T(T)> func) = 0;
    virtual Sequence<T>* Where(std::function<bool(T)> predicate) = 0;
    virtual T Reduce(std::function<T(T, T)> func, T initial) = 0;
    
    virtual Option<T> TryGetFirst(std::function<bool(T)> predicate = nullptr) = 0;
    virtual Option<T> TryGetLast(std::function<bool(T)> predicate = nullptr) = 0;
    
    virtual void Print() const = 0;
};