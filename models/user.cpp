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
    m_id = getValue<qint64>(query, "id_пользователя");
    m_login = getValue<QString>(query, "логин");
    m_passwordHash = getValue<QString>(query, "пароль");
    m_fullName = getValue<QString>(query, "ФИО");
}

QVariantMap User::toVariantMap() const
{
    QVariantMap map;
    map["id_пользователя"] = m_id;
    map["логин"] = m_login;
    map["пароль"] = m_passwordHash;
    map["ФИО"] = m_fullName;
    return map;
}
