#include "file_handler.h"
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Save hash table to file
 */
bool FileHandler::saveHashTable(const HashTable& ht, const std::string& filename) {
    ht.saveToFile(filename);
    return true;
}

/**
 * @brief Load hash table from file
 */
int FileHandler::loadHashTable(HashTable& ht, const std::string& filename) {
    return ht.loadFromFile(filename);
}

/**
 * @brief Check if file exists
 */
bool FileHandler::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

/**
 * @brief Read all records from file
 */
std::vector<Record> FileHandler::readRecordsFromFile(const std::string& filename) {
    std::vector<Record> records;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        return records;
    }

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
            records.push_back(Record(username, phone, address));
        }
    }

    file.close();
    return records;
}

/**
 * @brief Write records to file
 */
bool FileHandler::writeRecordsToFile(const std::vector<Record>& records, const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for writing!" << std::endl;
        return false;
    }

    for (const auto& rec : records) {
        file << rec.username << "," << rec.phoneNumber << "," << rec.address << std::endl;
    }

    file.close();
    return true;
}