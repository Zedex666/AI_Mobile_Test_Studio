#ifndef SERVICES_AI_SERVICE_H
#define SERVICES_AI_SERVICE_H

#include <QObject>
#include <QString>
#include <QJsonObject>

namespace services {

class AIService : public QObject
{
    Q_OBJECT

public:
    explicit AIService(QObject *parent = nullptr);

    void generateScript(const QString &requirement);
    void analyzeScreen(const QString &imagePath);
    void fixScript(const QString &script, const QString &error);
    void generateReport(const QJsonObject &testResult);

signals:
    void scriptGenerated(const QString &script);
    void screenAnalyzed(const QString &description);
    void scriptFixed(const QString &fixedScript);
    void reportGenerated(const QString &reportPath);
    void error(const QString &error);
    void progress(const QString &step, const QString &status);
};

} // namespace services

#endif // SERVICES_AI_SERVICE_H
