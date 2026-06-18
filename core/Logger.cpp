#include "logger.h"
#include <QDateTime>
#include <QDebug>
#include <QDir>

Logger::~Logger()
{
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}

Logger& Logger::getInstance()
{
    static Logger instance;
    instance.init();
    return instance;
}

void Logger::init()
{
    if (m_initialized) return;

    QString logPath = "logs/app.log";
    QFileInfo fileInfo(logPath);
    QDir dir = fileInfo.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    m_logFile.setFileName(logPath);
    if (m_logFile.open(QIODevice::Append | QIODevice::Text)) {
        m_logStream.setDevice(&m_logFile);
        m_initialized = true;
        info("Logger initialized");
    } else {
        qDebug() << "Failed to open log file:" << logPath;
        m_initialized = true;
    }
}

void Logger::info(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString formatted = QString("[%1] [INFO] %2").arg(timestamp, message);

    m_mutex.lock();
    if (m_initialized && m_logFile.isOpen()) {
        m_logStream << formatted << "\n" << Qt::flush;
    }
    m_mutex.unlock();

    qDebug() << formatted;
}

void Logger::warning(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString formatted = QString("[%1] [WARN] %2").arg(timestamp, message);

    m_mutex.lock();
    if (m_initialized && m_logFile.isOpen()) {
        m_logStream << formatted << "\n" << Qt::flush;
    }
    m_mutex.unlock();

    qDebug() << formatted;
}

void Logger::error(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString formatted = QString("[%1] [ERROR] %2").arg(timestamp, message);

    m_mutex.lock();
    if (m_initialized && m_logFile.isOpen()) {
        m_logStream << formatted << "\n" << Qt::flush;
    }
    m_mutex.unlock();

    qDebug() << formatted;
}
