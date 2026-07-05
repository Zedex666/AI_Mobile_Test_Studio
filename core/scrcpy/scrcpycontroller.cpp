#include "scrcpycontroller.h"
#include "core/config/configmanager.h"
#include <QDebug>
#include <QDir>
#include <QWidget>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace core {
namespace scrcpy {

ScrcpyController::ScrcpyController(QObject *parent)
    : QObject(parent)
    , m_process(new QProcess(this))
    , m_embedTimer(new QTimer(this))
{
    m_scrcpyPath = core::config::ConfigManager::instance()->scrcpyPath();

    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &ScrcpyController::onProcessFinished);
    connect(m_process, &QProcess::errorOccurred,
            this, &ScrcpyController::onProcessError);
    connect(m_process, &QProcess::readyReadStandardOutput,
            this, &ScrcpyController::onReadyReadStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError,
            this, &ScrcpyController::onReadyReadStandardError);

    m_embedTimer->setSingleShot(false);
    m_embedTimer->setInterval(500);
    connect(m_embedTimer, &QTimer::timeout, this, &ScrcpyController::tryEmbedWindow);
}

ScrcpyController::~ScrcpyController()
{
    stop();
}

bool ScrcpyController::isAvailable() const
{
    if (m_scrcpyPath.isEmpty()) return false;
    QFileInfo info(m_scrcpyPath);
    return info.exists() && info.isExecutable();
}

bool ScrcpyController::isRunning() const
{
    return m_isRunning && m_process->state() == QProcess::Running;
}

void ScrcpyController::setScrcpyPath(const QString &path)
{
    m_scrcpyPath = path;
}

void ScrcpyController::setDevice(const QString &deviceId)
{
    m_deviceId = deviceId;
}

bool ScrcpyController::start(int maxSize, int bitRate, int maxFps)
{
    if (isRunning()) {
        stop();
    }

    if (!isAvailable()) {
        emit error(tr("Scrcpy not found at: %1").arg(m_scrcpyPath));
        return false;
    }

    QStringList args;
    if (!m_deviceId.isEmpty()) {
        args << "--serial" << m_deviceId;
    }
    args << "--max-size" << QString::number(maxSize);
    args << "--bit-rate" << QString::number(bitRate);
    args << "--max-fps" << QString::number(maxFps);
    args << "--no-control";
    args << "--render-expired-frames";
    args << "--window-title" << "AMTS_Scrcpy_" + m_deviceId;

    if (m_embedContainer) {
        // Borderless for embedding
        args << "--window-borderless";
    }

    qDebug() << "Starting scrcpy:" << m_scrcpyPath << args;
    m_process->start(m_scrcpyPath, args);

    if (!m_process->waitForStarted(5000)) {
        emit error(tr("Failed to start scrcpy"));
        return false;
    }

    m_isRunning = true;
    emit started();

    if (m_embedContainer) {
        m_embedTimer->start(10);
    }

    return true;
}

void ScrcpyController::stop()
{
    m_embedTimer->stop();

#ifdef Q_OS_WIN
    if (m_scrcpyHwnd) {
        // Restore window style before closing
        SetWindowLongPtr((HWND)m_scrcpyHwnd, GWL_STYLE,
            WS_VISIBLE | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU);
        m_scrcpyHwnd = 0;
    }
#endif

    if (m_process->state() == QProcess::Running) {
        m_process->terminate();
        if (!m_process->waitForFinished(3000)) {
            m_process->kill();
        }
    }
    m_isRunning = false;
    emit stopped();
}

void ScrcpyController::setEmbedContainer(QWidget *container)
{
    m_embedContainer = container;
#ifdef Q_OS_WIN
    if (container) {
        m_containerHwnd = reinterpret_cast<quintptr>(container->winId());
    } else {
        m_containerHwnd = 0;
    }
#endif
}

void ScrcpyController::updateEmbedGeometry()
{
#ifdef Q_OS_WIN
    if (m_scrcpyHwnd && m_embedContainer) {
        updateNativeGeometry();
    }
#endif
}

void ScrcpyController::onProcessFinished(int exitCode, QProcess::ExitStatus status)
{
    Q_UNUSED(exitCode)
    Q_UNUSED(status)
    m_isRunning = false;
    m_embedTimer->stop();
#ifdef Q_OS_WIN
    m_scrcpyHwnd = 0;
#endif
    emit stopped();
}

void ScrcpyController::onProcessError(QProcess::ProcessError procError)
{
    Q_UNUSED(procError)
    emit error(tr("Scrcpy process error: %1").arg(m_process->errorString()));
}

void ScrcpyController::onReadyReadStandardOutput()
{
    QString text = QString::fromUtf8(m_process->readAllStandardOutput());
    if (!text.isEmpty()) {
        emit logOutput(text);
    }
}

void ScrcpyController::onReadyReadStandardError()
{
    QString text = QString::fromUtf8(m_process->readAllStandardError());
    if (!text.isEmpty()) {
        emit logOutput(text);
    }
}

void ScrcpyController::tryEmbedWindow()
{
#ifdef Q_OS_WIN
    if (!m_embedContainer || !m_isRunning) {
        m_embedTimer->stop();
        return;
    }

    if (m_scrcpyHwnd == 0) {
        if (embedNativeWindow()) {
            m_embedTimer->stop();
        }
    } else {
        m_embedTimer->stop();
    }
#else
    m_embedTimer->stop();
#endif
}

#ifdef Q_OS_WIN
bool ScrcpyController::embedNativeWindow()
{
    if (!m_containerHwnd) return false;

    // Find scrcpy window by title pattern
    QString title = QString("AMTS_Scrcpy_" + m_deviceId).toUtf8();
    HWND hwnd = FindWindowW(nullptr, reinterpret_cast<const wchar_t*>(
        QString("AMTS_Scrcpy_" + m_deviceId).utf16()));

    if (!hwnd) {
        // Try without device suffix
        hwnd = FindWindowW(nullptr, L"scrcpy");
    }

    if (!hwnd) return false;

    m_scrcpyHwnd = reinterpret_cast<quintptr>(hwnd);

    // Set parent to our container
    SetParent(hwnd, (HWND)m_containerHwnd);

    // Remove borders and caption
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~(WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
    style |= WS_CHILD | WS_CLIPCHILDREN;
    SetWindowLong(hwnd, GWL_STYLE, style);

    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    exStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_WINDOWEDGE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);

    updateNativeGeometry();
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return true;
}

void ScrcpyController::updateNativeGeometry()
{
    if (!m_scrcpyHwnd || !m_containerHwnd) return;

    HWND hwnd = (HWND)m_scrcpyHwnd;
    RECT rect;
    GetClientRect((HWND)m_containerHwnd, &rect);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
                 SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}
#endif

} // namespace scrcpy
} // namespace core
