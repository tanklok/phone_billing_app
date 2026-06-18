#ifndef CALLDETAIL_H
#define CALLDETAIL_H

#include "model.h"
#include <QDate>
#include <QString>

class CallDetail : public Model
{
    Q_OBJECT
public:
    explicit CallDetail(QObject *parent = nullptr);

    qint64 getCallId() const { return m_callId; }
    QDate getDate() const { return m_date; }
    int getDuration() const { return m_duration; }
    QString getTimeOfDay() const { return m_timeOfDay; }
    QString getSenderName() const { return m_senderName; }
    QString getReceiverName() const { return m_receiverName; }
    QString getFromCity() const { return m_fromCity; }
    QString getToCity() const { return m_toCity; }
    double getCostPerMinute() const { return m_costPerMinute; }
    double getDiscount() const { return m_discount; }
    double getTotalCost() const { return m_totalCost; }

    void setCallId(qint64 id) { m_callId = id; }
    void setDate(const QDate &date) { m_date = date; }
    void setDuration(int duration) { m_duration = duration; }
    void setTimeOfDay(const QString &time) { m_timeOfDay = time; }
    void setSenderName(const QString &name) { m_senderName = name; }
    void setReceiverName(const QString &name) { m_receiverName = name; }
    void setFromCity(const QString &city) { m_fromCity = city; }
    void setToCity(const QString &city) { m_toCity = city; }
    void setCostPerMinute(double cost) { m_costPerMinute = cost; }
    void setDiscount(double discount) { m_discount = discount; }
    void setTotalCost(double cost) { m_totalCost = cost; }

    qint64 getId() const override { return m_callId; }
    void setId(qint64 id) override { m_callId = id; }

    void fromQuery(const QSqlQuery &query) override;
    QVariantMap toVariantMap() const override;

private:
    qint64 m_callId = 0;
    QDate m_date;
    int m_duration = 0;
    QString m_timeOfDay;
    QString m_senderName;
    QString m_receiverName;
    QString m_fromCity;
    QString m_toCity;
    double m_costPerMinute = 0.0;
    double m_discount = 0.0;
    double m_totalCost = 0.0;
};

#endif // CALLDETAIL_H