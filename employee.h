#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QDateTime>

class Employee
{
public:
    explicit Employee(int id = 0, const QString &name = "", const QString &surname = "", const QString &job = "", const QString &phone = "", bool available = true, const QDateTime &createdAt = QDateTime::currentDateTime(), const QDateTime &updatedAt = QDateTime::currentDateTime());
    
    // Copy constructor and assignment operator
    Employee(const Employee &other) = default;
    Employee& operator=(const Employee &other) = default;

    int getId() const;
    void setId(int id);

    QString getName() const;
    void setName(const QString &name);

    QString getSurname() const;
    void setSurname(const QString &surname);

    QString getJob() const;
    void setJob(const QString &job);

    QString getPhone() const;
    void setPhone(const QString &phone);

    bool isAvailable() const;
    void setAvailable(bool available);

    QDateTime getCreatedAt() const;
    void setCreatedAt(const QDateTime &createdAt);

    QDateTime getUpdatedAt() const;
    void setUpdatedAt(const QDateTime &updatedAt);

private:
    int m_id;
    QString m_name;
    QString m_surname;
    QString m_job;
    QString m_phone;
    bool m_available;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
};

#endif // EMPLOYEE_H
