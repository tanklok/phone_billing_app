#include "authcontroller.h"
#include "../core/databasemanager.h"

const QRegularExpression AuthController::PASSWORD_REGEX(
    "^(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,}$"
);

AuthController::AuthController(QObject *parent) : BaseController(parent) {}

bool AuthController::isPasswordStrong(const QString &password)
{
    return PASSWORD_REGEX.match(password).hasMatch();
}

bool AuthController::login(const QString &login, const QString &password, QSharedPointer<User> &user)
{
    QString sql = "SELECT * FROM Пользователи WHERE логин = ?";
    DatabaseManager &db = DatabaseManager::getInstance();
    QSqlQuery query = db.executeQuery(sql, {login});

    if (!query.next()) {
        emit loginFailed("Пользователь не найден");
        return false;
    }

    user = QSharedPointer<User>::create();
    user->fromQuery(query);

    if (!user->checkPassword(password)) {
        emit loginFailed("Неверный пароль");
        user.clear();
        return false;
    }

    m_currentUser = user;
    m_isAuthenticated = true;
    emit loginSuccess(user);
    return true;
}

bool AuthController::registerUser(const QString &login, const QString &password,
                                   const QString &fullName, const QString &roleName,
                                   QSharedPointer<User> &user)
{
    if (!isPasswordStrong(password)) {
        emit registrationFailed("Пароль должен содержать минимум 8 символов, "
                                "одну заглавную букву, одну цифру и один спецсимвол");
        return false;
    }

    DatabaseManager &db = DatabaseManager::getInstance();

    QString checkSql = "SELECT COUNT(*) FROM Пользователи WHERE логин = ?";
    QSqlQuery checkQuery = db.executeQuery(checkSql, {login});
    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        emit registrationFailed("Пользователь с таким логином уже существует");
        return false;
    }

    db.beginTransaction();

    user = QSharedPointer<User>::create();
    user->setLogin(login);
    user->setPassword(password);
    user->setFullName(fullName);

    QString insertSql = "INSERT INTO Пользователи (логин, пароль, ФИО) VALUES (?, ?, ?)";
    if (!db.executeNonQuery(insertSql, {user->getLogin(), user->getPasswordHash(), user->getFullName()})) {
        emit registrationFailed("Ошибка при создании пользователя");
        db.rollbackTransaction();
        return false;
    }

    qint64 userId = db.lastInsertId();

    QString roleSql = "SELECT id_роли FROM Роли WHERE название_роли = ?";
    QSqlQuery roleQuery = db.executeQuery(roleSql, {roleName});
    if (!roleQuery.next()) {
        emit registrationFailed("Роль не найдена");
        db.rollbackTransaction();
        return false;
    }
    qint64 roleId = roleQuery.value("id_роли").toLongLong();

    QString assignSql = "INSERT INTO Назначения_ролей (id_пользователя, id_роли) VALUES (?, ?)";
    if (!db.executeNonQuery(assignSql, {userId, roleId})) {
        emit registrationFailed("Ошибка при назначении роли");
        db.rollbackTransaction();
        return false;
    }

    db.commitTransaction();

    QString loadSql = "SELECT * FROM Пользователи WHERE id_пользователя = ?";
    QSqlQuery loadQuery = db.executeQuery(loadSql, {userId});
    if (loadQuery.next()) {
        user->fromQuery(loadQuery);
        emit registrationSuccess(user);
        return true;
    }

    emit registrationFailed("Ошибка загрузки данных");
    return false;
}

bool AuthController::logout()
{
    if (m_isAuthenticated) {
        m_isAuthenticated = false;
        m_currentUser.clear();
        emit logoutSuccess();
        return true;
    }
    return false;
}

bool AuthController::isAdmin(qint64 userId) const
{
    QString sql = R"(
        SELECT COUNT(*) FROM Роли р
        JOIN Назначения_ролей нр ON р.id_роли = нр.id_роли
        WHERE нр.id_пользователя = ? AND р.название_роли = 'администратор'
    )";

    DatabaseManager &db = DatabaseManager::getInstance();
    QSqlQuery query = db.executeQuery(sql, {userId});

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}
