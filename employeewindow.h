#ifndef EMPLOYEEWINDOW_H
#define EMPLOYEEWINDOW_H

#include <QWidget>
#include "employee.h"

namespace Ui {
class Employee;
}

class EmployeeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeeWindow(QWidget *parent = nullptr);
    ~EmployeeWindow();

private:
    Ui::Employee *ui;
};

#endif // EMPLOYEEWINDOW_H
