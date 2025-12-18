#include "../include/hashtable.h"
#include "../include/record.h"
#include <iostream>
#include <cassert>
#include <vector>

/**
 * @file test_cases.cpp
 * @brief Unit tests for hash table implementation
 */

void testRecordCreation() {
    std::cout << "Test 1: Record Creation... ";
    
    Record rec1("Alice", "1234567890", "123 Main St");
    assert(rec1.username == "Alice");
    assert(rec1.phoneNumber == "1234567890");
    assert(rec1.address == "123 Main St");
    assert(!rec1.isEmpty);
    assert(!rec1.isDeleted);
    
    Record rec2;
    assert(rec2.isEmpty);
    assert(!rec2.isDeleted);
    
    std::cout << "PASSED" << std::endl;
}

void testHashTableInsert() {
    std::cout << "Test 2: Hash Table Insert... ";
    
    HashTable ht(10, "username");
    
    Record rec1("Alice", "1234567890", "123 Main St");
    assert(ht.insert(rec1) == true);
    assert(ht.getCount() == 1);
    
    // Test duplicate insertion
    assert(ht.insert(rec1) == false);
    assert(ht.getCount() == 1);
    
    // Test different record
    Record rec2("Bob", "0987654321", "456 Elm St");
    assert(ht.insert(rec2) == true);
    assert(ht.getCount() == 2);
    
    std::cout << "PASSED" << std::endl;
}

void testHashTableSearch() {
    std::cout << "Test 3: Hash Table Search... ";
    
    HashTable ht(10, "username");
    
    Record rec1("Alice", "1234567890", "123 Main St");
    Record rec2("Bob", "0987654321", "456 Elm St");
    
    ht.insert(rec1);
    ht.insert(rec2);
    
    Record* found1 = ht.search("Alice");
    assert(found1 != nullptr);
    assert(found1->username == "Alice");
    assert(found1->phoneNumber == "1234567890");
    
    Record* found2 = ht.search("Bob");
    assert(found2 != nullptr);
    assert(found2->username == "Bob");
    
    Record* found3 = ht.search("Charlie");
    assert(found3 == nullptr);
    
    std::cout << "PASSED" << std::endl;
}

void testHashTableDelete() {
    std::cout << "Test 4: Hash Table Delete... ";
    
    HashTable ht(10, "username");
    
    Record rec("Alice", "1234567890", "123 Main St");
    ht.insert(rec);
    
    assert(ht.getCount() == 1);
    assert(ht.remove("Alice") == true);
    assert(ht.getCount() == 0);
    
    Record* found = ht.search("Alice");
    assert(found == nullptr);
    
    // Test delete non-existent
    assert(ht.remove("Bob") == false);
    
    std::cout << "PASSED" << std::endl;
}

void testSearchLength() {
    std::cout << "Test 5: Search Length Calculation... ";
    
    HashTable ht(10, "username");
    
    Record rec1("Alice", "1234567890", "123 Main St");
    Record rec2("Bob", "0987654321", "456 Elm St");
    
    ht.insert(rec1);
    ht.insert(rec2);
    
    int length1 = ht.getSearchLength("Alice");
    assert(length1 > 0);
    
    int length2 = ht.getSearchLength("Bob");
    assert(length2 > 0);
    
    int length3 = ht.getSearchLength("Charlie");
    assert(length3 == -1);
    
    std::cout << "PASSED" << std::endl;
}

void testLoadFactor() {
    std::cout << "Test 6: Load Factor... ";
    
    HashTable ht(10, "username");
    
    assert(ht.getLoadFactor() == 0.0);
    
    for (int i = 0; i < 5; i++) {
        std::string name = "User" + std::to_string(i);
        std::string phone = "123456789" + std::to_string(i);
        Record rec(name, phone, "Address");
        ht.insert(rec);
    }
    
    assert(ht.getLoadFactor() == 0.5);
    
    std::cout << "PASSED" << std::endl;
}

