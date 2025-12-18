#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "hashtable.h"
#include <memory>

/**
 * @brief User interface operations and menu management
 * Manages dual hash tables (by username and phone)
 */
class Operations {
private:
    std::unique_ptr<HashTable> usernameTable;
    std::unique_ptr<HashTable> phoneTable;
    const std::string usernameFile;
    const std::string phoneFile;

public:
    /**
     * @brief Constructor
     * @param tableSize Size of hash tables
     */
    Operations(int tableSize);

    /**
     * @brief Display main menu
     */
    void displayMenu() const;

    /**
     * @brief Process user menu choice
     * @param choice Menu option number
     */
    void performOperation(int choice);

    /**
     * @brief Run main program loop
     */
    void run();

    /**
     * @brief Load data from files at startup
     */
    void loadData();

    /**
     * @brief Save data to files before exit
     */
    void saveData();

private:
    // Menu operation handlers
    void insertRecord();
    void searchByUsername();
    void searchByPhone();
    void deleteByUsername();
    void deleteByPhone();
    void displayAllRecords();
    void displayStatistics();
    void saveToFiles();
    void loadFromFiles();

    /**
     * @brief Get validated string input
     * @param prompt Prompt message
     * @return User input string
     */
    std::string getStringInput(const std::string& prompt) const;

    /**
     * @brief Sync record across both hash tables
     * @param record Record to sync
     * @param operation "insert" or "delete"
     */
    bool syncTables(const Record& record, const std::string& operation);
};

#endif // OPERATIONS_H