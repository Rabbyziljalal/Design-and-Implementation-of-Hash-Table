#include "operations.h"
#include "file_handler.h"
#include <iostream>
#include <iomanip>
#include <limits>

/**
 * @brief Constructor - Initialize dual hash tables
 */
Operations::Operations(int tableSize)
    : usernameFile("data/records_username.txt"),
      phoneFile("data/records_phone.txt") {
    
    usernameTable = std::make_unique<HashTable>(tableSize, "username");
    phoneTable = std::make_unique<HashTable>(tableSize, "phone");
}

/**
 * @brief Display main menu with enhanced UI
 */
void Operations::displayMenu() const {
    std::cout << "\n";
    std::cout << "\033[1;36m"; // Cyan color
    std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                                ║\n";
    std::cout << "║        \033[1;33m★ ADVANCED HASH TABLE - PHONE DIRECTORY ★\033[1;36m        ║\n";
    std::cout << "║                                                                ║\n";
    std::cout << "║               \033[1;37mStudent ID: F23040111\033[1;36m                        ║\n";
    std::cout << "║                                                                ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\n";
    std::cout << "║  \033[1;32m[1]\033[1;36m Insert New Record          \033[1;32m[6]\033[1;36m Display All Records  ║\n";
    std::cout << "║  \033[1;32m[2]\033[1;36m Search by Username         \033[1;32m[7]\033[1;36m Display Statistics   ║\n";
    std::cout << "║  \033[1;32m[3]\033[1;36m Search by Phone Number     \033[1;32m[8]\033[1;36m Save to Files        ║\n";
    std::cout << "║  \033[1;32m[4]\033[1;36m Delete by Username         \033[1;32m[9]\033[1;36m Load from Files      ║\n";
    std::cout << "║  \033[1;32m[5]\033[1;36m Delete by Phone Number     \033[1;31m[0]\033[1;36m Exit Program         ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════════╝\033[0m\n";
}

/**
 * @brief Get validated string input
 */
std::string Operations::getStringInput(const std::string& prompt) const {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    
    // Trim whitespace
    input.erase(0, input.find_first_not_of(" \t\r\n"));
    input.erase(input.find_last_not_of(" \t\r\n") + 1);
    
    return input;
}

/**
 * @brief Sync record across both hash tables
 */
bool Operations::syncTables(const Record& record, const std::string& operation) {
    if (operation == "insert") {
        bool usernameOk = usernameTable->insert(record);
        bool phoneOk = phoneTable->insert(record);
        
        if (!usernameOk || !phoneOk) {
            // Rollback if one failed
            if (usernameOk) usernameTable->remove(record.username);
            if (phoneOk) phoneTable->remove(record.phoneNumber);
            return false;
        }
        return true;
    }
    else if (operation == "delete") {
        bool usernameOk = usernameTable->remove(record.username);
        bool phoneOk = phoneTable->remove(record.phoneNumber);
        return usernameOk || phoneOk;
    }
    
    return false;
}

/**
 * @brief Insert new record with enhanced UI
 */
void Operations::insertRecord() {
    std::cout << "\n\033[1;35m╔═══════════════════════════════════════╗\n";
    std::cout << "║     📝 INSERT NEW RECORD              ║\n";
    std::cout << "╚═══════════════════════════════════════╝\033[0m\n";
    
    std::string username = getStringInput("\033[1;37m► Enter username: \033[0m");
    if (username.empty()) {
        std::cout << "\033[1;31m✗ Error: Username cannot be empty!\033[0m" << std::endl;
        return;
    }
    
    std::string phone = getStringInput("\033[1;37m► Enter phone number: \033[0m");
    if (phone.empty()) {
        std::cout << "\033[1;31m✗ Error: Phone number cannot be empty!\033[0m" << std::endl;
        return;
    }
    
    std::string address = getStringInput("\033[1;37m► Enter address: \033[0m");
    
    Record record(username, phone, address);
    
    if (syncTables(record, "insert")) {
        std::cout << "\n\033[1;32m✓ SUCCESS: Record inserted successfully!\033[0m" << std::endl;
        std::cout << "\033[1;36m  → Username: " << username << "\033[0m" << std::endl;
        std::cout << "\033[1;36m  → Phone: " << phone << "\033[0m" << std::endl;
    } else {
        std::cout << "\n\033[1;31m✗ FAILED: Cannot insert record!\033[0m" << std::endl;
        std::cout << "\033[1;33m  (Duplicate key or table full)\033[0m" << std::endl;
    }
}

