#ifndef CALL_H
#define CALL_H

#include "model.h"
#include <QDate>
#include <QString>

class Call : public Model
{
    Q_OBJECT
public:
    explicit Call(QObject *parent = nullptr);

    qint64 getId() const override { return m_id; }
    void setId(qint64 id) override { m_id = id; }

    QDate getDate() const { return m_date; }
    void setDate(const QDate &date) { m_date = date; }

    int getDuration() const { return m_duration; }
    void setDuration(int duration) { m_duration = duration; }

    QString getTimeOfDay() const { return m_timeOfDay; }
    void setTimeOfDay(const QString &time) { m_timeOfDay = time; }

    qint64 getCallerId() const { return m_callerId; }
    void setCallerId(qint64 id) { m_callerId = id; }

    qint64 getReceiverId() const { return m_receiverId; }
    void setReceiverId(qint64 id) { m_receiverId = id; }

    void fromQuery(const QSqlQuery &query) override;
    QVariantMap toVariantMap() const override;

private:
    qint64 m_id = 0;
    QDate m_date;
    int m_duration = 0;
    QString m_timeOfDay;
    qint64 m_callerId = 0;
    qint64 m_receiverId = 0;
};

#endif // CALL_H