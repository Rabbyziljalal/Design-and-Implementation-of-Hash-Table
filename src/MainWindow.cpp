#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "file_handler.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QHeaderView>
#include <QFile>
#include <QTextStream>
#include <iomanip>
#include <sstream>

/**
 * @brief Constructor - Initialize GUI and hash tables
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Set window properties
    setWindowTitle("Advanced Hash Table - Phone Directory System (F23040111)");
    setMinimumSize(1000, 700);
    
    // Initialize hash tables
    initializeHashTables();
    
    // Load existing data
    loadDataFromFiles();
    
    // Setup table widget
    ui->tableWidget->setColumnCount(4);
    QStringList headers;
    headers << "Index" << "Username" << "Phone Number" << "Address";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setAlternatingRowColors(true);
    
    // Display initial data
    displayAllRecords();
    
    // Update status bar
    updateStatusBar("Ready. Loaded " + QString::number(usernameTable->getCount()) + " records.");
}

/**
 * @brief Destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Handle window close event - save data before exit
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit Confirmation",
                                  "Do you want to save data before exiting?",
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    
    if (reply == QMessageBox::Yes) {
        saveDataToFiles();
        event->accept();
    } else if (reply == QMessageBox::No) {
        event->accept();
    } else {
        event->ignore();
    }
}

/**
 * @brief Initialize hash tables with default size
 */
void MainWindow::initializeHashTables()
{
    const int TABLE_SIZE = 30; // Size matches number of records
    usernameTable = std::make_unique<HashTable>(TABLE_SIZE, "username");
    phoneTable = std::make_unique<HashTable>(TABLE_SIZE, "phone");
}

/**
 * @brief Load data from files at startup
 */
void MainWindow::loadDataFromFiles()
{
    if (FileHandler::fileExists(usernameFile.toStdString())) {
        int count = usernameTable->loadFromFile(usernameFile.toStdString());
        updateStatusBar("Loaded " + QString::number(count) + " records from username file.");
    }
    
    if (FileHandler::fileExists(phoneFile.toStdString())) {
        phoneTable->loadFromFile(phoneFile.toStdString());
    }
}

/**
 * @brief Save data to files
 */
void MainWindow::saveDataToFiles()
{
    usernameTable->saveToFile(usernameFile.toStdString());
    phoneTable->saveToFile(phoneFile.toStdString());
    updateStatusBar("Data saved successfully!");
}

/**
 * @brief Clear all input fields
 */
void MainWindow::clearInputFields()
{
    ui->lineEditUsername->clear();
    ui->lineEditPhone->clear();
    ui->lineEditAddress->clear();
    ui->lineEditUsername->setFocus();
}

/**
 * @brief Validate input fields
 */
QString MainWindow::validateInput()
{
    QString username = ui->lineEditUsername->text().trimmed();
    QString phone = ui->lineEditPhone->text().trimmed();
    
    if (username.isEmpty()) {
        return "Username cannot be empty!";
    }
    
    if (phone.isEmpty()) {
        return "Phone number cannot be empty!";
    }
    
    return "";
}

/**
 * @brief Synchronize record across both hash tables
 */
bool MainWindow::syncTables(const Record& record, const QString& operation)
{
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
 * @brief Display all records in table widget
 */
void MainWindow::displayAllRecords()
{
    ui->tableWidget->setRowCount(0);
    
    int rowCount = 0;
    for (int i = 0; i < usernameTable->getSize(); i++) {
        const Record* rec = usernameTable->getRecordAt(i);
        if (rec && !rec->isEmpty && !rec->isDeleted) {
            ui->tableWidget->insertRow(rowCount);
            ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(QString::number(i)));
            ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(QString::fromStdString(rec->username)));
            ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(QString::fromStdString(rec->phoneNumber)));
            ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(QString::fromStdString(rec->address)));
            rowCount++;
        }
    }
    
    updateStatusBar("Displaying " + QString::number(rowCount) + " records.");
}

/**
 * @brief Update status bar with message
 */
void MainWindow::updateStatusBar(const QString& message, int timeout)
{
    ui->statusbar->showMessage(message, timeout);
}

/**
 * @brief Show success message box
 */
void MainWindow::showSuccessMessage(const QString& title, const QString& message)
{
    QMessageBox::information(this, title, message);
}

