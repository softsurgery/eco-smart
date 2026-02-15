#include "mainwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Setup Oracle database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("ORACLE");//inserer le nom de la source de données
    db.setUserName("TEST");//inserer nom de l'utilisateur
    db.setPassword("record25");//inserer mot de passe de cet utilisateur

    
    if (!db.open()) {
        QString errorMsg = QString("Failed to connect to Oracle database after trying multiple methods.\n\n"
                                 "Last error: %1\n\n"
                                 "Troubleshooting steps:\n"
                                 "1. Ensure Oracle 11g server is running and accessible\n"
                                 "2. Check server address and port (currently trying localhost:1521)\n"
                                 "3. Verify username (SYSTEM) and password\n"
                                 "4. Install Oracle 11g ODBC driver\n"
                                 "5. Check Windows firewall settings")
                          .arg(db.lastError().text());
        qDebug() << errorMsg;
        QMessageBox::critical(nullptr, "Database Connection Error", errorMsg);
        return 1;
    }
    
    if (db.isOpen()) {
        qDebug() << "Database connected successfully!";
    }

    // Create employees table and sequence if they don't exist
    QSqlQuery createTable;
    QString createTableQuery = 
        "CREATE TABLE employees ("
        "id NUMBER PRIMARY KEY,"
        "name VARCHAR2(50) NOT NULL,"
        "surname VARCHAR2(50) NOT NULL,"
        "job VARCHAR2(100),"
        "phone VARCHAR2(20),"
        "available NUMBER(1) DEFAULT 1,"
        "created_at TIMESTAMP,"
        "updated_at TIMESTAMP"
        ")";
    
    // Try to create table (ignore if exists)
    createTable.exec(createTableQuery);
    
    // Create sequence for auto-incrementing ID
    QSqlQuery createSeq;
    createSeq.exec("CREATE SEQUENCE employees_seq START WITH 1 INCREMENT BY 1");
    
    // Create trigger for auto-increment
    QSqlQuery createTrigger;
    QString triggerQuery = 
        "CREATE OR REPLACE TRIGGER employees_id_trigger "
        "BEFORE INSERT ON employees "
        "FOR EACH ROW "
        "BEGIN "
        "  SELECT employees_seq.NEXTVAL INTO :NEW.id FROM dual; "
        "END;";
    createTrigger.exec(triggerQuery);

    MainWindow w;
    w.show();
    return a.exec();
}
