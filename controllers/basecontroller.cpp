#include "basecontroller.h"

BaseController::BaseController(QObject *parent) 
    : QObject(parent) 
{
}

bool BaseController::executeNonQuery(const QString &sql, const QVariantList &params)
{
    DatabaseManager &db = DatabaseManager::getInstance();
    return db.executeNonQuery(sql, params);
}