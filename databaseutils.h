#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>
#include <QVariantList>
#include <QMessageBox>

class DatabaseUtils {
public:
    static bool executeQuery(const QString &queryStr, const QVariantList &params = {});
};

#endif // DATABASEUTILS_H