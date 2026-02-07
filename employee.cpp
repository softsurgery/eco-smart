#include "employee.h"

Employee::Employee(int id, const QString &name, const QString &surname, const QString &job, const QString &phone, bool available, const QDateTime &createdAt, const QDateTime &updatedAt)
    : m_id(id), m_name(name), m_surname(surname), m_job(job), m_phone(phone), m_available(available), m_createdAt(createdAt), m_updatedAt(updatedAt) {}

int Employee::getId() const {
    return m_id;
}

void Employee::setId(int id) {
    if (m_id != id) {
        m_id = id;
        emit idChanged();
    }
}

QString Employee::getName() const {
    return m_name;
}

void Employee::setName(const QString &name) {
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

QString Employee::getSurname() const {
    return m_surname;
}

void Employee::setSurname(const QString &surname) {
    if (m_surname != surname) {
        m_surname = surname;
        emit surnameChanged();
    }
}

QString Employee::getJob() const {
    return m_job;
}

void Employee::setJob(const QString &job) {
    if (m_job != job) {
        m_job = job;
        emit jobChanged();
    }
}

QString Employee::getPhone() const {
    return m_phone;
}

void Employee::setPhone(const QString &phone) {
    if (m_phone != phone) {
        m_phone = phone;
        emit phoneChanged();
    }
}

bool Employee::isAvailable() const {
    return m_available;
}

void Employee::setAvailable(bool available) {
    if (m_available != available) {
        m_available = available;
        emit availableChanged();
    }
}

QDateTime Employee::getCreatedAt() const {
    return m_createdAt;
}

void Employee::setCreatedAt(const QDateTime &createdAt) {
    if (m_createdAt != createdAt) {
        m_createdAt = createdAt;
        emit createdAtChanged();
    }
}

QDateTime Employee::getUpdatedAt() const {
    return m_updatedAt;
}

void Employee::setUpdatedAt(const QDateTime &updatedAt) {
    if (m_updatedAt != updatedAt) {
        m_updatedAt = updatedAt;
        emit updatedAtChanged();
    }
}
