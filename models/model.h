#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVariant>
#include <QSqlQuery>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~Model() = default;

    virtual qint64 getId() const = 0;
    virtual void setId(qint64 id) = 0;

    virtual void fromQuery(const QSqlQuery &query) = 0;
    virtual QVariantMap toVariantMap() const = 0;

protected:
    template<typename T>
    static T getValue(const QSqlQuery &query, const QString &fieldName, const T &defaultValue = T())
    {
        QVariant value = query.value(fieldName);
        if (!value.isValid() || value.isNull()) {
            return defaultValue;
        }
        return value.value<T>();
    }
};

#endif // MODEL_H