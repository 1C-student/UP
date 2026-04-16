#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

class Database
{
public:
    static Database& getInstance();
    bool connect();
    void disconnect();
    QSqlQuery execute(const QString& query, const QVariantList& params = {});
    QString hashPassword(const QString& password);

private:
    Database() = default;
    ~Database() = default;
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    QSqlDatabase db;
};

#endif
