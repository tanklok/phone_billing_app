#include "calldetail.h"

CallDetail::CallDetail(QObject *parent) : Model(parent) {}

void CallDetail::fromQuery(const QSqlQuery &query)
{
    m_callId = getValue<qint64>(query, "call_id");
    m_date = getValue<QDate>(query, "call_date");
    m_duration = getValue<int>(query, "duration_minutes");
    m_timeOfDay = getValue<QString>(query, "time_of_day");
    m_senderName = getValue<QString>(query, "sender_name");
    m_receiverName = getValue<QString>(query, "receiver_name");
    m_fromCity = getValue<QString>(query, "origin_city");
    m_toCity = getValue<QString>(query, "destination_city");
    m_costPerMinute = getValue<double>(query, "price_per_minute");
    m_discount = getValue<double>(query, "discount_percent");
    m_totalCost = getValue<double>(query, "total_cost");
}

QVariantMap CallDetail::toVariantMap() const
{
    QVariantMap map;
    map["call_id"] = m_callId;
    map["call_date"] = m_date;
    map["duration_minutes"] = m_duration;
    map["time_of_day"] = m_timeOfDay;
    map["sender_name"] = m_senderName;
    map["receiver_name"] = m_receiverName;
    map["origin_city"] = m_fromCity;
    map["destination_city"] = m_toCity;
    map["price_per_minute"] = m_costPerMinute;
    map["discount_percent"] = m_discount;
    map["total_cost"] = m_totalCost;
    return map;
}
