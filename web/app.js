// Application Logic
const MAX_RECORDS = 30; // Enforce 30 visible/sample records
const TABLE_SIZE = 45; // Hash table size
// Initialize hash tables (one for username, one for phone)
let usernameTable = new HashTable(TABLE_SIZE);
let phoneTable = new HashTable(TABLE_SIZE);

// Track unique records
let recordsMap = new Map(); // key: username, value: {username, phoneNumber, address}

// Load data on startup
window.addEventListener('DOMContentLoaded', () => {
    console.log(`SAMPLE_DATA contains ${SAMPLE_DATA.length} records`);
    loadFromLocalStorage();
    console.log(`After loadFromLocalStorage, recordsMap has ${recordsMap.size} records`);
    updateStats();
    // Data will be displayed only when Hash Table Contents button is clicked
});

// Modal Functions
function openInsertModal() {
    const modal = document.getElementById('insertModal');
    modal.classList.add('show');
    modal.style.display = 'flex';
}

function closeInsertModal() {
    const modal = document.getElementById('insertModal');
    modal.classList.remove('show');
    modal.style.display = 'none';
    document.getElementById('insertFormContainer').style.display = 'none';
}

function showInsertForm() {
    document.getElementById('insertFormContainer').style.display = 'block';
}

function openSearchModal() {
    const modal = document.getElementById('searchModal');
    modal.classList.add('show');
    modal.style.display = 'flex';
}

function closeSearchModal() {
    const modal = document.getElementById('searchModal');
    modal.classList.remove('show');
    modal.style.display = 'none';
    document.getElementById('searchFormContainer').style.display = 'none';
    document.getElementById('modalSearchResult').innerHTML = '';
}

function showSearchForm() {
    document.getElementById('searchFormContainer').style.display = 'block';
}

function openDeleteModal() {
    const modal = document.getElementById('deleteModal');
    modal.classList.add('show');
    modal.style.display = 'flex';
}

function closeDeleteModal() {
    const modal = document.getElementById('deleteModal');
    modal.classList.remove('show');
    modal.style.display = 'none';
    document.getElementById('deleteFormContainer').style.display = 'none';
}

function showDeleteForm() {
    document.getElementById('deleteFormContainer').style.display = 'block';
}

function openDataModal() {
    const modal = document.getElementById('dataModal');
    modal.classList.add('show');
    modal.style.display = 'flex';
}

function closeDataModal() {
    const modal = document.getElementById('dataModal');
    modal.classList.remove('show');
    modal.style.display = 'none';
}

function openTableModal() {
    const modal = document.getElementById('tableModal');
    modal.classList.add('show');
    modal.style.display = 'flex';
    displayTable('all'); // Display data when modal opens
}

function closeTableModal() {
    const modal = document.getElementById('tableModal');
    modal.classList.remove('show');
    modal.style.display = 'none';
}

function openStatsModal() {
    const modal = document.getElementById('statsModal');
    modal.classList.add('show');
    modal.style.display = 'flex';
    displayStatistics();
}

function closeStatsModal() {
    const modal = document.getElementById('statsModal');
    modal.classList.remove('show');
    modal.style.display = 'none';
}

