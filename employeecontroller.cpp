#include "employeecontroller.h"
#include "databaseutils.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include <QMutexLocker>

// Static members for thread-safe ID generation
QMutex EmployeeController::s_idMutex;
int EmployeeController::s_lastId = 0;

EmployeeController::EmployeeController(QObject *parent)
    : QObject(parent) {
    // Initialize the static ID counter based on existing database records
    initializeIdCounter();
}

int EmployeeController::generateUniqueId() {
    QMutexLocker locker(&s_idMutex);
    
    // Generate timestamp-based ID (current time in milliseconds since epoch)
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    int newId = static_cast<int>(timestamp % 2147483647); // Keep within int range
    
    // Ensure uniqueness by incrementing if we get the same timestamp
    if (newId <= s_lastId) {
        newId = s_lastId + 1;
    }
    
    s_lastId = newId;
    return newId;
}

void EmployeeController::initializeIdCounter() {
    QMutexLocker locker(&s_idMutex);
    
    // Only initialize once
    if (s_lastId != 0) {
        return;
    }
    
    // Query the database to find the current maximum ID
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        // If database is not open, use timestamp as starting point
        s_lastId = static_cast<int>(QDateTime::currentMSecsSinceEpoch() % 2147483647);
        return;
    }
    
    QSqlQuery query(db);
    if (query.exec("SELECT MAX(id) FROM employees")) {
        if (query.next() && !query.value(0).isNull()) {
            int maxId = query.value(0).toInt();
            // Start with the higher of: max existing ID or current timestamp
            qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
            int timestampId = static_cast<int>(timestamp % 2147483647);
            s_lastId = qMax(maxId, timestampId);
        } else {
            // No records exist, use current timestamp
            s_lastId = static_cast<int>(QDateTime::currentMSecsSinceEpoch() % 2147483647);
        }
    } else {
        // Query failed, use timestamp
        s_lastId = static_cast<int>(QDateTime::currentMSecsSinceEpoch() % 2147483647);
    }
    query.finish();
}

void EmployeeController::createEmployee(const Employee &employee) {
    Employee newEmployee = employee;
    
    // Generate unique ID manually
    int newId = generateUniqueId();
    newEmployee.setId(newId);
    
    QDateTime currentTime = QDateTime::currentDateTime();
    newEmployee.setCreatedAt(currentTime);
    newEmployee.setUpdatedAt(currentTime);
    
    // Insert with manually generated ID
    QString queryStr = "INSERT INTO employees (id, name, surname, job, phone, available, created_at, updated_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    QVariantList params = {newEmployee.getId(), newEmployee.getName(), newEmployee.getSurname(), newEmployee.getJob(), 
                          newEmployee.getPhone(), newEmployee.isAvailable() ? 1 : 0, 
                          newEmployee.getCreatedAt(), 
                          newEmployee.getUpdatedAt()};

    if (DatabaseUtils::executeQuery(queryStr, params)) {
        m_employees.append(newEmployee);
        emit employeeCreated(newEmployee);
    }
}

QList<Employee> EmployeeController::readEmployees() const {
    QList<Employee> employees;
    
    // Check database connection first
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database is not connected";
        return employees;
    }
    
    QSqlQuery query(db);
    QString queryStr = "SELECT id, name, surname, job, phone, available, created_at, updated_at FROM employees";
    
    if (!query.prepare(queryStr)) {
        qDebug() << "Failed to prepare read query:" << query.lastError().text();
        return employees;
    }

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text() << ", " << query.lastError().nativeErrorCode() << " " << query.lastError().driverText();
        query.finish();
        return employees;
    }

    while (query.next()) {
        Employee employee;
        employee.setId(query.value(0).toInt());
        employee.setName(query.value(1).toString());
        employee.setSurname(query.value(2).toString());
        employee.setJob(query.value(3).toString());
        employee.setPhone(query.value(4).toString());
        employee.setAvailable(query.value(5).toInt() == 1);
        employee.setCreatedAt(query.value(6).toDateTime());
        employee.setUpdatedAt(query.value(7).toDateTime());
        employees.append(employee);
    }
    
    query.finish();
    return employees;
}

void EmployeeController::updateEmployee(const Employee &employee) {
    Employee updatedEmployee = employee;
    updatedEmployee.setUpdatedAt(QDateTime::currentDateTime());
    
    QString queryStr = "UPDATE employees SET name = ?, surname = ?, job = ?, phone = ?, available = ?, updated_at = ? WHERE id = ?";
    QVariantList params = {updatedEmployee.getName(), updatedEmployee.getSurname(), updatedEmployee.getJob(), 
                          updatedEmployee.getPhone(), updatedEmployee.isAvailable() ? 1 : 0, 
                          updatedEmployee.getUpdatedAt(), updatedEmployee.getId()};

    if (DatabaseUtils::executeQuery(queryStr, params)) {
        for (int i = 0; i < m_employees.size(); ++i) {
            if (m_employees[i].getId() == updatedEmployee.getId()) {
                m_employees[i] = updatedEmployee;
                return;
            }
        }
        emit employeeUpdated(updatedEmployee);
    }
}

void EmployeeController::deleteEmployee(int id) {
    QString queryStr = "DELETE FROM employees WHERE id = ?";
    QVariantList params = {id};

    if (DatabaseUtils::executeQuery(queryStr, params)) {
        for (int i = 0; i < m_employees.size(); ++i) {
            if (m_employees[i].getId() == id) {
                m_employees.removeAt(i);
                return;
            }
        }
        emit employeeDeleted(id);
    }
}