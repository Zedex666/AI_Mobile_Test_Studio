#ifndef CORE_LOGGER_LOGGER_H
#define CORE_LOGGER_LOGGER_H

#include <QObject>
#include <QString>
#include <QDateTime>

namespace core {
namespace logger {

enum LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

class Logger : public QObject
{
    Q_OBJECT

public:
    static Logger* instance();

    void log(LogLevel level, const QString &category, const QString &message);
    void debug(const QString &category, const QString &message);
    void info(const QString &category, const QString &message);
    void warning(const QString &category, const QString &message);
    void error(const QString &category, const QString &message);
    void critical(const QString &category, const QString &message);

    void setLogFile(const QString &filePath);
    void setMinLevel(LogLevel level);

signals:
    void logEmitted(const QString &formattedMessage);

private:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();

    QString formatMessage(LogLevel level, const QString &category, const QString &message) const;
    void writeToFile(const QString &message);

    static Logger *s_instance;
    QString m_logFilePath;
    LogLevel m_minLevel = Debug;
};

} // namespace logger
} // namespace core

#endif // CORE_LOGGER_LOGGER_H
