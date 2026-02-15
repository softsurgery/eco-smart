#include "employee.h"

Employee::Employee(int id, const QString &name, const QString &surname, const QString &job, const QString &phone, bool available, const QDateTime &createdAt, const QDateTime &updatedAt)
    : m_id(id), m_name(name), m_surname(surname), m_job(job), m_phone(phone), m_available(available), m_createdAt(createdAt), m_updatedAt(updatedAt) {}

int Employee::getId() const {
    return m_id;
}

void Employee::setId(int id) {
    m_id = id;
}

QString Employee::getName() const {
    return m_name;
}

void Employee::setName(const QString &name) {
    m_name = name;
}

QString Employee::getSurname() const {
    return m_surname;
}

void Employee::setSurname(const QString &surname) {
    m_surname = surname;
}

QString Employee::getJob() const {
    return m_job;
}

void Employee::setJob(const QString &job) {
    m_job = job;
}

QString Employee::getPhone() const {
    return m_phone;
}

void Employee::setPhone(const QString &phone) {
    m_phone = phone;
}

bool Employee::isAvailable() const {
    return m_available;
}

void Employee::setAvailable(bool available) {
    m_available = available;
}

QDateTime Employee::getCreatedAt() const {
    return m_createdAt;
}

void Employee::setCreatedAt(const QDateTime &createdAt) {
    m_createdAt = createdAt;
}

QDateTime Employee::getUpdatedAt() const {
    return m_updatedAt;
}

void Employee::setUpdatedAt(const QDateTime &updatedAt) {
    m_updatedAt = updatedAt;
}
