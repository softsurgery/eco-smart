#include "employeewindow.h"
#include "ui_employee.h"

EmployeeWindow::EmployeeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Employee)
{
    ui->setupUi(this);
}

EmployeeWindow::~EmployeeWindow()
{
    delete ui;
}