/**
 * @brief Search by username with enhanced UI
 */
void Operations::searchByUsername() {
    std::cout << "\n\033[1;34m╔═══════════════════════════════════════╗\n";
    std::cout << "║     🔍 SEARCH BY USERNAME             ║\n";
    std::cout << "╚═══════════════════════════════════════╝\033[0m\n";
    
    std::string username = getStringInput("\033[1;37m► Enter username: \033[0m");
    if (username.empty()) {
        std::cout << "\033[1;31m✗ Error: Username cannot be empty!\033[0m" << std::endl;
        return;
    }
    
    Record* found = usernameTable->search(username);
    
    if (found) {
        int searchLength = usernameTable->getSearchLength(username);
        std::cout << "\n\033[1;32m✓ RECORD FOUND!\033[0m" << std::endl;
        std::cout << "\033[1;36m┌─────────────────────────────────────────────┐\033[0m\n";
        std::cout << "\033[1;36m│\033[0m \033[1;37mUsername:\033[0m " << std::left << std::setw(32) << found->username << "\033[1;36m│\033[0m\n";
        std::cout << "\033[1;36m│\033[0m \033[1;37mPhone:   \033[0m " << std::left << std::setw(32) << found->phoneNumber << "\033[1;36m│\033[0m\n";
        std::cout << "\033[1;36m│\033[0m \033[1;37mAddress: \033[0m " << std::left << std::setw(32) << found->address << "\033[1;36m│\033[0m\n";
        std::cout << "\033[1;36m├─────────────────────────────────────────────┤\033[0m\n";
        std::cout << "\033[1;36m│\033[0m \033[1;33m⚡ Search Length: " << searchLength << " probe(s)\033[0m" << std::string(32 - std::to_string(searchLength).length() - 16, ' ') << "\033[1;36m│\033[0m\n";
        std::cout << "\033[1;36m└─────────────────────────────────────────────┘\033[0m\n";
    } else {
        std::cout << "\n\033[1;31m✗ NOT FOUND: No record with username '" << username << "'\033[0m" << std::endl;
    }
}

/**
 * @brief Search by phone number with enhanced UI
 */
void Operations::searchByPhone() {
    std::cout << "\n\033[1;34m╔═══════════════════════════════════════╗\n";
    std::cout << "║     📞 SEARCH BY PHONE NUMBER         ║\n";
    std::cout << "╚═══════════════════════════════════════╝\033[0m\n";
    
    std::string phone = getStringInput("\033[1;37m► Enter phone number: \033[0m");
    if (phone.empty()) {
        std::cout << "\033[1;31m✗ Error: Phone number cannot be empty!\033[0m" << std::endl;
        return;
    }
    
    Record* found = phoneTable->search(phone);
    
    if (found) {
        int searchLength = phoneTable->getSearchLength(phone);
        std::cout << "\n\033[1;32m✓ RECORD FOUND!\033[0m" << std::endl;
        std::cout << "\033[1;36m┌─────────────────────────────────────────────┐\033[0m\n";
        std::cout << "\033[1;36m│\033[0m \033[1;37mUsername:\033[0m " << std::left << std::setw(32) << found->username << "\033[1;36m│\033[0m\n";
        std::cout << "\033[1;36m│\033[0m \033[1;37mPhone:   \033[0m " << std::left << std::setw(32) << found->phoneNumber << "\033[1;36m│\033[0m\n";
        std::cout << "\033[1;36m│\033[0m \033[1;37mAddress: \033[0m " << std::left << std::setw(32) << found->address << "\033[1;36m│\033[0m\n";
        std::cout << "\033[1;36m├─────────────────────────────────────────────┤\033[0m\n";
        std::cout << "\033[1;36m│\033[0m \033[1;33m⚡ Search Length: " << searchLength << " probe(s)\033[0m" << std::string(32 - std::to_string(searchLength).length() - 16, ' ') << "\033[1;36m│\033[0m\n";
        std::cout << "\033[1;36m└─────────────────────────────────────────────┘\033[0m\n";
    } else {
        std::cout << "\n\033[1;31m✗ NOT FOUND: No record with phone '" << phone << "'\033[0m" << std::endl;
    }
}

/**
 * @brief Delete by username with enhanced UI
 */
