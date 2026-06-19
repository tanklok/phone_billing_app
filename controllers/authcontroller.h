#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include "basecontroller.h"
#include "../models/user.h"
#include <QRegularExpression>
#include <QSharedPointer>

class AuthController : public BaseController
{
    Q_OBJECT
public:
    explicit AuthController(QObject *parent = nullptr);

    bool login(const QString &login, const QString &password, QSharedPointer<User> &user);
    bool registerUser(const QString &login, const QString &password,
                      const QString &fullName, const QString &roleName,
                      QSharedPointer<User> &user);
    bool logout();

    static bool isPasswordStrong(const QString &password);
    bool isAdmin(qint64 userId) const;

    QSharedPointer<User> getCurrentUser() const { return m_currentUser; }
    bool isAuthenticated() const { return m_isAuthenticated; }

    bool createUserByAdmin(const QString &login, const QString &password,
                       const QString &fullName, const QString &roleName,
                       QSharedPointer<User> &user);

signals:
    void loginSuccess(const QSharedPointer<User> &user);
    void loginFailed(const QString &reason);
    void registrationSuccess(const QSharedPointer<User> &user);
    void registrationFailed(const QString &reason);
    void logoutSuccess();

private:
    QSharedPointer<User> m_currentUser;
    bool m_isAuthenticated = false;

    static const QRegularExpression PASSWORD_REGEX;
};

#endif // AUTHCONTROLLER_H
