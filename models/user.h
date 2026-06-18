#ifndef USER_H
#define USER_H

#include "model.h"
#include <QString>
#include <QCryptographicHash>

class User : public Model
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr) : Model(parent) {}

    qint64 getId() const override { return m_id; }
    void setId(qint64 id) override { m_id = id; }

    QString getLogin() const { return m_login; }
    void setLogin(const QString &login) { m_login = login; }

    QString getPasswordHash() const { return m_passwordHash; }
    void setPasswordHash(const QString &hash) { m_passwordHash = hash; }

    QString getFullName() const { return m_fullName; }
    void setFullName(const QString &name) { m_fullName = name; }

    void setPassword(const QString &password);
    bool checkPassword(const QString &password) const;
    static QString hashPassword(const QString &password);

    void fromQuery(const QSqlQuery &query) override;
    QVariantMap toVariantMap() const override;

private:
    qint64 m_id = 0;
    QString m_login;
    QString m_passwordHash;
    QString m_fullName;
};

#endif // USER_H
