#include "callcontroller.h"
#include "../core/databasemanager.h"

CallController::CallController(QObject *parent) : BaseController(parent) {}

bool CallController::createCall(const Call &call)
{
    DatabaseManager &db = DatabaseManager::getInstance();

    QString sql = "INSERT INTO Звонки "
                  "(дата, длительность, время_суток, "
                  "id_абонента_отправитель, id_абонента_получатель) "
                  "VALUES (?, ?, ?, ?, ?)";

    bool success = db.executeNonQuery(sql, {
        call.getDate(),
        call.getDuration(),
        call.getTimeOfDay(),
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
    QString sql = "DELETE FROM Звонки WHERE id_звонка = ?";
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
    QString sql = R"(
        SELECT
            з.id_звонка,
            з.дата,
            з.длительность,
            з.время_суток,
            а1.название AS отправитель,
            а2.название AS получатель,
            г1.название_города AS город_отправления,
            г2.название_города AS город_назначения,
            т.стоимость_минуты,
            COALESCE(п.размер_скидки, 0) AS размер_скидки,
            (з.длительность * т.стоимость_минуты *
             (100 - COALESCE(п.размер_скидки, 0)) / 100) AS итоговая_стоимость
        FROM Звонки з
        JOIN Абоненты а1 ON з.id_абонента_отправитель = а1.id_абонента
        JOIN Города г1 ON а1.id_города = г1.id_города
        JOIN Абоненты а2 ON з.id_абонента_получатель = а2.id_абонента
        JOIN Города г2 ON а2.id_города = г2.id_города
        JOIN Тарифы т ON г1.id_города = т.id_города_отправления
                     AND г2.id_города = т.id_города_назначения
                     AND з.время_суток = т.время_суток
        LEFT JOIN Правила_скидок п ON з.id_правила_скидки = п.id_правила
        WHERE 1=1
    )";

    QVariantList params;

    if (dateFrom.isValid()) {
        sql += " AND з.дата >= ?";
        params << dateFrom;
    }

    if (dateTo.isValid()) {
        sql += " AND з.дата <= ?";
        params << dateTo;
    }

    if (abonentId > 0) {
        sql += " AND (а1.id_абонента = ? OR а2.id_абонента = ?)";
        params << abonentId << abonentId;
    }

    sql += " ORDER BY з.дата DESC, з.id_звонка DESC";

    return executeQueryAndMap<CallDetail>(sql, params);
}

QList<Call*> CallController::getAllCalls()
{
    QString sql = "SELECT * FROM Звонки ORDER BY дата DESC, id_звонка DESC";
    return executeQueryAndMap<Call>(sql);
}

double CallController::calculateCallCost(qint64 callerId, qint64 receiverId,
                                          const QString &timeOfDay, int duration,
                                          double &discount, double &totalCost)
{
    DatabaseManager &db = DatabaseManager::getInstance();

    QString citySql = R"(
        SELECT г1.id_города AS from_city, г2.id_города AS to_city
        FROM Абоненты а1
        JOIN Города г1 ON а1.id_города = г1.id_города
        CROSS JOIN Абоненты а2
        JOIN Города г2 ON а2.id_города = г2.id_города
        WHERE а1.id_абонента = ? AND а2.id_абонента = ?
    )";

    QSqlQuery cityQuery = db.executeQuery(citySql, {callerId, receiverId});

    if (!cityQuery.next()) {
        emit errorOccurred("Не удалось определить города абонентов");
        return -1;
    }

    qint64 fromCity = cityQuery.value("from_city").toLongLong();
    qint64 toCity = cityQuery.value("to_city").toLongLong();

    QString tariffSql = "SELECT стоимость_минуты FROM Тарифы "
                        "WHERE id_города_отправления = ? AND id_города_назначения = ? "
                        "AND время_суток = ?";

    QSqlQuery tariffQuery = db.executeQuery(tariffSql, {fromCity, toCity, timeOfDay});

    if (!tariffQuery.next()) {
        emit errorOccurred("Тариф не найден для данного направления и времени");
        return -1;
    }

    double costPerMinute = tariffQuery.value("стоимость_минуты").toDouble();
    double baseCost = duration * costPerMinute;

    QString discountSql = "SELECT размер_скидки FROM Правила_скидок "
                          "WHERE id_города_назначения = ? AND ? BETWEEN длительность_от AND длительность_до";

    QSqlQuery discountQuery = db.executeQuery(discountSql, {toCity, duration});

    discount = 0;
    if (discountQuery.next()) {
        discount = discountQuery.value("размер_скидки").toDouble();
    }

    totalCost = baseCost * (100 - discount) / 100;
    return totalCost;
}