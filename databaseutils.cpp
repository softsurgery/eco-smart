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
    
    QSqlQuery query(db);
    query.prepare(queryStr);

    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    if (!query.exec()) {
        qDebug() << "Database error:" << query.lastError().text();
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
        return false;
    }

    return true;
}