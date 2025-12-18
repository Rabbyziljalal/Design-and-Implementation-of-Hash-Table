#ifndef COLLISION_H
#define COLLISION_H

/**
 * @brief Collision resolution strategies for hash table
 * Implements linear probing: h(k, i) = (h(k) + i) mod m
 */
class CollisionResolution {
public:
    /**
     * @brief Linear probing collision resolution
     * @param hashIndex Original hash index
     * @param attempt Number of probing attempts (0-based)
     * @param tableSize Size of the hash table
     * @return New index after probing
     */
    static int linearProbe(int hashIndex, int attempt, int tableSize);

    /**
     * @brief Calculate next probe position
     * @param currentIndex Current position in table
     * @param tableSize Size of the hash table
     * @return Next probe position
     */
    static int nextProbe(int currentIndex, int tableSize);
};

#endif // COLLISION_H