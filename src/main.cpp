#include <iostream>
#include <string>
#include "ArraySequence.h"
#include "ListSequence.h"
#include "BitSequence.h"

void PrintMenu() {
    std::cout << "\n=== Lab work #2 ===" << std::endl;
    std::cout << "1. Test ArraySequence" << std::endl;
    std::cout << "2. Test ListSequence" << std::endl;
    std::cout << "3. Test BitSequence" << std::endl;
    std::cout << "4. Test Map-Reduce" << std::endl;
    std::cout << "5. Test Immutable" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choice: ";
}

void TestArraySequence() {
    std::cout << "\n--- ArraySequence ---" << std::endl;
    int arr[] = {1, 2, 3, 4, 5};
    ArraySequence<int>* seq = new ArraySequence<int>(arr, 5);
    
    std::cout << "Original: ";
    seq->Print();
    
    seq->Append(6);
    std::cout << "After Append(6): ";
    seq->Print();
    
    seq->Prepend(0);
    std::cout << "After Prepend(0): ";
    seq->Print();
    
    std::cout << "Get(3) = " << seq->Get(3) << std::endl;
    std::cout << "GetFirst() = " << seq->GetFirst() << std::endl;
    std::cout << "GetLast() = " << seq->GetLast() << std::endl;
    std::cout << "Length = " << seq->GetLength() << std::endl;
    
    delete seq;
}

void TestListSequence() {
    std::cout << "\n--- ListSequence ---" << std::endl;
    int arr[] = {10, 20, 30};
    ListSequence<int>* seq = new ListSequence<int>(arr, 3);
    
    std::cout << "Original: ";
    seq->Print();
    
    seq->Append(40);
    std::cout << "After Append(40): ";
    seq->Print();
    
    delete seq;
}

void TestBitSequence() {
    std::cout << "\n--- BitSequence ---" << std::endl;
    BitSequence* bs1 = new BitSequence();
    bs1->Append(Bit(1));
    bs1->Append(Bit(0));
    bs1->Append(Bit(1));
    bs1->Append(Bit(1));
    
    BitSequence* bs2 = new BitSequence();
    bs2->Append(Bit(1));
    bs2->Append(Bit(1));
    bs2->Append(Bit(0));
    bs2->Append(Bit(0));
    
    std::cout << "BS1: "; bs1->Print();
    std::cout << "BS2: "; bs2->Print();
    
    std::cout << "BS1 AND BS2: "; bs1->BitAnd(*bs2)->Print();
    std::cout << "BS1 OR BS2: "; bs1->BitOr(*bs2)->Print();
    std::cout << "BS1 XOR BS2: "; bs1->BitXor(*bs2)->Print();
    std::cout << "NOT BS1: "; bs1->BitNot()->Print();
    
    delete bs1;
    delete bs2;
}

void TestMapReduce() {
    std::cout << "\n--- Map-Reduce ---" << std::endl;
    int arr[] = {1, 2, 3, 4, 5};
    ArraySequence<int>* seq = new ArraySequence<int>(arr, 5);
    
    std::cout << "Original: ";
    seq->Print();
    
    // Map: multiply by 2
    seq->Map([](int x) { return x * 2; });
    std::cout << "After Map(x*2): ";
    seq->Print();
    
    // Where: keep only even
    seq->Where([](int x) { return x % 2 == 0; });
    std::cout << "After Where(even): ";
    seq->Print();
    
    // Reduce: sum
    int sum = seq->Reduce([](int a, int b) { return a + b; }, 0);
    std::cout << "Reduce(sum) = " << sum << std::endl;
    
    // TryGet
    Option<int> first = seq->TryGetFirst();
    if (first.IsSome()) {
        std::cout << "TryGetFirst = " << first.GetValue() << std::endl;
    }
    
    delete seq;
}

void TestImmutable() {
    std::cout << "\n--- Immutable Sequence ---" << std::endl;
    int arr[] = {1, 2, 3};
    ImmutableArraySequence<int>* seq1 = new ImmutableArraySequence<int>(arr, 3);
    
    std::cout << "seq1: ";
    seq1->Print();
    
    ImmutableArraySequence<int>* seq2 = (ImmutableArraySequence<int>*)seq1->Append(4);
    
    std::cout << "seq1 after Append (unchanged): ";
    seq1->Print();
    std::cout << "seq2 (new): ";
    seq2->Print();
    
    delete seq1;
    delete seq2;
}

int main() {
    setlocale(LC_ALL, "");
    
    while (true) {
        PrintMenu();
        int choice;
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1: TestArraySequence(); break;
                case 2: TestListSequence(); break;
                case 3: TestBitSequence(); break;
                case 4: TestMapReduce(); break;
                case 5: TestImmutable(); break;
                case 0: std::cout << "Exiting..." << std::endl; return 0;
                default: std::cout << "Invalid choice!" << std::endl;
            }
        } catch (std::exception& ex) {
            std::cout << "Error: " << ex.what() << std::endl;
        }
    }
    
    return 0;
}