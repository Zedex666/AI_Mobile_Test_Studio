#include "logger.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMutexLocker>
#include <QMutex>

namespace core {
namespace logger {

Logger* Logger::s_instance = nullptr;

Logger* Logger::instance()
{
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if (!s_instance) {
        s_instance = new Logger();
    }
    return s_instance;
}

Logger::Logger(QObject *parent)
    : QObject(parent)
{
}

Logger::~Logger()
{
}

void Logger::log(LogLevel level, const QString &category, const QString &message)
{
    if (level < m_minLevel) return;

    QString formatted = formatMessage(level, category, message);
    emit logEmitted(formatted);
    writeToFile(formatted);

    // Also output to Qt debug
    qDebug() << formatted;
}

void Logger::debug(const QString &category, const QString &message)
{
    log(Debug, category, message);
}

void Logger::info(const QString &category, const QString &message)
{
    log(Info, category, message);
}

void Logger::warning(const QString &category, const QString &message)
{
    log(Warning, category, message);
}

void Logger::error(const QString &category, const QString &message)
{
    log(Error, category, message);
}

void Logger::critical(const QString &category, const QString &message)
{
    log(Critical, category, message);
}

void Logger::setLogFile(const QString &filePath)
{
    m_logFilePath = filePath;
}

void Logger::setMinLevel(LogLevel level)
{
    m_minLevel = level;
}

QString Logger::formatMessage(LogLevel level, const QString &category, const QString &message) const
{
    static const QMap<LogLevel, QString> levelNames = {
        {Debug, "DEBUG"},
        {Info, "INFO"},
        {Warning, "WARN"},
        {Error, "ERROR"},
        {Critical, "CRIT"}
    };

    return QString("[%1] [%2] [%3] %4")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
        .arg(levelNames.value(level, "UNK"))
        .arg(category)
        .arg(message);
}

void Logger::writeToFile(const QString &message)
{
    if (m_logFilePath.isEmpty()) return;

    QFile file(m_logFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << message << "\n";
    }
}

} // namespace logger
} // namespace core
