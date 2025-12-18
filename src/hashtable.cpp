#include "hashtable.h"
#include "hashfunction.h"
#include "collision.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

/**
 * @brief Constructor - Initialize hash table
 */
HashTable::HashTable(int tableSize, const std::string& type) 
    : size(tableSize), count(0), keyType(type) {
    table.resize(size);
    for (int i = 0; i < size; i++) {
        table[i] = Record();  // Initialize with empty records
    }
}

/**
 * @brief Destructor
 */
HashTable::~HashTable() {
    // Vector handles cleanup automatically
}

/**
 * @brief Find index for a given key
 */
int HashTable::findIndex(const std::string& key, int& searchLength) const {
    searchLength = 0;
    
    if (key.empty()) {
        return -1;
    }

    int hashIndex = HashFunction::hash(key, size);
    int attempt = 0;
    int index = hashIndex;

    // Linear probing
    while (attempt < size) {
        searchLength++;
        
        // Check if slot is empty (never used)
        if (table[index].isEmpty && !table[index].isDeleted) {
            return -1;  // Key not found
        }

        // Check if key matches and not deleted
        std::string currentKey = (keyType == "username") ? 
                                  table[index].username : 
                                  table[index].phoneNumber;

        if (!table[index].isDeleted && currentKey == key) {
            return index;  // Found
        }

        // Continue probing
        attempt++;
        index = CollisionResolution::linearProbe(hashIndex, attempt, size);
    }

    return -1;  // Not found after full cycle
}

/**
 * @brief Insert a record into hash table
 */
bool HashTable::insert(const Record& record) {
    if (count >= size) {
        std::cerr << "Error: Hash table is full!" << std::endl;
        return false;
    }

    // Get the key based on table type
    std::string key = (keyType == "username") ? record.username : record.phoneNumber;

    if (key.empty()) {
        std::cerr << "Error: Key cannot be empty!" << std::endl;
        return false;
    }

    // Check for duplicate
    int dummyLength = 0;
    if (findIndex(key, dummyLength) != -1) {
        std::cerr << "Error: Record with key '" << key << "' already exists!" << std::endl;
        return false;
    }

    int hashIndex = HashFunction::hash(key, size);
    int attempt = 0;
    int index = hashIndex;

    // Find first available slot
    while (attempt < size) {
        if (table[index].isAvailable()) {
            table[index] = record;
            table[index].isEmpty = false;
            table[index].isDeleted = false;
            count++;
            return true;
        }

        attempt++;
        index = CollisionResolution::linearProbe(hashIndex, attempt, size);
    }

    std::cerr << "Error: Could not find available slot!" << std::endl;
    return false;
}

/**
 * @brief Search for a record by key
 */
Record* HashTable::search(const std::string& key) {
    int searchLength = 0;
    int index = findIndex(key, searchLength);

    if (index != -1) {
        return &table[index];
    }

    return nullptr;
}

/**
 * @brief Delete a record by key (lazy deletion)
 */
bool HashTable::remove(const std::string& key) {
    int searchLength = 0;
    int index = findIndex(key, searchLength);

    if (index != -1) {
        table[index].isDeleted = true;
        count--;
        return true;
    }

    return false;
}

/**
 * @brief Display all active records with enhanced UI
 */
