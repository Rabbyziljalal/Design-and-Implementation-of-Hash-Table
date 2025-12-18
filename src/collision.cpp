#include "collision.h"

/**
 * @brief Linear probing: h(k, i) = (h(k) + i) mod m
 * @param hashIndex Original hash value
 * @param attempt Probe attempt number (0, 1, 2, ...)
 * @param tableSize Size of hash table
 * @return New index after probing
 */
int CollisionResolution::linearProbe(int hashIndex, int attempt, int tableSize) {
    if (tableSize <= 0) {
        return 0;
    }
    return (hashIndex + attempt) % tableSize;
}

/**
 * @brief Get next position in linear probing sequence
 * @param currentIndex Current position
 * @param tableSize Size of hash table
 * @return Next probe position
 */
int CollisionResolution::nextProbe(int currentIndex, int tableSize) {
    if (tableSize <= 0) {
        return 0;
    }
    return (currentIndex + 1) % tableSize;
}