#ifndef ROLECONTROLLER_H
#define ROLECONTROLLER_H

#include "basecontroller.h"
#include "../models/role.h"

class RoleController : public BaseController
{
    Q_OBJECT
public:
    explicit RoleController(QObject *parent = nullptr);

    QList<Role*> getAllRoles();
    bool roleExists(const QString &roleName);
};

#endif // ROLECONTROLLER_H
