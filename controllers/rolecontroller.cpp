#include "rolecontroller.h"
#include "../core/databasemanager.h"

RoleController::RoleController(QObject *parent) : BaseController(parent) {}

QList<Role*> RoleController::getAllRoles()
{
    QString sql = "SELECT * FROM roles ORDER BY role_name";
    return executeQueryAndMap<Role>(sql);
}

bool RoleController::roleExists(const QString &roleName)
{
    QString sql = "SELECT COUNT(*) FROM roles WHERE role_name = $1";
    DatabaseManager &db = DatabaseManager::getInstance();
    QSqlQuery query = db.executeQuery(sql, {roleName});
    if (query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}