/**
 * @brief Show error message box
 */
void MainWindow::showErrorMessage(const QString& title, const QString& message)
{
    QMessageBox::critical(this, title, message);
}

/**
 * @brief Show info message box
 */
void MainWindow::showInfoMessage(const QString& title, const QString& message)
{
    QMessageBox::information(this, title, message);
}

/**
 * @brief Insert new record button handler
 */
void MainWindow::on_btnInsert_clicked()
{
    QString error = validateInput();
    if (!error.isEmpty()) {
        showErrorMessage("Input Error", error);
        return;
    }
    
    QString username = ui->lineEditUsername->text().trimmed();
    QString phone = ui->lineEditPhone->text().trimmed();
    QString address = ui->lineEditAddress->text().trimmed();
    
    Record record(username.toStdString(), phone.toStdString(), address.toStdString());
    
    if (syncTables(record, "insert")) {
        showSuccessMessage("Success", "Record inserted successfully!\n\nUsername: " + username + "\nPhone: " + phone);
        displayAllRecords();
        clearInputFields();
        updateStatusBar("Record inserted. Total: " + QString::number(usernameTable->getCount()));
    } else {
        showErrorMessage("Insert Failed", "Cannot insert record!\nDuplicate key or table is full.");
    }
}

/**
 * @brief Search by username button handler
 */
void MainWindow::on_btnSearchUsername_clicked()
{
    QString username = ui->lineEditUsername->text().trimmed();
    
    if (username.isEmpty()) {
        showErrorMessage("Input Error", "Please enter a username to search.");
        return;
    }
    
    Record* found = usernameTable->search(username.toStdString());
    
    if (found) {
        int searchLength = usernameTable->getSearchLength(username.toStdString());
        
        QString message = QString("✓ RECORD FOUND!\n\n") +
                         "Username: " + QString::fromStdString(found->username) + "\n" +
                         "Phone: " + QString::fromStdString(found->phoneNumber) + "\n" +
                         "Address: " + QString::fromStdString(found->address) + "\n\n" +
                         "Search Length: " + QString::number(searchLength) + " probe(s)";
        
        showSuccessMessage("Search Result", message);
        updateStatusBar("Found record. Search length: " + QString::number(searchLength) + " probes.");
        
        // Highlight in table
        for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
            if (ui->tableWidget->item(row, 1)->text() == QString::fromStdString(found->username)) {
                ui->tableWidget->selectRow(row);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(row, 0));
                break;
            }
        }
    } else {
        showErrorMessage("Not Found", "No record found with username: " + username);
        updateStatusBar("Record not found.");
    }
}

/**
 * @brief Search by phone number button handler
 */
void MainWindow::on_btnSearchPhone_clicked()
{
    QString phone = ui->lineEditPhone->text().trimmed();
    
    if (phone.isEmpty()) {
        showErrorMessage("Input Error", "Please enter a phone number to search.");
        return;
    }
    
    Record* found = phoneTable->search(phone.toStdString());
    
    if (found) {
        int searchLength = phoneTable->getSearchLength(phone.toStdString());
        
        QString message = QString("✓ RECORD FOUND!\n\n") +
                         "Username: " + QString::fromStdString(found->username) + "\n" +
                         "Phone: " + QString::fromStdString(found->phoneNumber) + "\n" +
                         "Address: " + QString::fromStdString(found->address) + "\n\n" +
                         "Search Length: " + QString::number(searchLength) + " probe(s)";
        
        showSuccessMessage("Search Result", message);
        updateStatusBar("Found record. Search length: " + QString::number(searchLength) + " probes.");
        
        // Highlight in table
        for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
            if (ui->tableWidget->item(row, 2)->text() == QString::fromStdString(found->phoneNumber)) {
                ui->tableWidget->selectRow(row);
                ui->tableWidget->scrollToItem(ui->tableWidget->item(row, 0));
                break;
            }
        }
    } else {
        showErrorMessage("Not Found", "No record found with phone number: " + phone);
        updateStatusBar("Record not found.");
    }
}

/**
 * @brief Delete by username button handler
 */