void Operations::deleteByUsername() {
    std::cout << "\n\033[1;31m╔═══════════════════════════════════════╗\n";
    std::cout << "║     🗑️  DELETE BY USERNAME            ║\n";
    std::cout << "╚═══════════════════════════════════════╝\033[0m\n";
    
    std::string username = getStringInput("\033[1;37m► Enter username: \033[0m");
    if (username.empty()) {
        std::cout << "\033[1;31m✗ Error: Username cannot be empty!\033[0m" << std::endl;
        return;
    }
    
    // Find the record first to get phone number
    Record* found = usernameTable->search(username);
    if (found) {
        Record record = *found;  // Make a copy
        if (syncTables(record, "delete")) {
            std::cout << "\n\033[1;32m✓ SUCCESS: Record deleted!\033[0m" << std::endl;
            std::cout << "\033[1;33m  → Deleted: " << username << "\033[0m" << std::endl;
        } else {
            std::cout << "\n\033[1;31m✗ FAILED: Could not delete record!\033[0m" << std::endl;
        }
    } else {
        std::cout << "\n\033[1;31m✗ NOT FOUND: No record with username '" << username << "'\033[0m" << std::endl;
    }
}

/**
 * @brief Delete by phone number with enhanced UI
 */
void Operations::deleteByPhone() {
    std::cout << "\n\033[1;31m╔═══════════════════════════════════════╗\n";
    std::cout << "║     🗑️  DELETE BY PHONE NUMBER        ║\n";
    std::cout << "╚═══════════════════════════════════════╝\033[0m\n";
    
    std::string phone = getStringInput("\033[1;37m► Enter phone number: \033[0m");
    if (phone.empty()) {
        std::cout << "\033[1;31m✗ Error: Phone number cannot be empty!\033[0m" << std::endl;
        return;
    }
    
    // Find the record first to get username
    Record* found = phoneTable->search(phone);
    if (found) {
        Record record = *found;  // Make a copy
        if (syncTables(record, "delete")) {
            std::cout << "\n\033[1;32m✓ SUCCESS: Record deleted!\033[0m" << std::endl;
            std::cout << "\033[1;33m  → Deleted: " << phone << "\033[0m" << std::endl;
        } else {
            std::cout << "\n\033[1;31m✗ FAILED: Could not delete record!\033[0m" << std::endl;
        }
    } else {
        std::cout << "\n\033[1;31m✗ NOT FOUND: No record with phone '" << phone << "'\033[0m" << std::endl;
    }
}

/**
 * @brief Display all records with enhanced UI
 */
void Operations::displayAllRecords() {
    std::cout << "\n\033[1;35m╔═══════════════════════════════════════╗\n";
    std::cout << "║     📋 DISPLAY ALL RECORDS            ║\n";
    std::cout << "╚═══════════════════════════════════════╝\033[0m\n";
    usernameTable->display();
}

/**
 * @brief Display statistics with enhanced UI
 */
void Operations::displayStatistics() {
    std::cout << "\n\033[1;33m╔════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║              📊 HASH TABLE STATISTICS                         ║\n";
    std::cout << "╠════════════════════════════════════════════════════════════════╣\033[0m\n";
    
    std::cout << "\033[1;36m║  📌 USERNAME HASH TABLE                                        ║\033[0m\n";
    std::cout << "\033[1;37m║    • Table Size:         " << std::setw(35) << std::left << usernameTable->getSize() << "║\033[0m\n";
    std::cout << "\033[1;37m║    • Record Count:       " << std::setw(35) << std::left << usernameTable->getCount() << "║\033[0m\n";
    std::cout << "\033[1;32m║    • Load Factor:        " << std::fixed << std::setprecision(2) 
              << std::setw(34) << std::left << (std::to_string((int)(usernameTable->getLoadFactor() * 100)) + "%") << "║\033[0m\n";
    std::cout << "\033[1;33m║    • Avg Search Length:  " << std::fixed << std::setprecision(2)
              << std::setw(28) << std::left << (std::to_string(usernameTable->getAverageSearchLength()).substr(0,4) + " probes") << "      ║\033[0m\n";
    
    std::cout << "\033[1;36m║                                                                ║\n";
    std::cout << "║  📌 PHONE NUMBER HASH TABLE                                    ║\033[0m\n";
    std::cout << "\033[1;37m║    • Table Size:         " << std::setw(35) << std::left << phoneTable->getSize() << "║\033[0m\n";
    std::cout << "\033[1;37m║    • Record Count:       " << std::setw(35) << std::left << phoneTable->getCount() << "║\033[0m\n";
    std::cout << "\033[1;32m║    • Load Factor:        " << std::fixed << std::setprecision(2)
              << std::setw(34) << std::left << (std::to_string((int)(phoneTable->getLoadFactor() * 100)) + "%") << "║\033[0m\n";
    std::cout << "\033[1;33m║    • Avg Search Length:  " << std::fixed << std::setprecision(2)
              << std::setw(28) << std::left << (std::to_string(phoneTable->getAverageSearchLength()).substr(0,4) + " probes") << "      ║\033[0m\n";
    
    std::cout << "\033[1;33m╚════════════════════════════════════════════════════════════════╝\033[0m\n";
}