function displayStatistics() {
    const statsDiv = document.getElementById('statsDisplay');
    
    // Calculate statistics
    const totalRecords = recordsMap.size;
    const tableSize = TABLE_SIZE;
    const loadFactor = ((totalRecords / tableSize) * 100).toFixed(2);
    
    // Count occupied, empty, and deleted slots in username table
    let occupiedSlots = 0;
    let emptySlots = 0;
    let deletedSlots = 0;
    
    for (let i = 0; i < usernameTable.table.length; i++) {
        const slot = usernameTable.table[i];
        if (slot.isDeleted) {
            deletedSlots++;
        } else if (slot.isEmpty) {
            emptySlots++;
        } else {
            occupiedSlots++;
        }
    }
    
    // Calculate collision statistics
    let totalProbes = 0;
    let recordsWithCollisions = 0;
    
    recordsMap.forEach((record) => {
        const searchResult = usernameTable.search(record.username);
        if (searchResult.success) {
            totalProbes += searchResult.probes;
            if (searchResult.probes > 1) {
                recordsWithCollisions++;
            }
        }
    });
    
    const avgProbes = totalRecords > 0 ? (totalProbes / totalRecords).toFixed(2) : 0;
    const collisionRate = totalRecords > 0 ? ((recordsWithCollisions / totalRecords) * 100).toFixed(2) : 0;
    const occupiedPercentage = ((occupiedSlots / tableSize) * 100).toFixed(2);
    
    statsDiv.innerHTML = `
        <div class="stat-card-large">
            <div class="stat-icon">📋</div>
            <div class="stat-info">
                <div class="stat-label">Total Records</div>
                <div class="stat-value">${totalRecords}</div>
            </div>
        </div>
        
        <div class="stat-card-large">
            <div class="stat-icon">📦</div>
            <div class="stat-info">
                <div class="stat-label">Table Size</div>
                <div class="stat-value">${tableSize}</div>
            </div>
        </div>
        
        <div class="stat-card-large">
            <div class="stat-icon">📈</div>
            <div class="stat-info">
                <div class="stat-label">Load Factor</div>
                <div class="stat-value">${loadFactor}%</div>
            </div>
        </div>
        
        <div class="stat-card-large">
            <div class="stat-icon">✅</div>
            <div class="stat-info">
                <div class="stat-label">Occupied Slots</div>
                <div class="stat-value">${occupiedPercentage}%</div>
            </div>
        </div>
        
        <div class="stat-card-large">
            <div class="stat-icon">⚪</div>
            <div class="stat-info">
                <div class="stat-label">Empty Slots</div>
                <div class="stat-value">${emptySlots}</div>
            </div>
        </div>
        
        <div class="stat-card-large">
            <div class="stat-icon">❌</div>
            <div class="stat-info">
                <div class="stat-label">Deleted Slots</div>
                <div class="stat-value">${deletedSlots}</div>
            </div>
        </div>
        
        <div class="stat-card-large">
            <div class="stat-icon">🔍</div>
            <div class="stat-info">
                <div class="stat-label">Avg Probes per Search</div>
                <div class="stat-value">${avgProbes}</div>
            </div>
        </div>
        
        <div class="stat-card-large">
            <div class="stat-icon">⚠️</div>
            <div class="stat-info">
                <div class="stat-label">Collision Rate</div>
                <div class="stat-value">${collisionRate}%</div>
            </div>
        </div>
        
        <div class="stat-card-large full-width">
            <div class="stat-icon">🎯</div>
            <div class="stat-info">
                <div class="stat-label">Records with Collisions</div>
                <div class="stat-value">${recordsWithCollisions} / ${totalRecords}</div>
            </div>
        </div>
    `;
}

// Handle Modal Insert
function handleModalInsert(event) {
    event.preventDefault();
    
    const username = document.getElementById('modalInsertUsername').value.trim();
    const phone = document.getElementById('modalInsertPhone').value.trim();
    const address = document.getElementById('modalInsertAddress').value.trim();

    if (!username || !phone || !address) {
        showMessage('All fields are required!', 'error');
        return false;
    }

    // Check if already exists
    if (recordsMap.has(username)) {
        showMessage('Username already exists!', 'error');
        return false;
    }

    // Check phone
    for (let [key, record] of recordsMap) {
        if (record.phoneNumber === phone) {
            showMessage('Phone number already exists!', 'error');
            return false;
        }
    }

    // Insert into both tables
    const usernameResult = usernameTable.insert(username, phone, address);
    const phoneResult = phoneTable.insert(username, phone, address);

    if (usernameResult.success && phoneResult.success) {
        recordsMap.set(username, { username, phoneNumber: phone, address });
        showMessage(`✓ Record inserted successfully!\n\nUsername: ${username}\nPhone: ${phone}\nAddress: ${address}`, 'success');
        
        // Clear form
        document.getElementById('modalInsertForm').reset();
        document.getElementById('insertFormContainer').style.display = 'none';
        
        // Update display
        updateStats();
        saveToLocalStorage();
        displayTable('all');
        
        // Close modal after short delay
        setTimeout(() => {
            closeInsertModal();
        }, 1500);
    } else {
        showMessage('Error inserting record!', 'error');
    }

    return false;
}

