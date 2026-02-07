#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class EmployeeWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDashboardButtonClicked();

private:
    Ui::MainWindow *ui;
    EmployeeWindow *employeeWindow;
};
#endif // MAINWINDOW_H
