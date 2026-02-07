#include "databaseutils.h"

bool DatabaseUtils::executeQuery(const QString &queryStr, const QVariantList &params) {
    QSqlQuery query;
    query.prepare(queryStr);

    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i, params[i]);
    }

    if (!query.exec()) {
        qDebug() << "Database error:" << query.lastError().text();
        return false;
    }

    return true;
}