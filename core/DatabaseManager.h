#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QList>
#include <QString>

class DatabaseManager
{
public:
    static DatabaseManager& getInstance();

    bool connect();
    void disconnect();
    bool isConnected() const { return m_connected; }
    QString getLastError() const { return m_lastError; }

    QSqlQuery executeQuery(const QString &sql, const QVariantList &params = {});
    bool executeNonQuery(const QString &sql, const QVariantList &params = {});

    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    qint64 lastInsertId() const;

private:
    DatabaseManager() = default;
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
    bool m_connected = false;
    QString m_lastError;

    void setupConnection();
};

#endif // DATABASEMANAGER_H