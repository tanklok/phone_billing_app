#include "callcontroller.h"
#include "../core/databasemanager.h"
#include "../core/logger.h"

CallController::CallController(QObject *parent) : BaseController(parent) {}

bool CallController::createCall(const Call &call)
{
    DatabaseManager &db = DatabaseManager::getInstance();

    // Преобразуем 'день'/'ночь' в 'DAY'/'NIGHT'
    QString timeOfDay = (call.getTimeOfDay() == "день") ? "DAY" : "NIGHT";

    QString sql = "INSERT INTO calls (call_date, duration_minutes, time_of_day, sender_subscriber_id, receiver_subscriber_id) "
                  "VALUES ($1, $2, $3, $4, $5)";

    bool success = db.executeNonQuery(sql, {
        call.getDate(),
        call.getDuration(),
        timeOfDay,
        call.getCallerId(),
        call.getReceiverId()
    });

    if (success) {
        emit callCreated(db.lastInsertId());
        return true;
    }

    emit errorOccurred("Ошибка при создании звонка");
    return false;
}

bool CallController::deleteCall(qint64 id)
{
    QString sql = "DELETE FROM calls WHERE call_id = $1";
    DatabaseManager &db = DatabaseManager::getInstance();

    if (db.executeNonQuery(sql, {id})) {
        emit callDeleted(id);
        return true;
    }

    emit errorOccurred("Ошибка при удалении звонка");
    return false;
}

QList<CallDetail*> CallController::getCallDetails(const QDate &dateFrom,
                                                   const QDate &dateTo,
                                                   qint64 abonentId)
{
    Q_UNUSED(dateFrom);
        Q_UNUSED(dateTo);
        Q_UNUSED(abonentId);

        QString sql = "SELECT * FROM calls";
        QVariantList params;
        return executeQueryAndMap<CallDetail>(sql, params);
}

QList<Call*> CallController::getAllCalls()
{
    QString sql = "SELECT * FROM calls ORDER BY call_date DESC, call_id DESC";
    return executeQueryAndMap<Call>(sql);
}

double CallController::calculateCallCost(qint64 callerId, qint64 receiverId,
                                          const QString &timeOfDay, int duration,
                                          double &discount, double &totalCost)
{
    DatabaseManager &db = DatabaseManager::getInstance();

    // Преобразуем время суток
    QString tod = (timeOfDay == "день") ? "DAY" : "NIGHT";

    QString citySql = R"(
        SELECT ct1.city_id AS origin_city, ct2.city_id AS destination_city
        FROM subscribers s1
        JOIN cities ct1 ON s1.city_id = ct1.city_id
        CROSS JOIN subscribers s2
        JOIN cities ct2 ON s2.city_id = ct2.city_id
        WHERE s1.subscriber_id = $1 AND s2.subscriber_id = $2
    )";

    QSqlQuery cityQuery = db.executeQuery(citySql, {callerId, receiverId});

    if (!cityQuery.next()) {
        emit errorOccurred("Не удалось определить города абонентов");
        return -1;
    }

    qint64 originCity = cityQuery.value("origin_city").toLongLong();
    qint64 destinationCity = cityQuery.value("destination_city").toLongLong();

    QString tariffSql = "SELECT price_per_minute FROM tariffs "
                        "WHERE origin_city_id = $1 AND destination_city_id = $2 "
                        "AND time_of_day = $3";

    QSqlQuery tariffQuery = db.executeQuery(tariffSql, {originCity, destinationCity, tod});

    if (!tariffQuery.next()) {
        emit errorOccurred("Тариф не найден для данного направления и времени");
        return -1;
    }

    double costPerMinute = tariffQuery.value("price_per_minute").toDouble();
    double baseCost = duration * costPerMinute;

    QString discountSql = "SELECT discount_percent FROM discount_rules "
                          "WHERE destination_city_id = $1 AND $2 BETWEEN duration_from AND duration_to";

    QSqlQuery discountQuery = db.executeQuery(discountSql, {destinationCity, duration});

    discount = 0;
    if (discountQuery.next()) {
        discount = discountQuery.value("discount_percent").toDouble();
    }

    totalCost = baseCost * (100 - discount) / 100;
    return totalCost;
}