void testCollisionHandling() {
    std::cout << "Test 7: Collision Handling... ";
    
    // Small table to force collisions
    HashTable ht(5, "username");
    
    std::vector<Record> records = {
        Record("Alice", "1111111111", "Address 1"),
        Record("Bob", "2222222222", "Address 2"),
        Record("Charlie", "3333333333", "Address 3"),
        Record("David", "4444444444", "Address 4")
    };
    
    // Insert all records
    for (const auto& rec : records) {
        assert(ht.insert(rec) == true);
    }
    
    // Verify all can be found
    for (const auto& rec : records) {
        Record* found = ht.search(rec.username);
        assert(found != nullptr);
        assert(found->username == rec.username);
    }
    
    std::cout << "PASSED" << std::endl;
}

void testFileOperations() {
    std::cout << "Test 8: File Save/Load... ";
    
    HashTable ht1(10, "username");
    
    // Insert records
    ht1.insert(Record("Alice", "1234567890", "123 Main St"));
    ht1.insert(Record("Bob", "0987654321", "456 Elm St"));
    
    // Save to file
    ht1.saveToFile("test_output.txt");
    
    // Load into new table
    HashTable ht2(10, "username");
    int loaded = ht2.loadFromFile("test_output.txt");
    
    assert(loaded == 2);
    assert(ht2.getCount() == 2);
    
    Record* found = ht2.search("Alice");
    assert(found != nullptr);
    assert(found->phoneNumber == "1234567890");
    
    std::cout << "PASSED" << std::endl;
}

void testPhoneNumberKey() {
    std::cout << "Test 9: Phone Number as Key... ";
    
    HashTable ht(10, "phone");
    
    Record rec1("Alice", "1234567890", "123 Main St");
    Record rec2("Bob", "0987654321", "456 Elm St");
    
    assert(ht.insert(rec1) == true);
    assert(ht.insert(rec2) == true);
    
    Record* found1 = ht.search("1234567890");
    assert(found1 != nullptr);
    assert(found1->username == "Alice");
    
    Record* found2 = ht.search("0987654321");
    assert(found2 != nullptr);
    assert(found2->username == "Bob");
    
    std::cout << "PASSED" << std::endl;
}

void testLargeDataset() {
    std::cout << "Test 10: Large Dataset (30+ records)... ";
    
    HashTable ht(30, "username");  // Size matches number of records
    
    // Insert 30 records
    for (int i = 0; i < 30; i++) {
        std::string name = "User" + std::to_string(i);
        std::string phone = "555000" + std::to_string(1000 + i);
        std::string address = "Address " + std::to_string(i);
        
        Record rec(name, phone, address);
        assert(ht.insert(rec) == true);
    }
    
    assert(ht.getCount() == 30);
    
    // Verify all can be found
    for (int i = 0; i < 30; i++) {
        std::string name = "User" + std::to_string(i);
        Record* found = ht.search(name);
        assert(found != nullptr);
        assert(found->username == name);
    }
    
    double loadFactor = ht.getLoadFactor();
    assert(loadFactor > 0.5 && loadFactor < 0.6);
    
    std::cout << "PASSED" << std::endl;
}

int main() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  ADVANCED HASH TABLE - UNIT TESTS" << std::endl;
    std::cout << "  Student ID: F23040111" << std::endl;
    std::cout << std::string(60, '=') << std::endl << std::endl;
    
    try {
        testRecordCreation();
        testHashTableInsert();
        testHashTableSearch();
        testHashTableDelete();
        testSearchLength();
        testLoadFactor();
        testCollisionHandling();
        testFileOperations();
        testPhoneNumberKey();
        testLargeDataset();
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "  ALL TESTS PASSED! ✓" << std::endl;
        std::cout << std::string(60, '=') << std::endl << std::endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "\n✗ TEST FAILED: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\n✗ TEST FAILED: Unknown error" << std::endl;
        return 1;
    }
}