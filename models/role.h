#ifndef ROLE_H
#define ROLE_H

#include "model.h"
#include <QString>

class Role : public Model
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ getId WRITE setId)
    Q_PROPERTY(QString name READ getName WRITE setName)

public:
    explicit Role(QObject *parent = nullptr);

    qint64 getId() const override { return m_id; }
    void setId(qint64 id) override { m_id = id; }

    QString getName() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    void fromQuery(const QSqlQuery &query) override;
    QVariantMap toVariantMap() const override;

private:
    qint64 m_id = 0;
    QString m_name;
};

#endif // ROLE_H