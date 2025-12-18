// Hash Table Implementation with Modulo-Division Hash and Linear Probing
// Student ID: F23040111

class Record {
    constructor(username, phoneNumber, address, isEmpty = false, isDeleted = false) {
        this.username = username;
        this.phoneNumber = phoneNumber;
        this.address = address;
        this.isEmpty = isEmpty;
        this.isDeleted = isDeleted;
    }
}

class HashTable {
    constructor(size = 45) {
        this.size = size; // Table size for hash distribution
        this.table = Array(size).fill(null).map(() => new Record('', '', '', true, false));
        this.count = 0;
    }

    // Modulo-Division Hash Function
    // hash(key) = (sum of ASCII values) % table_size
    hashFunction(key) {
        let sum = 0;
        for (let i = 0; i < key.length; i++) {
            sum += key.charCodeAt(i);
        }
        return sum % this.size;
    }

    // Linear Probing for collision resolution
    // h(k, i) = (h(k) + i) mod m
    probe(baseHash, attempt) {
        return (baseHash + attempt) % this.size;
    }

    // Insert a record into the hash table
    insert(username, phoneNumber, address) {
        if (this.count >= this.size) {
            return { success: false, message: 'Hash table is full!' };
        }

        // Check if username or phone already exists
        const usernameExists = this.search(username);
        const phoneExists = this.search(phoneNumber);
        
        if (usernameExists.success || phoneExists.success) {
            return { success: false, message: 'Record with this username or phone number already exists!' };
        }

        // Insert by username
        const usernameHash = this.hashFunction(username);
        let usernameIndex = -1;
        for (let i = 0; i < this.size; i++) {
            const index = this.probe(usernameHash, i);
            if (this.table[index].isEmpty || this.table[index].isDeleted) {
                usernameIndex = index;
                break;
            }
        }

        // Insert by phone number
        const phoneHash = this.hashFunction(phoneNumber);
        let phoneIndex = -1;
        for (let i = 0; i < this.size; i++) {
            const index = this.probe(phoneHash, i);
            if (this.table[index].isEmpty || this.table[index].isDeleted) {
                phoneIndex = index;
                break;
            }
        }

        if (usernameIndex === -1 || phoneIndex === -1) {
            return { success: false, message: 'Could not find slot for insertion!' };
        }

        // Store records at both indices
        this.table[usernameIndex] = new Record(username, phoneNumber, address, false, false);
        this.table[phoneIndex] = new Record(username, phoneNumber, address, false, false);
        this.count++;

        return { 
            success: true, 
            message: `Record inserted successfully!\nUsername hash: ${usernameHash} → Index: ${usernameIndex}\nPhone hash: ${phoneHash} → Index: ${phoneIndex}` 
        };
    }

    // Search for a record by key (username or phone)
    search(key) {
        const hash = this.hashFunction(key);
        
        for (let i = 0; i < this.size; i++) {
            const index = this.probe(hash, i);
            const record = this.table[index];
            
            if (record.isEmpty && !record.isDeleted) {
                break; // Key not found
            }
            
            if (!record.isDeleted && (record.username === key || record.phoneNumber === key)) {
                return { 
                    success: true, 
                    record: record,
                    index: index,
                    hash: hash,
                    probes: i + 1
                };
            }
        }
        
        return { success: false, message: 'Record not found!' };
    }

    // Delete a record by key (username or phone)
    delete(key) {
        const searchResult = this.search(key);
        
        if (!searchResult.success) {
            return { success: false, message: 'Record not found!' };
        }

        const record = searchResult.record;
        const username = record.username;
        const phoneNumber = record.phoneNumber;

        // Delete both entries (username and phone)
        let deletedCount = 0;
        
        for (let i = 0; i < this.size; i++) {
            const r = this.table[i];
            if (!r.isEmpty && !r.isDeleted && 
                (r.username === username || r.phoneNumber === phoneNumber)) {
                r.isDeleted = true;
                deletedCount++;
            }
        }

        if (deletedCount > 0) {
            this.count--;
            return { success: true, message: `Record deleted successfully! (${deletedCount} entries removed)` };
        }

        return { success: false, message: 'Error deleting record!' };
    }

    // Display all records
    getAllRecords() {
        const records = [];
        const seen = new Set();

        for (let i = 0; i < this.size; i++) {
            const record = this.table[i];
            if (!record.isEmpty && !record.isDeleted) {
                const key = `${record.username}|${record.phoneNumber}`;
                if (!seen.has(key)) {
                    records.push({
                        index: i,
                        username: record.username,
                        phoneNumber: record.phoneNumber,
                        address: record.address,
                        hash: this.hashFunction(record.username)
                    });
                    seen.add(key);
                }
            }
        }

        return records;
    }

    // Get table for display (username or phone based)
    getTableView(type = 'username') {
        const tableView = [];
        
        for (let i = 0; i < this.size; i++) {
            const record = this.table[i];
            if (record.isEmpty) {
                tableView.push({ index: i, status: 'empty', hash: '-' });
            } else if (record.isDeleted) {
                tableView.push({ index: i, status: 'deleted', hash: '-' });
            } else {
                const key = type === 'username' ? record.username : record.phoneNumber;
                const hash = this.hashFunction(key);
                tableView.push({
                    index: i,
                    status: 'occupied',
                    key: key,
                    username: record.username,
                    phoneNumber: record.phoneNumber,
                    address: record.address,
                    hash: hash
                });
            }
        }
        
        return tableView;
    }

