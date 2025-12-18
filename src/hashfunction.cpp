#include "hashfunction.h"

/**
 * @brief Hash function using modulo division method
 * Implements: hash(key) = (sum of ASCII values) % tableSize
 */
int HashFunction::hash(const std::string& key, int tableSize) {
    if (key.empty() || tableSize <= 0) {
        return 0;
    }

    unsigned long long sum = 0;
    for (size_t i = 0; i < key.length(); i++) {
        sum += static_cast<unsigned char>(key[i]);
    }
    
    return static_cast<int>(sum % tableSize);
}

/**
 * @brief Enhanced hash function with polynomial rolling hash
 * Better distribution for similar keys
 */
int HashFunction::hashWithSeed(const std::string& key, int tableSize) {
    if (key.empty() || tableSize <= 0) {
        return 0;
    }

    const unsigned long long PRIME = 31;
    unsigned long long hashValue = 0;
    unsigned long long power = 1;

    for (size_t i = 0; i < key.length(); i++) {
        hashValue = (hashValue + (static_cast<unsigned char>(key[i]) * power)) % tableSize;
        power = (power * PRIME) % tableSize;
    }

    return static_cast<int>(hashValue);
}