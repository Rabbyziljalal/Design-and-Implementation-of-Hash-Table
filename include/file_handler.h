#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "hashtable.h"
#include <string>
#include <vector>

/**
 * @brief File I/O operations for hash table persistence
 * Handles saving and loading hash tables to/from disk
 */
class FileHandler {
public:
    /**
     * @brief Save hash table to file
     * @param ht Hash table to save
     * @param filename Output file path
     * @return true if successful
     */
    static bool saveHashTable(const HashTable& ht, const std::string& filename);

    /**
     * @brief Load hash table from file
     * @param ht Hash table to load into
     * @param filename Input file path
     * @return Number of records loaded
     */
    static int loadHashTable(HashTable& ht, const std::string& filename);

    /**
     * @brief Check if file exists
     * @param filename File path
     * @return true if file exists
     */
    static bool fileExists(const std::string& filename);

    /**
     * @brief Read all records from file
     * @param filename File path
     * @return Vector of records
     */
    static std::vector<Record> readRecordsFromFile(const std::string& filename);

    /**
     * @brief Write records to file
     * @param records Vector of records
     * @param filename Output file path
     * @return true if successful
     */
    static bool writeRecordsToFile(const std::vector<Record>& records, const std::string& filename);
};

#endif // FILE_HANDLER_H