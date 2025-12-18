# Design and Implementation of Hash Table
## Course Design Report

---

### 1. Project Overview

**Project Title**: Hash Table Implementation for Phone Directory Management

**Development Environment**: 
- Platform: Windows
- Compiler: g++ (MinGW) / Visual C++
- IDE: Visual Studio Code / Visual Studio
- Language: C++17

**Project Structure**:
```
AdvancedHashTable/
├── include/          (Header files)
├── src/              (Implementation files)
├── data/             (Test data - 30 records)
├── test/             (Unit tests)
└── report/           (Documentation)
```

---

### 2. Problem Analysis

**Objective**: Design and implement a hash table data structure for managing a unit's phone directory with efficient insertion, searching, and deletion operations.

**Key Requirements**:
- Store records with phone number, username, and address
- Use username as primary key
- Implement hash function using modulo division
- Handle collisions using linear probing
- Support file persistence
- Advanced: Dual indexing by username and phone number

---

### 3. Design Approach

#### 3.1 Data Structure Design

**Record Structure** (`record.h`):
```cpp
struct Record {
    std::string phoneNumber;
    std::string username;
    std::string address;
    bool isDeleted;  // Lazy deletion flag
};
```

**Hash Table Structure** (`hashtable.h`):
- Uses `std::vector<Record*>` for storage
- Size: 50 (larger than 30 records for load factor < 0.6)
- Tracks count of active records

#### 3.2 Hash Function

**Implementation** (`hashfunction.cpp`):
- Method: Sum of ASCII values modulo table size
- Formula: `hash(key) = (Σ ASCII(char)) % tableSize`
- Simple and effective for string keys

#### 3.3 Collision Resolution

**Linear Probing** (`collision.cpp`):
- Formula: `index = (hashIndex + attempt) % tableSize`
- Sequential search for next available slot
- Handles clustering reasonably well for small datasets

#### 3.4 Operations Complexity

| Operation | Average Case | Worst Case |
|-----------|-------------|------------|
| Insert    | O(1)        | O(n)       |
| Search    | O(1)        | O(n)       |
| Delete    | O(1)        | O(n)       |

---

### 4. Implementation Details

#### 4.1 Core Components

1. **HashTable Class** (`hashtable.cpp`):
   - Constructor/Destructor for memory management
   - `insert()`: Add new records
   - `search()`: Find records by username
   - `remove()`: Lazy deletion
   - `display()`: Show all active records
   - `getSearchLength()`: Calculate probe count
   - `saveToFile()` / `loadFromFile()`: File I/O

2. **Advanced Features**:
   - `insertByPhone()`: Insert using phone as key
   - `searchByPhone()`: Find by phone number
   - `removeByPhone()`: Delete by phone number

3. **User Interface** (`operations.cpp`):
   - Interactive menu system
   - Input validation
   - Error handling

#### 4.2 Key Algorithms

**Insertion Algorithm**:
```
1. Calculate hash index from username
2. Linear probe until empty or deleted slot found
3. Insert record at available position
4. Increment count
```

**Search Algorithm**:
```
1. Calculate hash index
2. Linear probe while slots are occupied
3. Match username at each position
4. Return record if found, null otherwise
```

**Deletion Algorithm**:
```
1. Search for record
2. Mark as deleted (lazy deletion)
3. Decrement count
```

---

### 5. Testing

#### 5.1 Test Data
- 30 records provided in `data/records_username.txt`
- Sample format: `username,phone,address`

#### 5.2 Unit Tests (`test/test_cases.cpp`)
- Insert and search validation
- Delete operation verification
- All tests pass successfully

#### 5.3 Test Results
```
Test insert and search passed.
Test delete passed.
All tests passed.
```

---

### 6. Features Implemented

✅ **Basic Requirements**:
- Hash table with modulo division hash function
- Linear probing collision resolution
- Insert, search, delete, display operations
- Search length calculation
- File save/load functionality
- 30 test records

✅ **Advanced Requirements**:
- Dual indexing (username and phone number)
- Operations by both keys
- Persistent storage on disk

---

### 7. Usage Instructions

#### 7.1 Compilation
```bash
# Using g++
g++ -Iinclude src/*.cpp -o hashtable

# Using Visual Studio
Open AdvancedHashTable.sln and build
```

#### 7.2 Execution
```bash
./hashtable
```

#### 7.3 Menu Options
1. Insert record
2. Search by username
3. Delete by username
4. Display all records
5. Search by phone
6. Delete by phone
7. Save to file
8. Load from file
9. Exit

---

### 8. Performance Analysis

#### 8.1 Load Factor
- Table size: 50
- Max records: 30
- Load factor: 30/50 = 0.6
- Good balance between space and performance

#### 8.2 Search Length Examples
- Best case: 1 (direct hit)
- Average case: 1-3 probes
- Worst case: Up to 30 probes (rare with load factor 0.6)

---

### 9. Conclusions

**Achievements**:
- Successfully implemented a functional hash table
- All basic and advanced requirements met
- Clean, modular code structure
- Comprehensive testing

**Limitations**:
- Fixed table size (no dynamic resizing)
- Simple hash function (could be improved)
- Linear probing can cause clustering

**Future Improvements**:
- Dynamic resizing when load factor exceeds threshold
- Better hash functions (e.g., polynomial rolling hash)
- Alternative collision resolution (quadratic probing, double hashing)
- Separate chaining for better worst-case performance
- GUI interface

---

### 10. References

1. Cormen, T. H., et al. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
2. Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.). Addison-Wesley.
3. C++ Standard Library Documentation

---

**Author**: [Your Name]  
**Date**: December 17, 2025  
**Course**: Data Structures and Algorithms  
**Institution**: [Your Institution]
