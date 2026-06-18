#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMutex>

class Logger
{
public:
    static Logger& getInstance();

    void info(const QString &message);
    void warning(const QString &message);
    void error(const QString &message);

private:
    Logger() = default;
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void init();

    QFile m_logFile;
    QTextStream m_logStream;
    QMutex m_mutex;
    bool m_initialized = false;
};

#endif // LOGGER_H
