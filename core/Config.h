#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

class Config
{
public:
    static Config& getInstance();

    QString getDbHost() const;
    int getDbPort() const;
    QString getDbName() const;
    QString getDbUser() const;
    QString getDbPassword() const;

    bool loadFromFile(const QString &filePath = "config.ini");

private:
    Config() = default;
    ~Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    QString m_dbHost = "localhost";
    int m_dbPort = 5432;
    QString m_dbName = "phone_billing";
    QString m_dbUser = "postgres";
    QString m_dbPassword = "tankt3486";
};

#endif // CONFIG_H
