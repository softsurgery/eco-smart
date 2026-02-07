#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employee.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , employeeWindow(nullptr)
{
    ui->setupUi(this);
    connect(ui->redirect, &QPushButton::clicked, this, &MainWindow::onDashboardButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete employeeWindow;
}

void MainWindow::onDashboardButtonClicked()
{
    if (employeeWindow == nullptr) {
        employeeWindow = new Employee();
    }
    employeeWindow->show();
    this->close();
}
