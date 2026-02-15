#include "employeewindow.h"
#include "ui_employee.h"
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDateTime>
#include <QPushButton>
#include <QItemSelectionModel>
#include <QModelIndexList>

EmployeeWindow::EmployeeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Employee)
    , m_controller(new EmployeeController(this))
    , m_tableModel(new QStandardItemModel(this))
    , m_selectedEmployeeId(-1)
{
    ui->setupUi(this);
    setupUI();
    setupTableModel();
    loadEmployees();
}

EmployeeWindow::~EmployeeWindow()
{
    delete ui;
}

void EmployeeWindow::setupUI()
{
    // Connect button signals to slots
    connect(ui->btnAddEmployee, &QPushButton::clicked, this, &EmployeeWindow::onAddEmployeeClicked);
    connect(ui->btnUpdateEmployee, &QPushButton::clicked, this, &EmployeeWindow::onUpdateEmployeeClicked);
    connect(ui->btnDeleteEmployee, &QPushButton::clicked, this, &EmployeeWindow::onDeleteEmployeeClicked);
    connect(ui->btnClearEmployee, &QPushButton::clicked, this, &EmployeeWindow::onClearEmployeeClicked);
    
    // Connect table selection
    connect(ui->tvEmployees->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &EmployeeWindow::onEmployeeTableSelectionChanged);
    
    // Connect controller signals
    connect(m_controller, &EmployeeController::employeeCreated, this, &EmployeeWindow::onEmployeeCreated);
    connect(m_controller, &EmployeeController::employeeUpdated, this, &EmployeeWindow::onEmployeeUpdated);
    connect(m_controller, &EmployeeController::employeeDeleted, this, &EmployeeWindow::onEmployeeDeleted);
    
    // Set initial button states
    ui->btnUpdateEmployee->setEnabled(false);
    ui->btnDeleteEmployee->setEnabled(false);
    
    // Make ID field read-only (auto-generated)
    ui->leEmployeeId->setReadOnly(true);
    
    // Set default availability to true
    ui->chkEmployeeAvailable->setChecked(true);
}

void EmployeeWindow::setupTableModel()
{
    // Set up table headers
    m_tableModel->setHorizontalHeaderLabels({
        "ID", "Name", "Surname", "Job", "Phone", "Available", "Created At", "Updated At"
    });
    
    ui->tvEmployees->setModel(m_tableModel);
    
    // Set column widths
    ui->tvEmployees->setColumnWidth(0, 50);  // ID
    ui->tvEmployees->setColumnWidth(1, 100); // Name  
    ui->tvEmployees->setColumnWidth(2, 100); // Surname
    ui->tvEmployees->setColumnWidth(3, 150); // Job
    ui->tvEmployees->setColumnWidth(4, 120); // Phone
    ui->tvEmployees->setColumnWidth(5, 80);  // Available
    ui->tvEmployees->setColumnWidth(6, 150); // Created At
    ui->tvEmployees->setColumnWidth(7, 150); // Updated At
}

void EmployeeWindow::loadEmployees()
{
    QList<Employee> employees = m_controller->readEmployees();
    
    m_tableModel->clear();
    m_tableModel->setHorizontalHeaderLabels({
        "ID", "Name", "Surname", "Job", "Phone", "Available", "Created At", "Updated At"
    });
    
    for (const Employee &employee : employees) {
        QList<QStandardItem*> items;
        items << new QStandardItem(QString::number(employee.getId()));
        items << new QStandardItem(employee.getName());
        items << new QStandardItem(employee.getSurname());
        items << new QStandardItem(employee.getJob());
        items << new QStandardItem(employee.getPhone());
        items << new QStandardItem(employee.isAvailable() ? "Yes" : "No");
        items << new QStandardItem(employee.getCreatedAt().toString("yyyy-MM-dd hh:mm:ss"));
        items << new QStandardItem(employee.getUpdatedAt().toString("yyyy-MM-dd hh:mm:ss"));
        
        m_tableModel->appendRow(items);
    }
}

void EmployeeWindow::onAddEmployeeClicked()
{
    if (!validateForm()) {
        return;
    }
    
    Employee employee = getEmployeeFromForm();
    m_controller->createEmployee(employee);
}

void EmployeeWindow::onUpdateEmployeeClicked()
{
    if (m_selectedEmployeeId == -1) {
        QMessageBox::warning(this, "Update Employee", "Please select an employee to update.");
        return;
    }
    
    if (!validateForm()) {
        return;
    }
    
    Employee employee = getEmployeeFromForm();
    employee.setId(m_selectedEmployeeId);
    m_controller->updateEmployee(employee);
}