// Handle Modal Search
function handleModalSearch(event) {
    event.preventDefault();
    
    const searchKey = document.getElementById('modalSearchKey').value.trim();
    const searchType = document.querySelector('input[name="modalSearchType"]:checked').value;

    if (!searchKey) {
        showMessage('Please enter a search key!', 'error');
        return false;
    }

    const table = searchType === 'username' ? usernameTable : phoneTable;
    const result = table.search(searchKey);

    const resultDiv = document.getElementById('modalSearchResult');
    
    if (result.success) {
        resultDiv.innerHTML = `
            <div class="search-result-box success">
                <h4>✓ Record Found</h4>
                <div class="result-details">
                    <p><strong>Username:</strong> ${result.record.username}</p>
                    <p><strong>Phone:</strong> ${result.record.phoneNumber}</p>
                    <p><strong>Address:</strong> ${result.record.address}</p>
                    <hr>
                    <p class="hash-info"><strong>Hash Value:</strong> ${result.hash}</p>
                    <p class="hash-info"><strong>Index:</strong> ${result.index}</p>
                    <p class="hash-info"><strong>Probes:</strong> ${result.probes}</p>
                </div>
            </div>
        `;
    } else {
        resultDiv.innerHTML = `
            <div class="search-result-box error">
                <h4>✗ Record Not Found</h4>
                <p>No record found with ${searchType}: "${searchKey}"</p>
            </div>
        `;
    }

    return false;
}

// Handle Modal Delete
function handleModalDelete(event) {
    event.preventDefault();
    
    const deleteKey = document.getElementById('modalDeleteKey').value.trim();
    const deleteType = document.querySelector('input[name="modalDeleteType"]:checked').value;

    if (!deleteKey) {
        showMessage('Please enter a delete key!', 'error');
        return false;
    }

    // Search first to verify it exists
    const table = deleteType === 'username' ? usernameTable : phoneTable;
    const searchResult = table.search(deleteKey);

    if (!searchResult.success) {
        showMessage(`Record not found with ${deleteType}: "${deleteKey}"`, 'error');
        return false;
    }

    // Confirm deletion
    if (!confirm(`Are you sure you want to delete this record?\n\nUsername: ${searchResult.record.username}\nPhone: ${searchResult.record.phoneNumber}\nAddress: ${searchResult.record.address}`)) {
        return false;
    }

    // Delete from both tables
    const usernameResult = usernameTable.delete(searchResult.record.username);
    const phoneResult = phoneTable.delete(searchResult.record.phoneNumber);

    if (usernameResult.success && phoneResult.success) {
        recordsMap.delete(searchResult.record.username);
        showMessage(`✓ Record deleted successfully!\n\nUsername: ${searchResult.record.username}\nPhone: ${searchResult.record.phoneNumber}`, 'success');
        
        // Clear form
        document.getElementById('modalDeleteForm').reset();
        document.getElementById('deleteFormContainer').style.display = 'none';
        
        // Update display
        updateStats();
        saveToLocalStorage();
        displayTable('all');
        
        // Close modal after short delay
        setTimeout(() => {
            closeDeleteModal();
        }, 1500);
    } else {
        showMessage('Error deleting record!', 'error');
    }

    return false;
}

// Close modal when clicking outside
window.onclick = function(event) {
    const insertModal = document.getElementById('insertModal');
    const searchModal = document.getElementById('searchModal');
    const deleteModal = document.getElementById('deleteModal');
    const dataModal = document.getElementById('dataModal');
    const tableModal = document.getElementById('tableModal');
    const statsModal = document.getElementById('statsModal');
    
    if (event.target === insertModal) {
        closeInsertModal();
    }
    if (event.target === searchModal) {
        closeSearchModal();
    }
    if (event.target === deleteModal) {
        closeDeleteModal();
    }
    if (event.target === dataModal) {
        closeDataModal();
    }
    if (event.target === tableModal) {
        closeTableModal();
    }
    if (event.target === statsModal) {
        closeStatsModal();
    }
}

