#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <locale>
#include <memory>
#include <functional>
#include <algorithm>
#include <map>
#include "ArraySequence.h"
#include "ListSequence.h"
#include "BitSequence.h"

// ============================================================================
// GLOBAL SEQUENCE STORAGE
// ============================================================================

enum class SequenceType {
    ARRAY_MUTABLE,
    ARRAY_IMMUTABLE,
    LIST_MUTABLE,
    LIST_IMMUTABLE,
    BIT
};

struct SequenceContainer {
    SequenceType type;
    Sequence<int>* seqInt;
    BitSequence* seqBit;
    std::string name;
    
    SequenceContainer() : type(SequenceType::ARRAY_MUTABLE), seqInt(nullptr), seqBit(nullptr) {}
    
    ~SequenceContainer() {
        if (seqInt) delete seqInt;
        if (seqBit) delete seqBit;
    }
};

std::map<int, SequenceContainer> sequences;
int nextSequenceId = 1;

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void ClearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void PrintHeader(const std::string& title) {
    std::cout << "\n";
    std::cout << title;
    int padding = 55 - title.length();
    for (int i = 0; i < padding; i++) std::cout << " ";
}

void PrintSeparator() {
    std::cout << "------------------------------------------------------------" << std::endl;
}

int GetValidSequenceId() {
    if (sequences.empty()) {
        std::cout << "Warning: No sequences available. Create one first!" << std::endl;
        return -1;
    }
    
    std::cout << "\nAvailable sequences:" << std::endl;
    for (const auto& pair : sequences) {
        std::cout << "  [" << pair.first << "] " << pair.second.name 
                  << " (Length: " << (pair.second.seqInt ? pair.second.seqInt->GetLength() : 
                                      pair.second.seqBit ? pair.second.seqBit->GetLength() : 0) 
                  << ")" << std::endl;
    }
    
    int id;
    std::cout << "\nEnter sequence ID (0 to cancel): ";
    std::cin >> id;
    
    if (id == 0) return -1;
    if (sequences.find(id) == sequences.end()) {
        std::cout << "Error: Invalid sequence ID!" << std::endl;
        return -1;
    }
    
    return id;
}

std::vector<int> ParseIntVector(const std::string& input) {
    std::vector<int> result;
    std::string normalized = input;
    std::replace(normalized.begin(), normalized.end(), ',', ' ');
    std::stringstream ss(normalized);
    int value;
    
    while (ss >> value) {
        result.push_back(value);
    }
    
    return result;
}

// ============================================================================
// SEQUENCE CREATION
// ============================================================================

