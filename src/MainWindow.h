#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QStatusBar>
#include <memory>
#include "hashtable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Main window for Hash Table GUI application
 * Student ID: F23040111
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // Button click handlers
    void on_btnInsert_clicked();
    void on_btnSearchUsername_clicked();
    void on_btnSearchPhone_clicked();
    void on_btnDeleteUsername_clicked();
    void on_btnDeletePhone_clicked();
    void on_btnDisplayAll_clicked();
    void on_btnSave_clicked();
    void on_btnLoad_clicked();
    void on_btnStatistics_clicked();
    void on_btnClear_clicked();

private:
    Ui::MainWindow *ui;
    
    // Hash tables
    std::unique_ptr<HashTable> usernameTable;
    std::unique_ptr<HashTable> phoneTable;
    
    // Data files
    const QString usernameFile = "data/records_username.txt";
    const QString phoneFile = "data/records_phone.txt";
    
    // Helper functions
    void initializeHashTables();
    void loadDataFromFiles();
    void saveDataToFiles();
    void clearInputFields();
    void displayAllRecords();
    void updateStatusBar(const QString& message, int timeout = 3000);
    void showSuccessMessage(const QString& title, const QString& message);
    void showErrorMessage(const QString& title, const QString& message);
    void showInfoMessage(const QString& title, const QString& message);
    bool syncTables(const Record& record, const QString& operation);
    QString validateInput();
};

#endif // MAINWINDOW_H