// Tab switching
function switchTab(tabName) {
    // Hide all tabs
    document.querySelectorAll('.tab-content').forEach(tab => {
        tab.classList.remove('active');
    });
    document.querySelectorAll('.tab-btn').forEach(btn => {
        btn.classList.remove('active');
    });

    // Show selected tab
    document.getElementById(tabName + 'Tab').classList.add('active');
    event.target.classList.add('active');
}

// Handle Insert
function handleInsert(event) {
    event.preventDefault();
    
    const username = document.getElementById('insertUsername').value.trim();
    const phone = document.getElementById('insertPhone').value.trim();
    const address = document.getElementById('insertAddress').value.trim();

    if (!username || !phone || !address) {
        showMessage('All fields are required!', 'error');
        return false;
    }

    // Check if already exists
    if (recordsMap.has(username)) {
        showMessage('Username already exists!', 'error');
        return false;
    }

    // Check phone
    for (let [key, record] of recordsMap) {
        if (record.phoneNumber === phone) {
            showMessage('Phone number already exists!', 'error');
            return false;
        }
    }

    // Insert into both tables
    const usernameResult = usernameTable.insert(username, phone, address);
    const phoneResult = phoneTable.insert(username, phone, address);

    if (usernameResult.success && phoneResult.success) {
        recordsMap.set(username, { username, phoneNumber: phone, address });
        showMessage(`✓ Record inserted successfully!\n\nUsername: ${username}\nPhone: ${phone}\nAddress: ${address}`, 'success');
        
        // Clear form
        document.getElementById('insertForm').reset();
        
        // Update display
        updateStats();
        saveToLocalStorage();
        displayTable('username');
    } else {
        showMessage('Error inserting record!', 'error');
    }

    return false;
}

// Handle Search
function handleSearch(event) {
    event.preventDefault();
    
    const searchKey = document.getElementById('searchKey').value.trim();
    const searchType = document.querySelector('input[name="searchType"]:checked').value;

    if (!searchKey) {
        showMessage('Please enter a search key!', 'error');
        return false;
    }

    const table = searchType === 'username' ? usernameTable : phoneTable;
    const result = table.search(searchKey);

    const resultDiv = document.getElementById('searchResult');
    
    if (result.success) {
        resultDiv.innerHTML = `
            <div class="search-result-box success">
                <h4>✓ Record Found</h4>
                <div class="result-details">
                    <p><strong>Username:</strong> ${result.record.username}</p>
                    <p><strong>Phone:</strong> ${result.record.phoneNumber}</p>
                    <p><strong>Address:</strong> ${result.record.address}</p>
                    <hr>
                    <p class="hash-info"><strong>Hash Value:</strong> ${result.hash}</p>
                    <p class="hash-info"><strong>Index:</strong> ${result.index}</p>
                    <p class="hash-info"><strong>Probes:</strong> ${result.probes}</p>
                </div>
            </div>
        `;
    } else {
        resultDiv.innerHTML = `
            <div class="search-result-box error">
                <h4>✗ Record Not Found</h4>
                <p>No record found with ${searchType}: "${searchKey}"</p>
            </div>
        `;
    }

    return false;
}

// Handle Delete
function handleDelete(event) {
    event.preventDefault();
    
    const deleteKey = document.getElementById('deleteKey').value.trim();
    const deleteType = document.querySelector('input[name="deleteType"]:checked').value;

    if (!deleteKey) {
        showMessage('Please enter a key to delete!', 'error');
        return false;
    }

    const table = deleteType === 'username' ? usernameTable : phoneTable;
    const searchResult = table.search(deleteKey);

    if (!searchResult.success) {
        showMessage('Record not found!', 'error');
        return false;
    }

    const username = searchResult.record.username;
    
    // Delete from both tables
    usernameTable.delete(username);
    phoneTable.delete(searchResult.record.phoneNumber);
    recordsMap.delete(username);

    showMessage(`✓ Record deleted successfully!\n\nUsername: ${username}`, 'success');
    
    // Clear form
    document.getElementById('deleteForm').reset();
    
    // Update display
    updateStats();
    saveToLocalStorage();
    displayTable('username');

    return false;
}

