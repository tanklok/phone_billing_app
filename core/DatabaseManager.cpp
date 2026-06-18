#include "databasemanager.h"
#include "config.h"
#include "logger.h"
#include <QSqlDriver>

DatabaseManager::~DatabaseManager()
{
    disconnect();
}

DatabaseManager& DatabaseManager::getInstance()
{
    static DatabaseManager instance;
    return instance;
}

void DatabaseManager::setupConnection()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        m_db = QSqlDatabase::database();
    } else {
        m_db = QSqlDatabase::addDatabase("QPSQL");
    }

    Config &config = Config::getInstance();

    m_db.setHostName(config.getDbHost());
    m_db.setPort(config.getDbPort());
    m_db.setDatabaseName(config.getDbName());
    m_db.setUserName(config.getDbUser());
    m_db.setPassword(config.getDbPassword());
}

bool DatabaseManager::connect()
{
    if (m_connected) return true;

    setupConnection();

    if (!m_db.open()) {
        m_lastError = m_db.lastError().text();
        Logger::getInstance().error("Failed to connect: " + m_lastError);
        return false;
    }

    m_connected = true;
    Logger::getInstance().info("Connected to database");
    return true;
}

void DatabaseManager::disconnect()
{
    if (m_connected) {
        m_db.close();
        m_connected = false;
        Logger::getInstance().info("Disconnected from database");
    }
}

QSqlQuery DatabaseManager::executeQuery(const QString &sql, const QVariantList &params)
{
    if (!m_connected && !connect()) {
        return QSqlQuery();
    }

    // Если параметров нет, просто выполняем запрос
    if (params.isEmpty()) {
        QSqlQuery query(m_db);
        if (!query.exec(sql)) {
            m_lastError = query.lastError().text();
            Logger::getInstance().error("Query failed: " + m_lastError + "\nSQL: " + sql);
            return QSqlQuery();
        }
        return query;
    }

    // Заменяем все вхождения "?" на $1, $2, ...
    QString preparedSql = sql;
    int paramIndex = 1;
    int pos = preparedSql.indexOf('?');
    while (pos != -1) {
        // replace(pos, len, replacement) - заменяет len символов начиная с pos
        preparedSql.replace(pos, 1, QString("$%1").arg(paramIndex));
        paramIndex++;
        pos = preparedSql.indexOf('?', pos + 1);
    }

    QSqlQuery query(m_db);
    query.prepare(preparedSql);

    // bindValue нумерация с 1
    for (int i = 0; i < params.size(); ++i) {
        query.bindValue(i + 1, params[i]);
    }

    if (!query.exec()) {
        m_lastError = query.lastError().text();
        Logger::getInstance().error("Query failed: " + m_lastError + "\nSQL: " + sql);
        return QSqlQuery();
    }

    return query;
}

bool DatabaseManager::executeNonQuery(const QString &sql, const QVariantList &params)
{
    QSqlQuery query = executeQuery(sql, params);
    return query.isActive();
}

bool DatabaseManager::beginTransaction()
{
    if (!m_connected && !connect()) return false;
    return m_db.transaction();
}

bool DatabaseManager::commitTransaction()
{
    return m_db.commit();
}

bool DatabaseManager::rollbackTransaction()
{
    return m_db.rollback();
}

qint64 DatabaseManager::lastInsertId() const
{
    QSqlQuery query(m_db);
    if (query.exec("SELECT lastval()") && query.next()) {
        return query.value(0).toLongLong();
    }
    return 0;
}