void EmployeeWindow::onDeleteEmployeeClicked()
{
    if (m_selectedEmployeeId == -1) {
        QMessageBox::warning(this, "Delete Employee", "Please select an employee to delete.");
        return;
    }
    
    int result = QMessageBox::question(this, "Delete Employee", 
                                     "Are you sure you want to delete this employee?",
                                     QMessageBox::Yes | QMessageBox::No);
    
    if (result == QMessageBox::Yes) {
        m_controller->deleteEmployee(m_selectedEmployeeId);
    }
}

void EmployeeWindow::onClearEmployeeClicked()
{
    clearForm();
}

void EmployeeWindow::onEmployeeTableSelectionChanged()
{
    QModelIndexList selection = ui->tvEmployees->selectionModel()->selectedRows();
    
    if (selection.isEmpty()) {
        ui->btnUpdateEmployee->setEnabled(false);
        ui->btnDeleteEmployee->setEnabled(false);
        m_selectedEmployeeId = -1;
        return;
    }
    
    // Get selected row data
    int row = selection.first().row();
    QStandardItem *idItem = m_tableModel->item(row, 0);
    
    if (idItem) {
        m_selectedEmployeeId = idItem->text().toInt();
        
        // Populate form with selected employee data
        ui->leEmployeeId->setText(m_tableModel->item(row, 0)->text());
        ui->leEmployeeName->setText(m_tableModel->item(row, 1)->text());
        ui->leEmployeeSurname->setText(m_tableModel->item(row, 2)->text());
        ui->leEmployeeJob->setText(m_tableModel->item(row, 3)->text());
        ui->leEmployeePhone->setText(m_tableModel->item(row, 4)->text());
        ui->chkEmployeeAvailable->setChecked(m_tableModel->item(row, 5)->text() == "Yes");
        
        // Set datetime fields
        QString createdAtStr = m_tableModel->item(row, 6)->text();
        QString updatedAtStr = m_tableModel->item(row, 7)->text();
        ui->dtEmployeeCreatedAt->setDateTime(QDateTime::fromString(createdAtStr, "yyyy-MM-dd hh:mm:ss"));
        ui->dtEmployeeUpdatedAt->setDateTime(QDateTime::fromString(updatedAtStr, "yyyy-MM-dd hh:mm:ss"));
        
        ui->btnUpdateEmployee->setEnabled(true);
        ui->btnDeleteEmployee->setEnabled(true);
    }
}

void EmployeeWindow::onEmployeeCreated(const Employee &employee)
{
    Q_UNUSED(employee)
    loadEmployees(); // Refresh table
    clearForm();
    QMessageBox::information(this, "Success", "Employee created successfully!");
}

void EmployeeWindow::onEmployeeUpdated(const Employee &employee)
{
    Q_UNUSED(employee)
    loadEmployees(); // Refresh table
    clearForm();
    QMessageBox::information(this, "Success", "Employee updated successfully!");
}

void EmployeeWindow::onEmployeeDeleted(int id)
{
    Q_UNUSED(id)
    loadEmployees(); // Refresh table
    clearForm();
    QMessageBox::information(this, "Success", "Employee deleted successfully!");
}

void EmployeeWindow::clearForm()
{
    ui->leEmployeeId->clear();
    ui->leEmployeeName->clear();
    ui->leEmployeeSurname->clear();
    ui->leEmployeeJob->clear();
    ui->leEmployeePhone->clear();
    ui->chkEmployeeAvailable->setChecked(true);
    ui->dtEmployeeCreatedAt->setDateTime(QDateTime::currentDateTime());
    ui->dtEmployeeUpdatedAt->setDateTime(QDateTime::currentDateTime());
    
    m_selectedEmployeeId = -1;
    ui->btnUpdateEmployee->setEnabled(false);
    ui->btnDeleteEmployee->setEnabled(false);
}

Employee EmployeeWindow::getEmployeeFromForm() const
{
    Employee employee;
    employee.setName(ui->leEmployeeName->text());
    employee.setSurname(ui->leEmployeeSurname->text());
    employee.setJob(ui->leEmployeeJob->text());
    employee.setPhone(ui->leEmployeePhone->text());
    employee.setAvailable(ui->chkEmployeeAvailable->isChecked());
    return employee;
}

bool EmployeeWindow::validateForm() const
{
    if (ui->leEmployeeName->text().trimmed().isEmpty()) {
        QMessageBox::warning(const_cast<EmployeeWindow*>(this), "Validation Error", 
                           "Name field cannot be empty.");
        return false;
    }
    
    if (ui->leEmployeeSurname->text().trimmed().isEmpty()) {
        QMessageBox::warning(const_cast<EmployeeWindow*>(this), "Validation Error", 
                           "Surname field cannot be empty.");
        return false;
    }
    
    return true;
}
