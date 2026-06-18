#include "config.h"
#include <QSettings>
#include <QFileInfo>
#include <QDebug>

Config& Config::getInstance()
{
    static Config instance;
    return instance;
}

bool Config::loadFromFile(const QString &filePath)
{
    QSettings settings(filePath, QSettings::IniFormat);

    if (!QFileInfo::exists(filePath)) {
        qDebug() << "Config file not found, using defaults";
        return false;
    }

    m_dbHost = settings.value("Database/Host", "localhost").toString();
    m_dbPort = settings.value("Database/Port", 5432).toInt();
    m_dbName = settings.value("Database/Name", "phone_billing").toString();
    m_dbUser = settings.value("Database/User", "postgres").toString();
    m_dbPassword = settings.value("Database/Password", "postgres").toString();

    return true;
}

QString Config::getDbHost() const { return m_dbHost; }
int Config::getDbPort() const { return m_dbPort; }
QString Config::getDbName() const { return m_dbName; }
QString Config::getDbUser() const { return m_dbUser; }
QString Config::getDbPassword() const { return m_dbPassword; }