// Display table
function displayTable(type) {
    const displayDiv = document.getElementById('tableDisplay');
    
    // Get all records and display in a gorgeous table format
    const records = Array.from(recordsMap.values());
    
    if (records.length === 0) {
        displayDiv.innerHTML = `
            <div class="empty-state">
                <p>📋 No records to display. Load sample data or insert new records.</p>
            </div>
        `;
        return;
    }
    
    displayDiv.innerHTML = `
        <div class="gorgeous-table-container">
            <h3>📋 All Records (${records.length} / ${MAX_RECORDS})</h3>
            <table class="gorgeous-table">
                <thead>
                    <tr>
                        <th class="col-no">No</th>
                        <th class="col-username">Username</th>
                        <th class="col-phone">Phone Number</th>
                        <th class="col-address">Address</th>
                    </tr>
                </thead>
                <tbody>
                    ${records.map((record, index) => `
                        <tr class="${index % 2 === 0 ? 'even-row' : 'odd-row'}">
                            <td class="col-no">${index + 1}</td>
                            <td class="col-username">${record.username}</td>
                            <td class="col-phone">${record.phoneNumber}</td>
                            <td class="col-address">${record.address}</td>
                        </tr>
                    `).join('')}
                </tbody>
            </table>
        </div>
    `;
}

function renderTableView(view, type) {
    // Legacy function - now using displayTable for gorgeous display
    let html = '<div class="hash-table-display">';
    
    view.forEach(slot => {
        if (slot.status === 'empty') {
            html += `<div class="table-slot empty"><span class="slot-index">[${slot.index}]</span> Empty</div>`;
        } else if (slot.status === 'deleted') {
            html += `<div class="table-slot deleted"><span class="slot-index">[${slot.index}]</span> Deleted</div>`;
        } else {
            html += `
                <div class="table-slot occupied">
                    <span class="slot-index">[${slot.index}]</span>
                    <div class="slot-content">
                        <strong>${slot.key}</strong>
                        <small>Hash: ${slot.hash} | User: ${slot.username} | Phone: ${slot.phoneNumber}</small>
                    </div>
                </div>
            `;
        }
    });
    
    html += '</div>';
    return html;
}

// Load sample data
function loadSampleData() {
    if (!confirm('This will clear existing data and load 30 sample records. Continue?')) {
        return;
    }

    // Clear localStorage first to ensure clean slate
    localStorage.removeItem('phoneDirectoryData');
    
    clearAllData(false);

    let successCount = 0;
    let failed = [];

    console.log(`Loading ${SAMPLE_DATA.length} sample records...`);

    SAMPLE_DATA.forEach((record, index) => {
        const result = usernameTable.insert(record.username, record.phoneNumber, record.address);
        const result2 = phoneTable.insert(record.username, record.phoneNumber, record.address);

        // Always add to recordsMap so the UI shows all sample entries
        recordsMap.set(record.username, record);

        if (result.success && result2.success) {
            successCount++;
        } else {
            failed.push(record.username);
            console.warn(`Insert ${index + 1} failed for ${record.username}:`, result.message, result2.message);
        }
    });

    console.log(`Loaded ${SAMPLE_DATA.length} records into recordsMap. Hash table success: ${successCount}, Failed: ${failed.length}`);
    console.log(`recordsMap size: ${recordsMap.size}`);

    if (failed.length === 0) {
        showMessage(`✓ Loaded ${successCount} sample records successfully!`, 'success');
    } else {
        showMessage(`⚠ Loaded ${SAMPLE_DATA.length} sample records. ${failed.length} entries could not be stored in the hash tables but are visible in the UI.`, 'warning');
    }

    updateStats();
    saveToLocalStorage();
    displayTable('all');

    // Scroll the table container to the top so first records are visible
    setTimeout(() => {
        const container = document.querySelector('.gorgeous-table-container');
        if (container) container.scrollTop = 0;
    }, 50);
}

