#include "call.h"

Call::Call(QObject *parent) : Model(parent) {}

void Call::fromQuery(const QSqlQuery &query)
{
    m_id = getValue<qint64>(query, "call_id");
    m_date = getValue<QDate>(query, "call_date");
    m_duration = getValue<int>(query, "duration_minutes");
    m_timeOfDay = getValue<QString>(query, "time_of_day");
    m_callerId = getValue<qint64>(query, "sender_subscriber_id");
    m_receiverId = getValue<qint64>(query, "receiver_subscriber_id");
}

QVariantMap Call::toVariantMap() const
{
    QVariantMap map;
    map["call_id"] = m_id;
    map["call_date"] = m_date;
    map["duration_minutes"] = m_duration;
    map["time_of_day"] = m_timeOfDay;
    map["sender_subscriber_id"] = m_callerId;
    map["receiver_subscriber_id"] = m_receiverId;
    return map;
}