void CreateSequence() {
    PrintHeader("CREATE NEW SEQUENCE");
    
    std::cout << "\nSelect sequence type:" << std::endl;
    std::cout << "  1. ArraySequence (Mutable)" << std::endl;
    std::cout << "  2. ArraySequence (Immutable)" << std::endl;
    std::cout << "  3. ListSequence (Mutable)" << std::endl;
    std::cout << "  4. ListSequence (Immutable)" << std::endl;
    std::cout << "  5. BitSequence" << std::endl;
    std::cout << "  0. Cancel" << std::endl;
    
    int choice;
    std::cout << "\nChoice: ";
    std::cin >> choice;
    
    if (choice == 0) return;
    
    std::cout << "\nEnter sequence name: ";
    std::string name;
    std::cin >> name;
    
    int id = nextSequenceId++;
    sequences[id].name = name;
    
    try {
        if (choice == 1) {
            std::cout << "Enter initial elements (space-separated, e.g., 1 2 3): ";
            std::cin.ignore();
            std::string input;
            std::getline(std::cin, input);
            
            if (input.empty()) {
                sequences[id].seqInt = new ArraySequence<int>();
            } else {
                std::vector<int> values = ParseIntVector(input);
                sequences[id].seqInt = new ArraySequence<int>(values.data(), values.size());
            }
            sequences[id].type = SequenceType::ARRAY_MUTABLE;
        }
        else if (choice == 2) {
            std::cout << "Enter initial elements (space-separated, e.g., 1 2 3): ";
            std::cin.ignore();
            std::string input;
            std::getline(std::cin, input);
            
            if (input.empty()) {
                sequences[id].seqInt = new ImmutableArraySequence<int>();
            } else {
                std::vector<int> values = ParseIntVector(input);
                sequences[id].seqInt = new ImmutableArraySequence<int>(values.data(), values.size());
            }
            sequences[id].type = SequenceType::ARRAY_IMMUTABLE;
        }
        else if (choice == 3) {
            std::cout << "Enter initial elements (space-separated, e.g., 1 2 3): ";
            std::cin.ignore();
            std::string input;
            std::getline(std::cin, input);
            
            if (input.empty()) {
                sequences[id].seqInt = new ListSequence<int>();
            } else {
                std::vector<int> values = ParseIntVector(input);
                sequences[id].seqInt = new ListSequence<int>(values.data(), values.size());
            }
            sequences[id].type = SequenceType::LIST_MUTABLE;
        }
        else if (choice == 4) {
            std::cout << "Enter initial elements (space-separated, e.g., 1 2 3): ";
            std::cin.ignore();
            std::string input;
            std::getline(std::cin, input);
            
            if (input.empty()) {
                // Empty immutable sequences are created via the default constructor.
                std::cout << "\nCreated empty immutable list sequence." << std::endl;
                sequences[id].seqInt = new ImmutableListSequence<int>();
            } else {
                std::vector<int> values = ParseIntVector(input);
                sequences[id].seqInt = new ImmutableListSequence<int>(values.data(), values.size());
            }
            sequences[id].type = SequenceType::LIST_IMMUTABLE;
        }
        else if (choice == 5) {
            sequences[id].seqBit = new BitSequence();
            sequences[id].type = SequenceType::BIT;
            
            std::cout << "Enter initial bits (0 or 1, space-separated, e.g., 1 0 1): ";
            std::cin.ignore();
            std::string input;
            std::getline(std::cin, input);
            
            if (!input.empty()) {
                std::stringstream ss(input);
                int bit;
                while (ss >> bit) {
                    sequences[id].seqBit->Append(Bit(bit != 0));
                }
            }
        }
        
        std::cout << "\nSequence '" << name << "' created with ID: " << id << std::endl;
        
    } catch (std::exception& ex) {
        std::cout << "\nError creating sequence: " << ex.what() << std::endl;
        sequences.erase(id);
        nextSequenceId--;
    }
}

// ============================================================================
// SEQUENCE OPERATIONS
// ============================================================================