// Export data
function exportData() {
    const records = Array.from(recordsMap.values());
    const dataStr = JSON.stringify(records, null, 2);
    const blob = new Blob([dataStr], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    
    const a = document.createElement('a');
    a.href = url;
    a.download = `phone_directory_${new Date().toISOString().split('T')[0]}.json`;
    a.click();
    
    showMessage(`✓ Data exported successfully (${records.length} records).`, 'success');
}

// Import data
function importData() {
    document.getElementById('fileInput').click();
}

function handleFileImport(event) {
    const file = event.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = function(e) {
        try {
            const records = JSON.parse(e.target.result);
            
            if (!confirm(`This will replace existing data with ${records.length} records. Continue?`)) {
                return;
            }

            clearAllData(false);

            let successCount = 0;
            let failed = [];
            for (let i = 0; i < records.length; i++) {
                const record = records[i];
                const result = usernameTable.insert(record.username, record.phoneNumber, record.address);
                const result2 = phoneTable.insert(record.username, record.phoneNumber, record.address);

                // Always add to recordsMap to ensure UI shows imported entries
                recordsMap.set(record.username, record);

                if (result.success && result2.success) {
                    successCount++;
                } else {
                    failed.push(record.username);
                }
            }

            if (failed.length === 0) {
                showMessage(`✓ Imported ${successCount} records successfully!`, 'success');
            } else {
                showMessage(`⚠ Imported ${records.length} records. ${failed.length} could not be stored in the hash tables but are visible in the UI.`, 'warning');
            }

            updateStats();
            saveToLocalStorage();
            displayTable('all');
        } catch (error) {
            showMessage('Error importing data: Invalid file format!', 'error');
        }
    };
    reader.readAsText(file);
}

// Clear all data
function clearAllData(confirm = true) {
    if (confirm && !window.confirm('Are you sure you want to clear all data?')) {
        return;
    }

    usernameTable.clear();
    phoneTable.clear();
    recordsMap.clear();

    if (confirm) {
        showMessage('✓ All data cleared!', 'success');
    }
    
    updateStats();
    saveToLocalStorage();
    displayTable('username');
}

// Update statistics
function updateStats() {
    document.getElementById('recordCount').textContent = `${recordsMap.size} / ${MAX_RECORDS}`;
    const loadFactor = ((recordsMap.size / TABLE_SIZE) * 100).toFixed(1);
    document.getElementById('loadFactor').textContent = loadFactor + '%';
}

// Show message
function showMessage(message, type) {
    const messageBox = document.getElementById('messageBox');
    messageBox.textContent = message;
    messageBox.className = `message-box ${type} show`;
    
    setTimeout(() => {
        messageBox.classList.remove('show');
    }, 4000);
}

// LocalStorage functions
function saveToLocalStorage() {
    const data = {
        records: Array.from(recordsMap.values())
    };
    localStorage.setItem('phoneDirectoryData', JSON.stringify(data));
}

function loadFromLocalStorage() {
    const data = localStorage.getItem('phoneDirectoryData');
    if (data) {
        try {
            // Clear current in-memory data before loading to avoid duplicates
            clearAllData(false);

            const parsed = JSON.parse(data);
            parsed.records.forEach((record, index) => {
                const r1 = usernameTable.insert(record.username, record.phoneNumber, record.address);
                const r2 = phoneTable.insert(record.username, record.phoneNumber, record.address);
                recordsMap.set(record.username, record);

                if (!r1.success || !r2.success) {
                    console.warn(`Load insert failed for ${record.username}:`, r1.message, r2.message);
                }
            });

            console.log(`Loaded ${recordsMap.size} records from localStorage.`);
        } catch (error) {
            console.error('Error loading data:', error);
        }
    }
}

// Refresh display by reloading from localStorage and re-rendering the table
function refreshDisplay() {
    console.log('Refreshing display from localStorage...');
    loadFromLocalStorage();
    updateStats();
    displayTable('all');
    showMessage('🔄 Display refreshed!', 'success');

    // Scroll the table container to the top so first records are visible
    setTimeout(() => {
        const container = document.querySelector('.gorgeous-table-container');
        if (container) container.scrollTop = 0;
    }, 50);
}
