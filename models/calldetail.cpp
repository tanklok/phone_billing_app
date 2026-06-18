#include "calldetail.h"

CallDetail::CallDetail(QObject *parent) : Model(parent) {}

void CallDetail::fromQuery(const QSqlQuery &query)
{
    m_callId = getValue<qint64>(query, "id_звонка");
    m_date = getValue<QDate>(query, "дата");
    m_duration = getValue<int>(query, "длительность");
    m_timeOfDay = getValue<QString>(query, "время_суток");
    m_senderName = getValue<QString>(query, "отправитель");
    m_receiverName = getValue<QString>(query, "получатель");
    m_fromCity = getValue<QString>(query, "город_отправления");
    m_toCity = getValue<QString>(query, "город_назначения");
    m_costPerMinute = getValue<double>(query, "стоимость_минуты");
    m_discount = getValue<double>(query, "размер_скидки");
    m_totalCost = getValue<double>(query, "итоговая_стоимость");
}

QVariantMap CallDetail::toVariantMap() const
{
    QVariantMap map;
    map["id_звонка"] = m_callId;
    map["дата"] = m_date;
    map["длительность"] = m_duration;
    map["время_суток"] = m_timeOfDay;
    map["отправитель"] = m_senderName;
    map["получатель"] = m_receiverName;
    map["город_отправления"] = m_fromCity;
    map["город_назначения"] = m_toCity;
    map["стоимость_минуты"] = m_costPerMinute;
    map["размер_скидки"] = m_discount;
    map["итоговая_стоимость"] = m_totalCost;
    return map;
}