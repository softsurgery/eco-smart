#include "employeecontroller.h"
#include "databaseutils.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QDebug>

EmployeeController::EmployeeController(QObject *parent)
    : QObject(parent) {}

void EmployeeController::createEmployee(const Employee &employee) {
    Employee newEmployee = employee;
    newEmployee.setCreatedAt(QDateTime::currentDateTime());
    newEmployee.setUpdatedAt(QDateTime::currentDateTime());
    
    // For Oracle with auto-increment trigger, don't specify ID in INSERT
    QString queryStr = "INSERT INTO employees (name, surname, job, phone, available, created_at, updated_at) VALUES (?, ?, ?, ?, ?, ?, ?)";
    QVariantList params = {newEmployee.getName(), newEmployee.getSurname(), newEmployee.getJob(), 
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
                emit employeeUpdated(updatedEmployee);
                return;
            }
        }
    }
}

void EmployeeController::deleteEmployee(int id) {
    QString queryStr = "DELETE FROM employees WHERE id = ?";
    QVariantList params = {id};

    if (DatabaseUtils::executeQuery(queryStr, params)) {
        for (int i = 0; i < m_employees.size(); ++i) {
            if (m_employees[i].getId() == id) {
                m_employees.removeAt(i);
                emit employeeDeleted(id);
                return;
            }
        }
    }
}