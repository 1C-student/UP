#include "database.h"
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

Database& Database::getInstance()
{
    static Database instance;
    return instance;
}

bool Database::connect()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("party_work");
    db.setUserName("postgre");
    db.setPassword("postgres");

    if (!db.open()) {
        qDebug() << "DB error:" << db.lastError().text();
        return false;
    }
    return true;
}

void Database::disconnect()
{
    if (db.isOpen()) db.close();
}

QSqlQuery Database::execute(const QString& query, const QVariantList& params)
{
    QSqlQuery q(db);
    q.prepare(query);
    for (int i = 0; i < params.size(); ++i) q.bindValue(i, params[i]);
    q.exec();
    return q;
}

QString Database::hashPassword(const QString& password)
{
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex();
}
