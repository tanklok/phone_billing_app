#include "call.h"

Call::Call(QObject *parent) : Model(parent) {}

void Call::fromQuery(const QSqlQuery &query)
{
    m_id = getValue<qint64>(query, "id_звонка");
    m_date = getValue<QDate>(query, "дата");
    m_duration = getValue<int>(query, "длительность");
    m_timeOfDay = getValue<QString>(query, "время_суток");
    m_callerId = getValue<qint64>(query, "id_абонента_отправитель");
    m_receiverId = getValue<qint64>(query, "id_абонента_получатель");
}

QVariantMap Call::toVariantMap() const
{
    QVariantMap map;
    map["id_звонка"] = m_id;
    map["дата"] = m_date;
    map["длительность"] = m_duration;
    map["время_суток"] = m_timeOfDay;
    map["id_абонента_отправитель"] = m_callerId;
    map["id_абонента_получатель"] = m_receiverId;
    return map;
}