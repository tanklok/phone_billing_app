#ifndef CALLCONTROLLER_H
#define CALLCONTROLLER_H

#include "basecontroller.h"
#include "../models/call.h"
#include "../models/calldetail.h"
#include <QDate>

class CallController : public BaseController
{
    Q_OBJECT
public:
    explicit CallController(QObject *parent = nullptr);

    bool createCall(const Call &call);
    bool deleteCall(qint64 id);

    // Возвращаем списки указателей
    QList<CallDetail*> getCallDetails(const QDate &dateFrom = QDate(),
                                       const QDate &dateTo = QDate(),
                                       qint64 abonentId = 0);

    double calculateCallCost(qint64 callerId, qint64 receiverId,
                             const QString &timeOfDay, int duration,
                             double &discount, double &totalCost);

    QList<Call*> getAllCalls();

signals:
    void callCreated(qint64 id);
    void callDeleted(qint64 id);
};

#endif // CALLCONTROLLER_H