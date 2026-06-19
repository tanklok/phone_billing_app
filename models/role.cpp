#include "role.h"

Role::Role(QObject *parent) : Model(parent) {}

void Role::fromQuery(const QSqlQuery &query)
{
    m_id = getValue<qint64>(query, "role_id");
    m_name = getValue<QString>(query, "role_name");
}

QVariantMap Role::toVariantMap() const
{
    QVariantMap map;
    map["role_id"] = m_id;
    map["role_name"] = m_name;
    return map;
}