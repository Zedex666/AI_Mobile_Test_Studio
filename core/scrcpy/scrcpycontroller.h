#ifndef CORE_SCRCPY_SCRCPYCONTROLLER_H
#define CORE_SCRCPY_SCRCPYCONTROLLER_H

#include <QObject>
#include <QProcess>
#include <QTimer>

namespace core {
namespace scrcpy {

class ScrcpyController : public QObject
{
    Q_OBJECT

public:
    explicit ScrcpyController(QObject *parent = nullptr);
    ~ScrcpyController();

    bool isAvailable() const;
    bool isRunning() const;

    void setScrcpyPath(const QString &path);
    void setDevice(const QString &deviceId);

    bool start(int maxSize = 1080, int maxFps = 60);
    void stop();

    // Window embedding (Windows only)
    void setEmbedContainer(QWidget *container);
    void updateEmbedGeometry();

signals:
    void started();
    void embedded();
    void stopped();
    void error(const QString &message);
    void logOutput(const QString &text);

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);
    void onProcessError(QProcess::ProcessError procError);
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void tryEmbedWindow();

private:
    QString m_scrcpyPath;
    QString m_deviceId;
    QProcess *m_process = nullptr;
    QWidget *m_embedContainer = nullptr;
    QTimer *m_embedTimer = nullptr;
    bool m_isRunning = false;
    QString m_stderrBuffer;   // accumulates all stderr for error reporting
    int m_embedCornerRadius = 22;

#ifdef Q_OS_WIN
    quintptr m_scrcpyHwnd = 0;
    quintptr m_containerHwnd = 0;
    bool embedNativeWindow();
    void updateNativeGeometry();
    void updateWindowRgn(int w, int h);
#endif
};

} // namespace scrcpy
} // namespace core

#endif // CORE_SCRCPY_SCRCPYCONTROLLER_H