void MainWindow::on_btnDeleteUsername_clicked()
{
    QString username = ui->lineEditUsername->text().trimmed();
    
    if (username.isEmpty()) {
        showErrorMessage("Input Error", "Please enter a username to delete.");
        return;
    }
    
    Record* found = usernameTable->search(username.toStdString());
    if (found) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm Deletion",
                                      "Are you sure you want to delete this record?\n\n" +
                                      QString::fromStdString(found->username) + " - " +
                                      QString::fromStdString(found->phoneNumber),
                                      QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            Record record = *found;
            if (syncTables(record, "delete")) {
                showSuccessMessage("Success", "Record deleted successfully!");
                displayAllRecords();
                clearInputFields();
                updateStatusBar("Record deleted. Total: " + QString::number(usernameTable->getCount()));
            } else {
                showErrorMessage("Delete Failed", "Failed to delete record.");
            }
        }
    } else {
        showErrorMessage("Not Found", "No record found with username: " + username);
    }
}

/**
 * @brief Delete by phone number button handler
 */
void MainWindow::on_btnDeletePhone_clicked()
{
    QString phone = ui->lineEditPhone->text().trimmed();
    
    if (phone.isEmpty()) {
        showErrorMessage("Input Error", "Please enter a phone number to delete.");
        return;
    }
    
    Record* found = phoneTable->search(phone.toStdString());
    if (found) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm Deletion",
                                      "Are you sure you want to delete this record?\n\n" +
                                      QString::fromStdString(found->username) + " - " +
                                      QString::fromStdString(found->phoneNumber),
                                      QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            Record record = *found;
            if (syncTables(record, "delete")) {
                showSuccessMessage("Success", "Record deleted successfully!");
                displayAllRecords();
                clearInputFields();
                updateStatusBar("Record deleted. Total: " + QString::number(usernameTable->getCount()));
            } else {
                showErrorMessage("Delete Failed", "Failed to delete record.");
            }
        }
    } else {
        showErrorMessage("Not Found", "No record found with phone number: " + phone);
    }
}

/**
 * @brief Display all records button handler
 */
void MainWindow::on_btnDisplayAll_clicked()
{
    displayAllRecords();
}

/**
 * @brief Save to file button handler
 */
void MainWindow::on_btnSave_clicked()
{
    saveDataToFiles();
    showSuccessMessage("Success", "Data saved to files successfully!");
}

/**
 * @brief Load from file button handler
 */
void MainWindow::on_btnLoad_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Load Data",
                                  "This will clear current data and reload from files. Continue?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        usernameTable->clear();
        phoneTable->clear();
        loadDataFromFiles();
        displayAllRecords();
        showSuccessMessage("Success", "Data loaded from files successfully!");
    }
}

/**
 * @brief Display statistics button handler
 */
void MainWindow::on_btnStatistics_clicked()
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2);
    
    QString stats = "═══════════════════════════════════════\n";
    stats += "📊 HASH TABLE STATISTICS\n";
    stats += "═══════════════════════════════════════\n\n";
    
    stats += "📌 USERNAME HASH TABLE:\n";
    stats += "   • Table Size: " + QString::number(usernameTable->getSize()) + "\n";
    stats += "   • Record Count: " + QString::number(usernameTable->getCount()) + "\n";
    ss << "   • Load Factor: " << (usernameTable->getLoadFactor() * 100) << "%\n";
    ss << "   • Avg Search Length: " << usernameTable->getAverageSearchLength() << " probes\n\n";
    
    stats += QString::fromStdString(ss.str());
    ss.str("");
    
    stats += "📌 PHONE NUMBER HASH TABLE:\n";
    stats += "   • Table Size: " + QString::number(phoneTable->getSize()) + "\n";
    stats += "   • Record Count: " + QString::number(phoneTable->getCount()) + "\n";
    ss << "   • Load Factor: " << (phoneTable->getLoadFactor() * 100) << "%\n";
    ss << "   • Avg Search Length: " << phoneTable->getAverageSearchLength() << " probes\n";
    
    stats += QString::fromStdString(ss.str());
    
    showInfoMessage("Hash Table Statistics", stats);
}

/**
 * @brief Clear input fields button handler
 */
void MainWindow::on_btnClear_clicked()
{
    clearInputFields();
    updateStatusBar("Input fields cleared.");
}
