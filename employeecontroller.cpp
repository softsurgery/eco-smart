#include "employeecontroller.h"
#include "databaseutils.h"
#include <QSqlQuery>
#include <QVariant>

EmployeeController::EmployeeController(QObject *parent)
    : QObject(parent) {}

void EmployeeController::createEmployee(const Employee &employee) {
    QString queryStr = "INSERT INTO employees (id, name, surname, job, phone, available, created_at, updated_at) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    QVariantList params = {employee.getId(), employee.getName(), employee.getSurname(), employee.getJob(), employee.getPhone(), employee.isAvailable(), employee.getCreatedAt(), employee.getUpdatedAt()};

    if (DatabaseUtils::executeQuery(queryStr, params)) {
        m_employees.append(employee);
        emit employeeCreated(employee);
    }
}

QList<Employee> EmployeeController::readEmployees() const {
    QList<Employee> employees;
    QSqlQuery query("SELECT id, name, surname, job, phone, available, created_at, updated_at FROM employees");

    while (query.next()) {
        Employee employee;
        employee.setId(query.value(0).toInt());
        employee.setName(query.value(1).toString());
        employee.setSurname(query.value(2).toString());
        employee.setJob(query.value(3).toString());
        employee.setPhone(query.value(4).toString());
        employee.setAvailable(query.value(5).toBool());
        employee.setCreatedAt(query.value(6).toDateTime());
        employee.setUpdatedAt(query.value(7).toDateTime());
        employees.append(employee);
    }

    return employees;
}

void EmployeeController::updateEmployee(const Employee &employee) {
    QString queryStr = "UPDATE employees SET name = ?, surname = ?, job = ?, phone = ?, available = ?, updated_at = ? WHERE id = ?";
    QVariantList params = {employee.getName(), employee.getSurname(), employee.getJob(), employee.getPhone(), employee.isAvailable(), employee.getUpdatedAt(), employee.getId()};

    if (DatabaseUtils::executeQuery(queryStr, params)) {
        for (int i = 0; i < m_employees.size(); ++i) {
            if (m_employees[i].getId() == employee.getId()) {
                m_employees[i] = employee;
                emit employeeUpdated(employee);
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