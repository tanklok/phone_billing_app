#ifndef BASECONTROLLER_H
#define BASECONTROLLER_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QSqlQuery>
#include "../core/databasemanager.h"

class BaseController : public QObject
{
    Q_OBJECT
public:
    explicit BaseController(QObject *parent = nullptr);

protected:
    // Возвращаем список указателей (чтобы избежать копирования)
    template<typename T>
    QList<T*> executeQueryAndMap(const QString &sql, const QVariantList &params = {})
    {
        QList<T*> result;
        DatabaseManager &db = DatabaseManager::getInstance();
        QSqlQuery query = db.executeQuery(sql, params);

        while (query.next()) {
            T *model = new T();
            model->fromQuery(query);
            result.append(model);
        }

        return result;
    }

    // Возвращаем указатель
    template<typename T>
    T* executeQueryAndGetOne(const QString &sql, const QVariantList &params = {})
    {
        DatabaseManager &db = DatabaseManager::getInstance();
        QSqlQuery query = db.executeQuery(sql, params);

        if (query.next()) {
            T *model = new T();
            model->fromQuery(query);
            return model;
        }

        return nullptr;
    }

    bool executeNonQuery(const QString &sql, const QVariantList &params = {});

signals:
    void errorOccurred(const QString &error);
};

#endif // BASECONTROLLER_H