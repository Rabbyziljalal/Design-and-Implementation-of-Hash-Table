#ifndef RECORD_H
#define RECORD_H

#include <string>

/**
 * @brief Record structure for phone directory entry
 * Contains username, phone number, and address information
 */
struct Record {
    std::string username;
    std::string phoneNumber;
    std::string address;
    bool isDeleted;  // Flag for lazy deletion
    bool isEmpty;    // Flag for empty slot

    // Default constructor
    Record() : username(""), phoneNumber(""), address(""), isDeleted(false), isEmpty(true) {}

    // Parameterized constructor
    Record(const std::string& user, const std::string& phone, const std::string& addr)
        : username(user), phoneNumber(phone), address(addr), isDeleted(false), isEmpty(false) {}

    // Check if slot is available for insertion
    bool isAvailable() const {
        return isEmpty || isDeleted;
    }

    // Clear the record
    void clear() {
        username = "";
        phoneNumber = "";
        address = "";
        isDeleted = false;
        isEmpty = true;
    }
};

#endif // RECORD_H