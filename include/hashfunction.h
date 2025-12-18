#ifndef HASHFUNCTION_H
#define HASHFUNCTION_H

#include <string>

/**
 * @brief Hash function implementation using modulo division method
 * Implements hash(key) = sum(ASCII values) % tableSize
 */
class HashFunction {
public:
    /**
     * @brief Calculate hash value using modulo division method
     * @param key The string key to hash
     * @param tableSize Size of the hash table
     * @return Hash index in range [0, tableSize-1]
     */
    static int hash(const std::string& key, int tableSize);

    /**
     * @brief Calculate hash with custom seed for better distribution
     * @param key The string key to hash
     * @param tableSize Size of the hash table
     * @return Hash index in range [0, tableSize-1]
     */
    static int hashWithSeed(const std::string& key, int tableSize);
};

#endif // HASHFUNCTION_H