#include "script_service.h"
#include <QDebug>

namespace services {

ScriptService::ScriptService(QObject *parent)
    : QObject(parent)
{
    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &ScriptService::onReadyReadStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &ScriptService::onReadyReadStandardError);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &ScriptService::onFinished);
}

bool ScriptService::runScript(const QString &scriptPath, const QStringList &args)
{
    if (isRunning()) return false;

    QStringList arguments;
    arguments << scriptPath;
    arguments << args;

    m_process->start("python", arguments);
    emit started();
    return m_process->waitForStarted(5000);
}

bool ScriptService::runPythonCode(const QString &code)
{
    if (isRunning()) return false;

    m_process->start("python", QStringList() << "-c" << code);
    emit started();
    return m_process->waitForStarted(5000);
}

bool ScriptService::stop()
{
    if (!isRunning()) return false;
    m_process->kill();
    return m_process->waitForFinished(3000);
}

bool ScriptService::isRunning() const
{
    return m_process && m_process->state() == QProcess::Running;
}

void ScriptService::onReadyReadStandardOutput()
{
    QString text = QString::fromLocal8Bit(m_process->readAllStandardOutput());
    emit output(text);
}

void ScriptService::onReadyReadStandardError()
{
    QString text = QString::fromLocal8Bit(m_process->readAllStandardError());
    emit error(text);
}

void ScriptService::onFinished(int exitCode)
{
    emit finished(exitCode);
}

} // namespace services