    // Get load factor
    getLoadFactor() {
        return ((this.count / this.size) * 100).toFixed(1);
    }

    // Export data to JSON
    exportToJSON() {
        const records = this.getAllRecords();
        return JSON.stringify(records, null, 2);
    }

    // Import data from JSON
    importFromJSON(jsonString) {
        try {
            const records = JSON.parse(jsonString);
            this.clear();
            
            let successCount = 0;
            let failCount = 0;

            records.forEach(record => {
                const result = this.insert(record.username, record.phoneNumber, record.address);
                if (result.success) successCount++;
                else failCount++;
            });

            return { 
                success: true, 
                message: `Imported ${successCount} records successfully. ${failCount} failed.` 
            };
        } catch (error) {
            return { success: false, message: 'Invalid JSON format!' };
        }
    }

    // Clear all data
    clear() {
        this.table = Array(this.size).fill(null).map(() => new Record('', '', '', true, false));
        this.count = 0;
    }

    // Save to localStorage
    saveToStorage() {
        const data = {
            size: this.size,
            count: this.count,
            table: this.table
        };
        localStorage.setItem('phoneDirectory', JSON.stringify(data));
    }

    // Load from localStorage
    loadFromStorage() {
        const data = localStorage.getItem('phoneDirectory');
        if (data) {
            const parsed = JSON.parse(data);
            this.size = parsed.size;
            this.count = parsed.count;
            this.table = parsed.table.map(r => new Record(r.username, r.phoneNumber, r.address, r.isEmpty, r.isDeleted));
            return true;
        }
        return false;
    }
}

// Sample data for testing
const SAMPLE_DATA = [
    { username: 'alice_smith', phoneNumber: '555-0101', address: '123 Maple Street, Springfield' },
    { username: 'bob_jones', phoneNumber: '555-0102', address: '456 Oak Avenue, Riverside' },
    { username: 'charlie_brown', phoneNumber: '555-0103', address: '789 Pine Road, Lakewood' },
    { username: 'diana_prince', phoneNumber: '555-0104', address: '321 Elm Drive, Hilltown' },
    { username: 'edward_norton', phoneNumber: '555-0105', address: '654 Cedar Lane, Brookfield' },
    { username: 'fiona_apple', phoneNumber: '555-0106', address: '987 Birch Court, Meadowview' },
    { username: 'george_martin', phoneNumber: '555-0107', address: '147 Willow Way, Parkside' },
    { username: 'hannah_montana', phoneNumber: '555-0108', address: '258 Aspen Place, Riverside' },
    { username: 'ian_malcolm', phoneNumber: '555-0109', address: '369 Cherry Street, Hillcrest' },
    { username: 'julia_roberts', phoneNumber: '555-0110', address: '741 Walnut Avenue, Lakeside' },
    { username: 'kevin_hart', phoneNumber: '555-0111', address: '852 Spruce Road, Greenfield' },
    { username: 'laura_palmer', phoneNumber: '555-0112', address: '963 Poplar Drive, Fairview' },
    { username: 'michael_scott', phoneNumber: '555-0113', address: '159 Sycamore Lane, Oakdale' },
    { username: 'nancy_drew', phoneNumber: '555-0114', address: '357 Hickory Court, Maplewood' },
    { username: 'oliver_twist', phoneNumber: '555-0115', address: '468 Magnolia Way, Pineville' },
    { username: 'peter_parker', phoneNumber: '555-0116', address: '579 Chestnut Place, Cedartown' },
    { username: 'quinn_fabray', phoneNumber: '555-0117', address: '681 Beech Street, Elmwood' },
    { username: 'rachel_green', phoneNumber: '555-0118', address: '792 Ash Avenue, Birchfield' },
    { username: 'sam_wilson', phoneNumber: '555-0119', address: '813 Fir Road, Willowbrook' },
    { username: 'tina_fey', phoneNumber: '555-0120', address: '924 Redwood Drive, Aspendale' },
    { username: 'ursula_burns', phoneNumber: '555-0121', address: '135 Cypress Lane, Cherryvale' },
    { username: 'victor_hugo', phoneNumber: '555-0122', address: '246 Sequoia Court, Walnutville' },
    { username: 'wendy_williams', phoneNumber: '555-0123', address: '357 Dogwood Way, Sprucetown' },
    { username: 'xavier_woods', phoneNumber: '555-0124', address: '468 Juniper Place, Poplarcity' },
    { username: 'yvonne_strahovski', phoneNumber: '555-0125', address: '579 Hemlock Street, Sycamore' },
    { username: 'zachary_levi', phoneNumber: '555-0126', address: '681 Laurel Avenue, Hickoryhill' },
    { username: 'amy_adams', phoneNumber: '555-0127', address: '792 Acacia Road, Magnoliapark' },
    { username: 'bruce_wayne', phoneNumber: '555-0128', address: '813 Cottonwood Drive, Chestnutgrove' },
    { username: 'clark_kent', phoneNumber: '555-0129', address: '924 Alder Lane, Beechwood' },
    { username: 'david_lynch', phoneNumber: '555-0130', address: '135 Boxwood Court, Ashville' }
];
