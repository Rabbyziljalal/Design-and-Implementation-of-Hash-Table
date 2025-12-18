#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QDir>

/**
 * @brief Main entry point for Qt GUI application
 * Student ID: F23040111
 * Project: Advanced Hash Table - Phone Directory System
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application information
    app.setApplicationName("Advanced Hash Table - Phone Directory");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("F23040111");
    
    // Create data directory if it doesn't exist
    QDir dir;
    if (!dir.exists("data")) {
        dir.mkpath("data");
    }
    
    // Apply stylesheet for better appearance
    QString styleSheet = R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        QGroupBox {
            font-weight: bold;
            border: 2px solid #cccccc;
            border-radius: 5px;
            margin-top: 10px;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px 0 5px;
        }
        QPushButton {
            background-color: #4CAF50;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #45a049;
        }
        QPushButton:pressed {
            background-color: #3d8b40;
        }
        QLineEdit {
            padding: 6px;
            border: 1px solid #cccccc;
            border-radius: 4px;
            font-size: 10pt;
        }
        QLineEdit:focus {
            border: 2px solid #4CAF50;
        }
        QTableWidget {
            gridline-color: #d0d0d0;
            background-color: white;
            alternate-background-color: #f9f9f9;
        }
        QHeaderView::section {
            background-color: #4CAF50;
            color: white;
            padding: 6px;
            border: none;
            font-weight: bold;
        }
        QStatusBar {
            background-color: #e0e0e0;
            color: #333333;
            font-weight: bold;
        }
    )";
    
    app.setStyleSheet(styleSheet);
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}