/**
 * @brief Save data to files
 */
void Operations::saveToFiles() {
    std::cout << "\n\033[1;36m💾 Saving Data...\033[0m" << std::endl;
    usernameTable->saveToFile(usernameFile);
    phoneTable->saveToFile(phoneFile);
    std::cout << "\033[1;32m✓ Data saved successfully!\033[0m" << std::endl;
}

/**
 * @brief Load data from files
 */
void Operations::loadFromFiles() {
    std::cout << "\n\033[1;36m📂 Loading Data...\033[0m" << std::endl;
    
    // Clear existing data
    usernameTable->clear();
    phoneTable->clear();
    
    int loaded = usernameTable->loadFromFile(usernameFile);
    phoneTable->loadFromFile(phoneFile);
    
    std::cout << "\033[1;32m✓ Data loaded successfully!\033[0m" << std::endl;
}

/**
 * @brief Load data at startup
 */
void Operations::loadData() {
    if (FileHandler::fileExists(usernameFile)) {
        usernameTable->loadFromFile(usernameFile);
    }
    if (FileHandler::fileExists(phoneFile)) {
        phoneTable->loadFromFile(phoneFile);
    }
}

/**
 * @brief Save data before exit
 */
void Operations::saveData() {
    saveToFiles();
}

/**
 * @brief Process user menu choice
 */
void Operations::performOperation(int choice) {
    switch (choice) {
        case 1:
            insertRecord();
            break;
        case 2:
            searchByUsername();
            break;
        case 3:
            searchByPhone();
            break;
        case 4:
            deleteByUsername();
            break;
        case 5:
            deleteByPhone();
            break;
        case 6:
            displayAllRecords();
            break;
        case 7:
            displayStatistics();
            break;
        case 8:
            saveToFiles();
            break;
        case 9:
            loadFromFiles();
            break;
        case 0:
            // Exit handled in run() loop
            break;
        default:
            std::cout << "\n\033[1;31m✗ Invalid choice! Please enter a number between 0-9.\033[0m" << std::endl;
    }
}

/**
 * @brief Run main program loop with enhanced UI
 */
void Operations::run() {
    // Display welcome banner
    std::cout << "\033[2J\033[1;1H"; // Clear screen
    std::cout << "\033[1;33m";
    std::cout << "╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                               ║\n";
    std::cout << "║    ★★★  ADVANCED HASH TABLE PHONE DIRECTORY SYSTEM  ★★★     ║\n";
    std::cout << "║                                                               ║\n";
    std::cout << "║                   Student ID: F23040111                       ║\n";
    std::cout << "║              Course: Data Structures & Algorithms             ║\n";
    std::cout << "║                                                               ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\033[0m\n";
    std::cout << "\n\033[1;36m🔄 Loading existing data...\033[0m\n";
    
    // Load existing data
    loadData();
    
    std::cout << "\n\033[1;32m✓ Ready!\033[0m";
    std::cout << "\n\033[1;37mPress Enter to continue...\033[0m";
    std::cin.get();
    
    int choice;
    do {
        std::cout << "\033[2J\033[1;1H"; // Clear screen
        displayMenu();
        std::cout << "\n\033[1;37m► Enter your choice: \033[0m";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        performOperation(choice);
        
        if (choice != 0) {
            std::cout << "\n\033[1;36m─────────────────────────────────────────────────────────────────\033[0m";
            std::cout << "\n\033[1;37mPress Enter to continue...\033[0m";
            std::cin.get();
        }
        
    } while (choice != 0);
    
    // Save data before exit
    std::cout << "\n\033[1;36m💾 Saving data...\033[0m\n";
    saveData();
    
    std::cout << "\n\033[1;33m╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                                                               ║\n";
    std::cout << "║           Thank you for using Phone Directory System!        ║\n";
    std::cout << "║                       Goodbye! 👋                             ║\n";
    std::cout << "║                                                               ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\033[0m\n\n";
}