void HashTable::display() const {
    if (count == 0) {
        std::cout << "\033[1;33m⚠ Hash table is empty.\033[0m" << std::endl;
        return;
    }

    // Gorgeous gradient header with cyan to magenta
    std::cout << "\n\033[1;96m╔════════════════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║\033[1;95m                         📋 HASH TABLE RECORDS DISPLAY                              \033[1;96m║\n";
    std::cout << "║\033[0m  Key Type: \033[1;93m" << keyType << "\033[0m" << std::string(65 - keyType.length(), ' ') << "\033[1;96m║\n";
    std::cout << "╠══════╦═══════════════════════╦═══════════════════╦═════════════════════════════════════╣\n";
    std::cout << "║\033[1;93m No. \033[1;96m║\033[1;92m Username            \033[1;96m║\033[1;94m Phone Number    \033[1;96m║\033[1;95m Address                           \033[1;96m║\n";
    std::cout << "╠══════╬═══════════════════════╬═══════════════════╬═════════════════════════════════════╣\033[0m\n";

    int displayed = 0;
    for (int i = 0; i < size; i++) {
        if (!table[i].isEmpty && !table[i].isDeleted) {
            displayed++;
            std::string username = table[i].username.length() > 20 ? table[i].username.substr(0, 17) + "..." : table[i].username;
            std::string phone = table[i].phoneNumber.length() > 17 ? table[i].phoneNumber.substr(0, 14) + "..." : table[i].phoneNumber;
            std::string address = table[i].address.length() > 35 ? table[i].address.substr(0, 32) + "..." : table[i].address;
            
            // Alternating row colors for better readability
            std::string rowColor = (displayed % 2 == 0) ? "\033[0;97m" : "\033[0;37m";
            
            std::cout << "\033[1;96m║\033[0m " << rowColor << std::setw(4) << std::right << displayed << " \033[1;96m║\033[0m "
                      << rowColor << std::setw(20) << std::left << username << " \033[1;96m║\033[0m "
                      << rowColor << std::setw(17) << std::left << phone << " \033[1;96m║\033[0m "
                      << rowColor << std::setw(35) << std::left << address << " \033[1;96m║\033[0m" << std::endl;
        }
    }

    std::cout << "\033[1;96m╚══════╩═══════════════════════╩═══════════════════╩═════════════════════════════════════╝\033[0m\n";
    std::cout << "\033[1;92m✨ Total records displayed: \033[1;93m" << displayed << "\033[1;92m / \033[1;94m" << count << "\033[1;92m entries in table\033[0m\n" << std::endl;
}

/**
 * @brief Get search length for a specific key
 */
int HashTable::getSearchLength(const std::string& key) const {
    int searchLength = 0;
    int index = findIndex(key, searchLength);
    
    return (index != -1) ? searchLength : -1;
}

/**
 * @brief Calculate average search length across all records
 */
double HashTable::getAverageSearchLength() const {
    if (count == 0) {
        return 0.0;
    }

    int totalLength = 0;
    int recordCount = 0;

    for (int i = 0; i < size; i++) {
        if (!table[i].isEmpty && !table[i].isDeleted) {
            std::string key = (keyType == "username") ? 
                              table[i].username : 
                              table[i].phoneNumber;
            
            int searchLength = getSearchLength(key);
            if (searchLength > 0) {
                totalLength += searchLength;
                recordCount++;
            }
        }
    }

    return (recordCount > 0) ? (static_cast<double>(totalLength) / recordCount) : 0.0;
}

/**
 * @brief Get current load factor
 */
double HashTable::getLoadFactor() const {
    return (size > 0) ? (static_cast<double>(count) / size) : 0.0;
}

/**
 * @brief Save hash table to file
 * Format: username,phone,address
 */
void HashTable::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for writing!" << std::endl;
        return;
    }

    int saved = 0;
    for (int i = 0; i < size; i++) {
        if (!table[i].isEmpty && !table[i].isDeleted) {
            file << table[i].username << ","
                 << table[i].phoneNumber << ","
                 << table[i].address << std::endl;
            saved++;
        }
    }

    file.close();
    std::cout << "Saved " << saved << " records to '" << filename << "'" << std::endl;
}

/**
 * @brief Load hash table from file
 * Format: username,phone,address
 */
int HashTable::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open file '" << filename << "' for reading!" << std::endl;
        return 0;
    }

    int loaded = 0;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string username, phone, address;

        std::getline(ss, username, ',');
        std::getline(ss, phone, ',');
        std::getline(ss, address);

        // Trim whitespace
        username.erase(0, username.find_first_not_of(" \t\r\n"));
        username.erase(username.find_last_not_of(" \t\r\n") + 1);
        phone.erase(0, phone.find_first_not_of(" \t\r\n"));
        phone.erase(phone.find_last_not_of(" \t\r\n") + 1);
        address.erase(0, address.find_first_not_of(" \t\r\n"));
        address.erase(address.find_last_not_of(" \t\r\n") + 1);

        if (!username.empty() && !phone.empty()) {
            Record rec(username, phone, address);
            if (insert(rec)) {
                loaded++;
            }
        }
    }

    file.close();
    std::cout << "Loaded " << loaded << " records from '" << filename << "'" << std::endl;
    return loaded;
}

/**
 * @brief Clear all records
 */
void HashTable::clear() {
    for (int i = 0; i < size; i++) {
        table[i].clear();
    }
    count = 0;
}
/**
 * @brief Get record at specific index (for GUI display)
 */
const Record* HashTable::getRecordAt(int index) const {
    if (index >= 0 && index < size) {
        return &table[index];
    }
    return nullptr;
}