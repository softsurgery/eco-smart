#ifndef EMPLOYEECONTROLLER_H
#define EMPLOYEECONTROLLER_H

#include "employee.h"
#include <QObject>
#include <QList>

class EmployeeController : public QObject {
    Q_OBJECT

public:
    explicit EmployeeController(QObject *parent = nullptr);

    void createEmployee(const Employee &employee);
    QList<Employee> readEmployees() const;
    void updateEmployee(const Employee &employee);
    void deleteEmployee(int id);

signals:
    void employeeCreated(const Employee &employee);
    void employeeUpdated(const Employee &employee);
    void employeeDeleted(int id);

private:
    QList<Employee> m_employees;
};

#endif // EMPLOYEECONTROLLER_H