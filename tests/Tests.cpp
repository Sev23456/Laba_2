#include <iostream>
#include <cassert>
#include "../include/ArraySequence.h"
#include "../include/ListSequence.h"
#include "../include/BitSequence.h"

void TestDynamicArray() {
    std::cout << "Testing DynamicArray... ";
    DynamicArray<int> arr(5);
    for (int i = 0; i < 5; i++) arr.Set(i, i * 10);
    assert(arr.Get(0) == 0);
    assert(arr.Get(4) == 40);
    assert(arr.GetSize() == 5);
    std::cout << "OK" << std::endl;
}

void TestArraySequence() {
    std::cout << "Testing ArraySequence... ";
    int data[] = {1, 2, 3};
    ArraySequence<int>* seq = new ArraySequence<int>(data, 3);
    assert(seq->GetLength() == 3);
    assert(seq->Get(0) == 1);
    assert(seq->GetFirst() == 1);
    assert(seq->GetLast() == 3);
    seq->Append(4);
    assert(seq->GetLength() == 4);
    delete seq;
    std::cout << "OK" << std::endl;
}

void TestListSequence() {
    std::cout << "Testing ListSequence... ";
    int data[] = {10, 20, 30};
    ListSequence<int>* seq = new ListSequence<int>(data, 3);
    assert(seq->GetLength() == 3);
    assert(seq->Get(1) == 20);
    seq->Prepend(5);
    assert(seq->GetFirst() == 5);
    delete seq;
    std::cout << "OK" << std::endl;
}

void TestExceptions() {
    std::cout << "Testing Exceptions... ";
    ArraySequence<int>* seq = new ArraySequence<int>();
    bool caught = false;
    try {
        seq->Get(0);
    } catch (IndexOutOfRangeException&) {
        caught = true;
    }
    assert(caught);
    delete seq;
    std::cout << "OK" << std::endl;
}

void TestMapReduce() {
    std::cout << "Testing Map-Reduce... ";
    int data[] = {1, 2, 3, 4, 5};
    ArraySequence<int>* seq = new ArraySequence<int>(data, 5);
    
    int sum = seq->Reduce([](int a, int b) { return a + b; }, 0);
    assert(sum == 15);
    
    seq->Where([](int x) { return x % 2 == 0; });
    assert(seq->GetLength() == 2);
    
    delete seq;
    std::cout << "OK" << std::endl;
}

void TestBitSequence() {
    std::cout << "Testing BitSequence... ";
    BitSequence* bs = new BitSequence();
    bs->Append(Bit(1));
    bs->Append(Bit(0));
    assert(bs->GetLength() == 2);
    assert(bs->Get(0).GetValue() == true);
    delete bs;
    std::cout << "OK" << std::endl;
}

int main() {
    setlocale(LC_ALL, "");
    std::cout << "=== Running tests ===" << std::endl;
    
    TestDynamicArray();
    TestArraySequence();
    TestListSequence();
    TestExceptions();
    TestMapReduce();
    TestBitSequence();
    
    std::cout << "\nAll tests passed!" << std::endl;
    return 0;
}