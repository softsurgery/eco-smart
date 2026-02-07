# Guidelines for Building Uniform CRUDs in Qt6 and C++

This document outlines the rules and strategies for building Create, Read, Update, and Delete (CRUD) functionalities in a uniform manner using Qt6 and C++. Following these guidelines will ensure consistency, maintainability, and scalability across all CRUD implementations in the project.

---

## General Principles

1. **Consistency**: All CRUD implementations must follow the same structure and naming conventions.
2. **Reusability**: Maximize code reuse by creating reusable components and utility functions.
3. **Separation of Concerns**: Follow the Model-View-Controller (MVC) design pattern to separate business logic, UI, and data handling.
4. **Error Handling**: Implement robust error handling and user feedback mechanisms.
5. **Scalability**: Design CRUD operations to be easily extendable for future requirements.

---

## Naming Conventions

1. **Classes**: Use PascalCase for class names (e.g., `Employee`, `Product`).
2. **Methods**: Use camelCase for method names (e.g., `createEmployee`, `updateEmployee`).
3. **Variables**: Use camelCase for variables (e.g., `employeeName`, `productId`).
4. **UI Elements**: Use descriptive names with prefixes (e.g., `btnAddEmployee`, `lblEmployeeName`).
5. **File Names**: Match file names with class names (e.g., `Employee.h`, `Employee.cpp`).

---

## CRUD Implementation Strategy

### 1. **Model**

- Each entity should have a corresponding model class.
- The model class should encapsulate all data and provide getter and setter methods.
- Use `Q_PROPERTY` for properties that need to be used in Qt's meta-object system.

#### Example:

```cpp
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>

class Employee {
    Q_OBJECT
    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

public:
    explicit Employee(int id = 0, const QString &name = "");

    int getId() const;
    void setId(int id);

    QString getName() const;
    void setName(const QString &name);

signals:
    void idChanged();
    void nameChanged();

private:
    int m_id;
    QString m_name;
};

#endif // EMPLOYEE_H
```

### 2. **View**

- Use Qt Designer to create `.ui` files for the user interface.
- Follow the naming conventions for UI elements.

#### Example:

- `EmployeeView.ui` should contain input fields for employee details (e.g., ID, Name) and buttons for CRUD operations (e.g., Add, Edit, Delete).

### 3. **Controller**

- Each entity should have a controller class to handle business logic and interact with the model and view.
- Use signals and slots to handle user interactions.
- Implement the following methods for each CRUD operation:
  - `create<EntityName>()`
  - `read<EntityName>()`
  - `update<EntityName>()`
  - `delete<EntityName>()`

#### Example:

```cpp
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
```

### 4. **Database Integration**

- Use `Oracle11g` for database connections.
- Use parameterized queries.
- Create a utility class for database operations to avoid redundant code.

#### Example:

```cpp
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool DatabaseUtils::executeQuery(const QString &queryStr, const QVariantList &params) {
    QSqlQuery query;
    query.prepare(queryStr);

    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    if (!query.exec()) {
        qDebug() << "Database error:" << query.lastError().text();
        return false;
    }

    return true;
}
```

## Rules for Code Review

1. Ensure adherence to the folder and file structure.
2. Verify that naming conventions are followed.
3. Check for proper use of the MVC pattern.
4. Confirm that all CRUD operations are implemented and tested.
5. Ensure that database queries are parameterized and secure.
6. Verify that the UI is user-friendly and consistent with other CRUDs.
7. Ensure that error handling and user feedback are implemented.

---

By following these guidelines, all CRUD implementations in the project will maintain a consistent structure and quality, making the codebase easier to understand, maintain, and extend.
