#include "databaseutils.h"
#include <QSqlDatabase>
#include <QMessageBox>

bool DatabaseUtils::executeQuery(const QString &queryStr, const QVariantList &params) {
    // Check if database is connected
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database is not connected";
        QMessageBox::warning(nullptr, "Database Warning", 
                           "Database connection is not available. Please check your connection settings.");
        return false;
    }
    
    // Test database connectivity
    if (!db.isValid()) {
        qDebug() << "Database connection is invalid";
        return false;
    }
    
    // Create query with proper connection reference
    QSqlQuery query(db);
    
    // Clear any previous state
    query.clear();
    
    // Prepare the query
    if (!query.prepare(queryStr)) {
        qDebug() << "Failed to prepare query:" << query.lastError().text();
        qDebug() << "Query was:" << queryStr;
        return false;
    }

    // Bind parameters
    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Database error:" << query.lastError().text();
        qDebug() << "Failed to execute query:" << query.lastError().text() << ", " << query.lastError().nativeErrorCode() << " " << query.lastError().driverText();
        qDebug() << "Query was:" << queryStr;
        qDebug() << "Parameters:" << params;
        
        // Show user-friendly error message for common Oracle errors
        QString errorText = query.lastError().text();
        if (errorText.contains("ORA-00942")) {
            QMessageBox::critical(nullptr, "Database Error", 
                                "Table not found. Please ensure the employees table exists.");
        } else if (errorText.contains("ORA-00001")) {
            QMessageBox::warning(nullptr, "Database Warning", 
                               "Duplicate entry. This record already exists.");
        } else {
            QMessageBox::critical(nullptr, "Database Error", 
                                "Query execution failed:\n" + errorText);
        }
        
        // Finish the query to clean up
        query.finish();
        return false;
    }
    
    // Clean up the query
    query.finish();
    return true;
}