#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QDateTime>
#include <QObject>

class Employee : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString surname READ getSurname WRITE setSurname NOTIFY surnameChanged)
    Q_PROPERTY(QString job READ getJob WRITE setJob NOTIFY jobChanged)
    Q_PROPERTY(QString phone READ getPhone WRITE setPhone NOTIFY phoneChanged)
    Q_PROPERTY(bool available READ isAvailable WRITE setAvailable NOTIFY availableChanged)
    Q_PROPERTY(QDateTime createdAt READ getCreatedAt WRITE setCreatedAt NOTIFY createdAtChanged)
    Q_PROPERTY(QDateTime updatedAt READ getUpdatedAt WRITE setUpdatedAt NOTIFY updatedAtChanged)

public:
    explicit Employee(int id = 0, const QString &name = "", const QString &surname = "", const QString &job = "", const QString &phone = "", bool available = true, const QDateTime &createdAt = QDateTime::currentDateTime(), const QDateTime &updatedAt = QDateTime::currentDateTime());

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

signals:
    void idChanged();
    void nameChanged();
    void surnameChanged();
    void jobChanged();
    void phoneChanged();
    void availableChanged();
    void createdAtChanged();
    void updatedAtChanged();

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
