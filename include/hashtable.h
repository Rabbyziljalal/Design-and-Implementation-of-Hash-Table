#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "record.h"
#include <vector>
#include <string>

/**
 * @brief Hash table implementation with linear probing
 * Supports operations by both username and phone number keys
 */
class HashTable {
private:
    std::vector<Record> table;  // Hash table storage
    int size;                   // Table size
    int count;                  // Number of active records
    std::string keyType;        // "username" or "phone"

    /**
     * @brief Find index for a given key
     * @param key Search key
     * @param searchLength Output parameter for probe count
     * @return Index if found, -1 otherwise
     */
    int findIndex(const std::string& key, int& searchLength) const;

public:
    /**
     * @brief Constructor
     * @param tableSize Size of hash table (should be prime for better distribution)
     * @param type Key type: "username" or "phone"
     */
    HashTable(int tableSize, const std::string& type = "username");

    /**
     * @brief Destructor
     */
    ~HashTable();

    /**
     * @brief Insert a record into hash table
     * @param record Record to insert
     * @return true if successful, false if duplicate or table full
     */
    bool insert(const Record& record);

    /**
     * @brief Search for a record by key
     * @param key Search key (username or phone)
     * @return Pointer to record if found, nullptr otherwise
     */
    Record* search(const std::string& key);

    /**
     * @brief Delete a record by key
     * @param key Key to delete
     * @return true if deleted, false if not found
     */
    bool remove(const std::string& key);

    /**
     * @brief Display all active records
     */
    void display() const;

    /**
     * @brief Get search length (number of probes) for a key
     * @param key Search key
     * @return Number of probes, -1 if not found
     */
    int getSearchLength(const std::string& key) const;

    /**
     * @brief Calculate average search length
     * @return Average number of probes across all records
     */
    double getAverageSearchLength() const;

    /**
     * @brief Get current load factor
     * @return Load factor (count / size)
     */
    double getLoadFactor() const;

    /**
     * @brief Save hash table to file
     * @param filename File path
     */
    void saveToFile(const std::string& filename) const;

    /**
     * @brief Load hash table from file
     * @param filename File path
     * @return Number of records loaded
     */
    int loadFromFile(const std::string& filename);

    /**
     * @brief Clear all records
     */
    void clear();

    /**
     * @brief Get number of active records
     * @return Record count
     */
    int getCount() const { return count; }

    /**
     * @brief Get table size
     * @return Table size
     */
    int getSize() const { return size; }

    /**
     * @brief Get key type
     * @return "username" or "phone"
     */
    std::string getKeyType() const { return keyType; }

    /**
     * @brief Get record at specific index (for GUI display)
     * @param index Table index
     * @return Const pointer to record, nullptr if invalid index
     */
    const Record* getRecordAt(int index) const;
};

#endif // HASHTABLE_H