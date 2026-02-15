#ifndef EMPLOYEEWINDOW_H
#define EMPLOYEEWINDOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDateTime>
#include "employee.h"
#include "employeecontroller.h"

namespace Ui {
class Employee;
}

class EmployeeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeeWindow(QWidget *parent = nullptr);
    ~EmployeeWindow();

private slots:
    void onAddEmployeeClicked();
    void onUpdateEmployeeClicked();
    void onDeleteEmployeeClicked();
    void onClearEmployeeClicked();
    void onEmployeeTableSelectionChanged();
    void onEmployeeCreated(const Employee &employee);
    void onEmployeeUpdated(const Employee &employee);
    void onEmployeeDeleted(int id);

private:
    void setupUI();
    void setupTableModel();
    void loadEmployees();
    void clearForm();
    void populateForm(const Employee &employee);
    Employee getEmployeeFromForm() const;
    bool validateForm() const;
    void refreshTable();

    Ui::Employee *ui;
    EmployeeController *m_controller;
    QStandardItemModel *m_tableModel;
    int m_selectedEmployeeId;
};

#endif // EMPLOYEEWINDOW_H
