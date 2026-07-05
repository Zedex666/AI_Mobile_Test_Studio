#ifndef SERVICES_SCRIPT_SERVICE_H
#define SERVICES_SCRIPT_SERVICE_H

#include <QObject>
#include <QString>
#include <QProcess>

namespace services {

class ScriptService : public QObject
{
    Q_OBJECT

public:
    explicit ScriptService(QObject *parent = nullptr);

    bool runScript(const QString &scriptPath, const QStringList &args = {});
    bool runPythonCode(const QString &code);
    bool stop();
    bool isRunning() const;

signals:
    void started();
    void finished(int exitCode);
    void output(const QString &text);
    void error(const QString &text);

private slots:
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onFinished(int exitCode);

private:
    QProcess *m_process = nullptr;
};

} // namespace services

#endif // SERVICES_SCRIPT_SERVICE_H
