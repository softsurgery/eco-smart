#ifndef DATABASEUTILS_H
#define DATABASEUTILS_H

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariantList>

class DatabaseUtils {
public:
    static bool executeQuery(const QString &queryStr, const QVariantList &params = {});
};

#endif // DATABASEUTILS_H