#include "operations.h"
#include <iostream>

/**
 * @file main.cpp
 * @brief Main entry point for Advanced Hash Table Phone Directory System
 * @author Student ID: F23040111
 * @date December 17, 2025
 * 
 * This program implements a hash table-based phone directory system with:
 * - Modulo division hash function
 * - Linear probing for collision resolution
 * - Dual hash tables (by username and phone number)
 * - File persistence
 * - Complete CRUD operations
 */

int main() {
    try {
        // Table size should be prime for better distribution
        // Using 30 for 30 records gives load factor ~1.0
        const int TABLE_SIZE = 30;
        
        // Create operations manager with dual hash tables
        Operations ops(TABLE_SIZE);
        
        // Run main program loop
        ops.run();
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred!" << std::endl;
        return 1;
    }
}