void ViewSequence() {
    PrintHeader("VIEW SEQUENCE");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    try {
        std::cout << "\nSequence '" << sequences[id].name << "' (ID: " << id << ")\n" << std::endl;
        
        if (sequences[id].type == SequenceType::BIT) {
            std::cout << "Type: BitSequence" << std::endl;
            std::cout << "Length: " << sequences[id].seqBit->GetLength() << std::endl;
            std::cout << "Content: ";
            sequences[id].seqBit->Print();
            std::cout << std::endl;
        } else {
            std::cout << "Type: ";
            switch (sequences[id].type) {
                case SequenceType::ARRAY_MUTABLE: std::cout << "ArraySequence (Mutable)"; break;
                case SequenceType::ARRAY_IMMUTABLE: std::cout << "ArraySequence (Immutable)"; break;
                case SequenceType::LIST_MUTABLE: std::cout << "ListSequence (Mutable)"; break;
                case SequenceType::LIST_IMMUTABLE: std::cout << "ListSequence (Immutable)"; break;
            }
            std::cout << std::endl;
            std::cout << "Length: " << sequences[id].seqInt->GetLength() << std::endl;
            std::cout << "Content: ";
            sequences[id].seqInt->Print();
            std::cout << std::endl;
            
            if (sequences[id].seqInt->GetLength() > 0) {
                std::cout << "First: " << sequences[id].seqInt->GetFirst() << std::endl;
                std::cout << "Last: " << sequences[id].seqInt->GetLast() << std::endl;
            }
        }
        
    } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

void AppendElement() {
    PrintHeader("APPEND ELEMENT");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    if (sequences[id].type == SequenceType::BIT) {
        int bit;
        std::cout << "\nEnter bit value (0 or 1): ";
        std::cin >> bit;
        
        try {
            sequences[id].seqBit->Append(Bit(bit != 0));
            std::cout << "\nElement appended successfully!" << std::endl;
        } catch (std::exception& ex) {
            std::cout << "\nError: " << ex.what() << std::endl;
        }
    } else {
        int value;
        std::cout << "\nEnter value to append: ";
        std::cin >> value;
        
        try {
            Sequence<int>* result = sequences[id].seqInt->Append(value);
            
            // For immutable sequences, we need to store the new sequence
            if (sequences[id].type == SequenceType::ARRAY_IMMUTABLE || 
                sequences[id].type == SequenceType::LIST_IMMUTABLE) {
                delete sequences[id].seqInt;
                sequences[id].seqInt = result;
            }
            
            std::cout << "\nElement appended successfully!" << std::endl;
        } catch (std::exception& ex) {
            std::cout << "\nError: " << ex.what() << std::endl;
        }
    }
}

void PrependElement() {
    PrintHeader("PREPEND ELEMENT");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    if (sequences[id].type == SequenceType::BIT) {
        int bit;
        std::cout << "\nEnter bit value (0 or 1): ";
        std::cin >> bit;
        
        try {
            sequences[id].seqBit->Prepend(Bit(bit != 0));
            std::cout << "\nElement prepended successfully!" << std::endl;
        } catch (std::exception& ex) {
            std::cout << "\nError: " << ex.what() << std::endl;
        }
    } else {
        int value;
        std::cout << "\nEnter value to prepend: ";
        std::cin >> value;
        
        try {
            Sequence<int>* result = sequences[id].seqInt->Prepend(value);
            
            if (sequences[id].type == SequenceType::ARRAY_IMMUTABLE || 
                sequences[id].type == SequenceType::LIST_IMMUTABLE) {
                delete sequences[id].seqInt;
                sequences[id].seqInt = result;
            }
            
            std::cout << "\nElement prepended successfully!" << std::endl;
        } catch (std::exception& ex) {
            std::cout << "\nError: " << ex.what() << std::endl;
        }
    }
}

void InsertAtPosition() {
    PrintHeader("INSERT AT POSITION");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    int index, value;
    std::cout << "\nEnter index: ";
    std::cin >> index;
    std::cout << "Enter value: ";
    std::cin >> value;
    
    try {
        if (sequences[id].type == SequenceType::BIT) {
            sequences[id].seqBit->InsertAt(Bit(value != 0), index);
        } else {
            Sequence<int>* result = sequences[id].seqInt->InsertAt(value, index);
            
            if (sequences[id].type == SequenceType::ARRAY_IMMUTABLE || 
                sequences[id].type == SequenceType::LIST_IMMUTABLE) {
                delete sequences[id].seqInt;
                sequences[id].seqInt = result;
            }
        }
        
            std::cout << "\nElement inserted successfully!" << std::endl;
    } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

void GetElementByIndex() {
    PrintHeader("GET ELEMENT BY INDEX");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    int index;
    std::cout << "\nEnter index: ";
    std::cin >> index;
    
    try {
        if (sequences[id].type == SequenceType::BIT) {
            Bit bit = sequences[id].seqBit->Get(index);
            std::cout << "\nElement at index " << index << ": " << (bit.GetValue() ? 1 : 0) << std::endl;
        } else {
            int value = sequences[id].seqInt->Get(index);
            std::cout << "\nElement at index " << index << ": " << value << std::endl;
        }
        } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

void GetSubsequence() {
    PrintHeader("GET SUBSEQUENCE");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    int startIndex, endIndex;
    std::cout << "\nEnter start index: ";
    std::cin >> startIndex;
    std::cout << "Enter end index: ";
    std::cin >> endIndex;
    
    try {
        if (sequences[id].type == SequenceType::BIT) {
            Sequence<Bit>* sub = sequences[id].seqBit->GetSubsequence(startIndex, endIndex);
            std::cout << "\nSubsequence: ";
            sub->Print();
            std::cout << std::endl;
            delete sub;
        } else {
            Sequence<int>* sub = sequences[id].seqInt->GetSubsequence(startIndex, endIndex);
            std::cout << "\nSubsequence: ";
            sub->Print();
            std::cout << std::endl;
            
            // Ask if user wants to save as new sequence
            std::cout << "\nSave as new sequence? (1=Yes, 0=No): ";
            int save;
            std::cin >> save;
            
            if (save == 1) {
                std::cout << "Enter name for new sequence: ";
                std::string name;
                std::cin >> name;
                
                int newId = nextSequenceId++;
                sequences[newId].name = name;
                sequences[newId].seqInt = sub;
                sequences[newId].type = sequences[id].type;
                
                std::cout << "\nSaved as sequence ID: " << newId << std::endl;
            } else {
                delete sub;
            }
        }
        } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

// ============================================================================
// MAP-REDUCE OPERATIONS
// ============================================================================

void MapOperation() {
    PrintHeader("MAP OPERATION");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    std::cout << "\nSelect transformation:" << std::endl;
    std::cout << "  1. Multiply by 2 (x * 2)" << std::endl;
    std::cout << "  2. Add 10 (x + 10)" << std::endl;
    std::cout << "  3. Square (x * x)" << std::endl;
    std::cout << "  4. Negate (-x)" << std::endl;
    std::cout << "  5. Custom (enter multiplier)" << std::endl;
    
    int choice;
    std::cout << "\nChoice: ";
    std::cin >> choice;
    
    try {
        std::function<int(int)> func;
        
        switch (choice) {
            case 1: func = [](int x) { return x * 2; }; break;
            case 2: func = [](int x) { return x + 10; }; break;
            case 3: func = [](int x) { return x * x; }; break;
            case 4: func = [](int x) { return -x; }; break;
            case 5: {
                int multiplier;
                std::cout << "Enter multiplier: ";
                std::cin >> multiplier;
                func = [multiplier](int x) { return x * multiplier; };
                break;
            }
            default: std::cout << "Invalid choice!" << std::endl; return;
        }
        
        if (sequences[id].type == SequenceType::BIT) {
            sequences[id].seqBit->Map([func](Bit b) { return Bit(b.GetValue() ? func(1) : func(0)); });
        } else {
            Sequence<int>* result = sequences[id].seqInt->Map(func);
            
            if (sequences[id].type == SequenceType::ARRAY_IMMUTABLE || 
                sequences[id].type == SequenceType::LIST_IMMUTABLE) {
                delete sequences[id].seqInt;
                sequences[id].seqInt = result;
            }
        }
        
        std::cout << "\nMap operation completed!" << std::endl;
        
    } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

void WhereOperation() {
    PrintHeader("WHERE (FILTER) OPERATION");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    std::cout << "\nSelect filter:" << std::endl;
    std::cout << "  1. Even numbers (x % 2 == 0)" << std::endl;
    std::cout << "  2. Odd numbers (x % 2 != 0)" << std::endl;
    std::cout << "  3. Positive (x > 0)" << std::endl;
    std::cout << "  4. Negative (x < 0)" << std::endl;
    std::cout << "  5. Greater than (custom)" << std::endl;
    std::cout << "  6. Less than (custom)" << std::endl;
    
    int choice;
    std::cout << "\nChoice: ";
    std::cin >> choice;
    
    try {
        std::function<bool(int)> predicate;
        
        switch (choice) {
            case 1: predicate = [](int x) { return x % 2 == 0; }; break;
            case 2: predicate = [](int x) { return x % 2 != 0; }; break;
            case 3: predicate = [](int x) { return x > 0; }; break;
            case 4: predicate = [](int x) { return x < 0; }; break;
            case 5: {
                int threshold;
                std::cout << "Enter threshold: ";
                std::cin >> threshold;
                predicate = [threshold](int x) { return x > threshold; };
                break;
            }
            case 6: {
                int threshold;
                std::cout << "Enter threshold: ";
                std::cin >> threshold;
                predicate = [threshold](int x) { return x < threshold; };
                break;
            }
            default: std::cout << "Invalid choice!" << std::endl; return;
        }
        
        if (sequences[id].type == SequenceType::BIT) {
            sequences[id].seqBit->Where([predicate](Bit b) { return predicate(b.GetValue() ? 1 : 0); });
        } else {
            Sequence<int>* result = sequences[id].seqInt->Where(predicate);
            
            if (sequences[id].type == SequenceType::ARRAY_IMMUTABLE || 
                sequences[id].type == SequenceType::LIST_IMMUTABLE) {
                delete sequences[id].seqInt;
                sequences[id].seqInt = result;
            }
        }
        
        std::cout << "\nFilter operation completed!" << std::endl;
        
    } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

void ReduceOperation() {
    PrintHeader("REDUCE OPERATION");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    std::cout << "\nSelect reduction:" << std::endl;
    std::cout << "  1. Sum" << std::endl;
    std::cout << "  2. Product" << std::endl;
    std::cout << "  3. Maximum" << std::endl;
    std::cout << "  4. Minimum" << std::endl;
    
    int choice;
    std::cout << "\nChoice: ";
    std::cin >> choice;
    
    try {
        if (sequences[id].type == SequenceType::BIT) {
            Bit result = sequences[id].seqBit->Reduce([](Bit a, Bit b) { return Bit(a.GetValue() || b.GetValue()); }, Bit(0));
            std::cout << "\nResult: " << (result.GetValue() ? 1 : 0) << std::endl;
        } else {
            std::function<int(int, int)> func;
            int initial = 0;
            
            switch (choice) {
                case 1: 
                    func = [](int a, int b) { return a + b; }; 
                    initial = 0;
                    break;
                case 2: 
                    func = [](int a, int b) { return a * b; }; 
                    initial = 1;
                    break;
                case 3: 
                    func = [](int a, int b) { return (a > b) ? a : b; }; 
                    if (sequences[id].seqInt->GetLength() > 0)
                        initial = sequences[id].seqInt->Get(0);
                    break;
                case 4: 
                    func = [](int a, int b) { return (a < b) ? a : b; }; 
                    if (sequences[id].seqInt->GetLength() > 0)
                        initial = sequences[id].seqInt->Get(0);
                    break;
                default: std::cout << "Invalid choice!" << std::endl; return;
            }
            
            int result = sequences[id].seqInt->Reduce(func, initial);
            std::cout << "\nResult: " << result << std::endl;
        }
        
    } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

void StatisticsOperation() {
    PrintHeader("STATISTICS (1 PASS)");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    try {
        if (sequences[id].type == SequenceType::BIT) {
            int len = sequences[id].seqBit->GetLength();
            if (len == 0) {
                std::cout << "\nStatistics: sequence is empty." << std::endl;
            } else {
                int minV = 1, maxV = 0;
                int sum = 0;
                for (int i = 0; i < len; i++) {
                    int v = sequences[id].seqBit->Get(i).GetValue() ? 1 : 0;
                    if (v < minV) minV = v;
                    if (v > maxV) maxV = v;
                    sum += v;
                }
                double avg = (double)sum / len;
                std::cout << "\nStatistics:" << std::endl;
                std::cout << "  Min: " << minV << std::endl;
                std::cout << "  Max: " << maxV << std::endl;
                std::cout << "  Avg: " << avg << std::endl;
            }
        } else {
            int len = sequences[id].seqInt->GetLength();
            if (len == 0) {
                std::cout << "\nStatistics: sequence is empty." << std::endl;
            } else {
                int minV = sequences[id].seqInt->Get(0);
                int maxV = minV;
                long long sum = 0;
                for (int i = 0; i < len; i++) {
                    int v = sequences[id].seqInt->Get(i);
                    if (v < minV) minV = v;
                    if (v > maxV) maxV = v;
                    sum += v;
                }
                double avg = (double)sum / len;
                std::cout << "\nStatistics:" << std::endl;
                std::cout << "  Min: " << minV << std::endl;
                std::cout << "  Max: " << maxV << std::endl;
                std::cout << "  Avg: " << avg << std::endl;
            }
        }
    } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

void TryGetOperation() {
    PrintHeader("TRY-GET OPERATION (Option<T>)");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    if (sequences[id].type == SequenceType::BIT) {
        std::cout << "\nError: This operation is only available for integer sequences (not BitSequence)!" << std::endl;
        return;
    }
    
    std::cout << "\nSelect search:" << std::endl;
    std::cout << "  1. First element" << std::endl;
    std::cout << "  2. Last element" << std::endl;
    std::cout << "  3. First element > threshold" << std::endl;
    std::cout << "  4. First element < threshold" << std::endl;
    
    int choice;
    std::cout << "\nChoice: ";
    std::cin >> choice;
    
    try {
        Option<int> result;
        
        switch (choice) {
            case 1:
                result = sequences[id].seqInt->TryGetFirst();
                break;
            case 2:
                result = sequences[id].seqInt->TryGetLast();
                break;
            case 3: {
                int threshold;
                std::cout << "Enter threshold: ";
                std::cin >> threshold;
                result = sequences[id].seqInt->TryGetFirst([threshold](int x) { return x > threshold; });
                break;
            }
            case 4: {
                int threshold;
                std::cout << "Enter threshold: ";
                std::cin >> threshold;
                result = sequences[id].seqInt->TryGetFirst([threshold](int x) { return x < threshold; });
                break;
            }
            default: std::cout << "Invalid choice!" << std::endl; return;
        }
        
        if (result.IsSome()) {
            std::cout << "\nFound: " << result.GetValue() << std::endl;
        } else {
            std::cout << "\nNot found (Option::None)" << std::endl;
        }
        
    } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

// ============================================================================
// BIT SEQUENCE OPERATIONS
// ============================================================================

void BitwiseOperation() {
    PrintHeader("BITWISE OPERATIONS");
    
    int id1 = GetValidSequenceId();
    if (id1 == -1) return;
    
    if (sequences[id1].type != SequenceType::BIT) {
        std::cout << "\nError: This operation is only available for BitSequence!" << std::endl;
        return;
    }
    
    std::cout << "\nSelect operation:" << std::endl;
    std::cout << "  1. AND with another BitSequence" << std::endl;
    std::cout << "  2. OR with another BitSequence" << std::endl;
    std::cout << "  3. XOR with another BitSequence" << std::endl;
    std::cout << "  4. NOT (complement)" << std::endl;
    
    int choice;
    std::cout << "\nChoice: ";
    std::cin >> choice;
    
    try {
        if (choice == 4) {
            BitSequence* result = sequences[id1].seqBit->BitNot();
            std::cout << "\nResult: ";
            result->Print();
            std::cout << std::endl;
            
            std::cout << "\nSave as new sequence? (1=Yes, 0=No): ";
            int save;
            std::cin >> save;
            
            if (save == 1) {
                std::cout << "Enter name: ";
                std::string name;
                std::cin >> name;
                
                int newId = nextSequenceId++;
                sequences[newId].name = name;
                sequences[newId].seqBit = result;
                sequences[newId].type = SequenceType::BIT;
                
                std::cout << "\nSaved as sequence ID: " << newId << std::endl;
            } else {
                delete result;
            }
        } else {
            std::cout << "\nSelect second BitSequence:" << std::endl;
            int id2 = GetValidSequenceId();
            if (id2 == -1) return;
            
            if (sequences[id2].type != SequenceType::BIT) {
                std::cout << "\nError: Second sequence must also be BitSequence!" << std::endl;
                return;
            }
            
            BitSequence* result = nullptr;
            
            switch (choice) {
                case 1: result = sequences[id1].seqBit->BitAnd(*sequences[id2].seqBit); break;
                case 2: result = sequences[id1].seqBit->BitOr(*sequences[id2].seqBit); break;
                case 3: result = sequences[id1].seqBit->BitXor(*sequences[id2].seqBit); break;
            }
            
            std::cout << "\nResult: ";
            result->Print();
            std::cout << std::endl;
            
            std::cout << "\nSave as new sequence? (1=Yes, 0=No): ";
            int save;
            std::cin >> save;
            
            if (save == 1) {
                std::cout << "Enter name: ";
                std::string name;
                std::cin >> name;
                
                int newId = nextSequenceId++;
                sequences[newId].name = name;
                sequences[newId].seqBit = result;
                sequences[newId].type = SequenceType::BIT;
                
                std::cout << "\nSaved as sequence ID: " << newId << std::endl;
            } else {
                delete result;
            }
        }
        
    } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

// ============================================================================
// ITERATOR DEMONSTRATION
// ============================================================================

void IteratorDemo() {
    PrintHeader("ITERATOR DEMONSTRATION");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    try {
        std::cout << "\nIterating through sequence:" << std::endl;
        std::cout << "[";
        
        if (sequences[id].type == SequenceType::BIT) {
            int len = sequences[id].seqBit->GetLength();
            for (int i = 0; i < len; i++) {
                if (i) std::cout << ", ";
                std::cout << (sequences[id].seqBit->Get(i).GetValue() ? 1 : 0);
            }
        } else {
            int len = sequences[id].seqInt->GetLength();
            for (int i = 0; i < len; i++) {
                if (i) std::cout << ", ";
                std::cout << sequences[id].seqInt->Get(i);
            }
        }
        
        std::cout << "]" << std::endl;
        
    } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

// ============================================================================
// SEQUENCE MANAGEMENT
// ============================================================================

void DeleteSequence() {
    PrintHeader("DELETE SEQUENCE");
    
    int id = GetValidSequenceId();
    if (id == -1) return;
    
    std::cout << "\nAre you sure you want to delete '" << sequences[id].name << "'? (1=Yes, 0=No): ";
    int confirm;
    std::cin >> confirm;
    
    if (confirm == 1) {
        sequences.erase(id);
        std::cout << "\nSequence deleted successfully!" << std::endl;
    }
}

void ConcatenateSequences() {
    PrintHeader("CONCATENATE SEQUENCES");
    
    int id1 = GetValidSequenceId();
    if (id1 == -1) return;
    
    std::cout << "\nSelect second sequence:" << std::endl;
    int id2 = GetValidSequenceId();
    if (id2 == -1) return;
    
    if (sequences[id1].type != sequences[id2].type) {
        std::cout << "\nError: Sequences must be of the same type!" << std::endl;
        return;
    }
    
    try {
        if (sequences[id1].type == SequenceType::BIT) {
            sequences[id1].seqBit->Concat(sequences[id2].seqBit);
        } else {
            Sequence<int>* result = sequences[id1].seqInt->Concat(sequences[id2].seqInt);
            
            if (sequences[id1].type == SequenceType::ARRAY_IMMUTABLE || 
                sequences[id1].type == SequenceType::LIST_IMMUTABLE) {
                delete sequences[id1].seqInt;
                sequences[id1].seqInt = result;
            }
        }
        
        std::cout << "\nSequences concatenated successfully!" << std::endl;
        
    } catch (std::exception& ex) {
        std::cout << "\nError: " << ex.what() << std::endl;
    }
}

// ============================================================================
// MAIN MENU
// ============================================================================

void PrintMainMenu() {
    std::cout << "\n";
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "|           LINEAR DATA STRUCTURES SYSTEM                  |" << std::endl;
    std::cout << "|                    Lab Work #2                           |" << std::endl;
    std::cout << "+----------------------------------------------------------+" << std::endl;
    std::cout << "\n";
    std::cout << "SEQUENCE MANAGEMENT:" << std::endl;
    std::cout << "  1.  Create new sequence" << std::endl;
    std::cout << "  2.  View sequence" << std::endl;
    std::cout << "  3.  Delete sequence" << std::endl;
    std::cout << "  4.  Concatenate two sequences" << std::endl;
    std::cout << "\n";
    std::cout << "ELEMENT OPERATIONS:" << std::endl;
    std::cout << "  5.  Append element" << std::endl;
    std::cout << "  6.  Prepend element" << std::endl;
    std::cout << "  7.  Insert at position" << std::endl;
    std::cout << "  8.  Get element by index" << std::endl;
    std::cout << "  9.  Get subsequence" << std::endl;
    std::cout << "\n";
    std::cout << "MAP-REDUCE OPERATIONS:" << std::endl;
    std::cout << "  10. Map (transform)" << std::endl;
    std::cout << "  11. Where (filter)" << std::endl;
    std::cout << "  12. Reduce (fold)" << std::endl;
    std::cout << "  13. Statistics (Min/Max/Avg)" << std::endl;
    std::cout << "  14. Try-Get (Option<T>)" << std::endl;
    std::cout << "\n";
    std::cout << "BIT SEQUENCE OPERATIONS:" << std::endl;
    std::cout << "  15. Bitwise operations (AND/OR/XOR/NOT)" << std::endl;
    std::cout << "\n";
    std::cout << "ADVANCED:" << std::endl;
    std::cout << "  16. Iterator demonstration" << std::endl;
    std::cout << "\n";
    std::cout << "  0.  Exit" << std::endl;
    std::cout << "\n";
    PrintSeparator();
}

int main() {
    std::setlocale(LC_ALL, "English");
    
    int choice;
    
    do {
        PrintMainMenu();
        std::cout << "Enter choice: ";
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1:  CreateSequence(); break;
                case 2:  ViewSequence(); break;
                case 3:  DeleteSequence(); break;
                case 4:  ConcatenateSequences(); break;
                case 5:  AppendElement(); break;
                case 6:  PrependElement(); break;
                case 7:  InsertAtPosition(); break;
                case 8:  GetElementByIndex(); break;
                case 9:  GetSubsequence(); break;
                case 10: MapOperation(); break;
                case 11: WhereOperation(); break;
                case 12: ReduceOperation(); break;
                case 13: StatisticsOperation(); break;
                case 14: TryGetOperation(); break;
                case 15: BitwiseOperation(); break;
                case 16: IteratorDemo(); break;
                case 0:  
                    std::cout << "\nGoodbye!" << std::endl; 
                    break;
                default: 
                    std::cout << "\nInvalid choice! Please try again." << std::endl;
            }
            
            if (choice != 0) {
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }
            
        } catch (std::exception& ex) {
            std::cout << "\nCritical error: " << ex.what() << std::endl;
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore();
            std::cin.get();
        }
        
    } while (choice != 0);
    
    // Cleanup
    // SequenceContainer destructor will free contained pointers; just clear the map.
    sequences.clear();
    
    return 0;
}
