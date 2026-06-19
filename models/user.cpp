#include "user.h"


QString User::hashPassword(const QString &password)
{
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString::fromUtf8(hash.toHex());
}

void User::setPassword(const QString &password)
{
    m_passwordHash = hashPassword(password);
}

bool User::checkPassword(const QString &password) const
{
    return m_passwordHash == hashPassword(password);
}

void User::fromQuery(const QSqlQuery &query)
{
    m_id = getValue<qint64>(query, "user_id");
    m_login = getValue<QString>(query, "login");
    m_passwordHash = getValue<QString>(query, "password_hash");
    m_fullName = getValue<QString>(query, "full_name");
}

QVariantMap User::toVariantMap() const
{
    QVariantMap map;
    map["user_id"] = m_id;
    map["login"] = m_login;
    map["password_hash"] = m_passwordHash;
    map["full_name"] = m_fullName;
    return map;
}
