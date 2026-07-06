#include "scrcpycontroller.h"
#include "core/config/configmanager.h"
#include "core/logger/logger.h"
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
    bool exists = QFile::exists(m_scrcpyPath);
    core::logger::Logger::instance()->info("ScrcpyController",
        tr("isAvailable: path=%1 exists=%2").arg(m_scrcpyPath).arg(exists));
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

bool ScrcpyController::start(int maxSize, int maxFps)
{
    core::logger::Logger::instance()->info("ScrcpyController",
        tr("start() called, running=%1").arg(isRunning()));
    if (isRunning()) {
        stop();
    }

    if (!isAvailable()) {
        emit error(tr("Scrcpy not found at: %1").arg(m_scrcpyPath));
        return false;
    }

    // Clear accumulated stderr from any previous run
    m_stderrBuffer.clear();

    // Build scrcpy command-line arguments with MAXIMUM cross-version compatibility.
    // Only include options that are universally supported across scrcpy 1.x, 2.x, 3.x,
    // and any repackaged "v4.0" builds. Non-universal options cause scrcpy to print
    // "Unknown option" and exit immediately.
    //
    // Intentionally OMITTED for cross-version safety:
    //   --render-expired-frames  → removed in scrcpy 3.0+ (always enabled by default)
    //   --window-borderless      → removed in scrcpy 3.0+ (we strip borders in embedNativeWindow)
    //   --bit-rate <N>           → renamed to --video-bit-rate in scrcpy 3.0+
    //                             (let scrcpy use its default bitrate for max compatibility)
    QStringList args;
    if (!m_deviceId.isEmpty()) {
        args << "--serial" << m_deviceId;
    }
    args << "--max-size" << QString::number(maxSize);
    args << "--max-fps" << QString::number(maxFps);
    args << "--no-control";
    args << "--window-title" << "AMTS_Scrcpy_" + m_deviceId;

    // Detailed per-argument logging for diagnostics
    core::logger::Logger::instance()->info("ScrcpyController",
        tr("scrcpy binary: %1").arg(m_scrcpyPath));
    core::logger::Logger::instance()->info("ScrcpyController",
        tr("scrcpy arg count: %1").arg(args.size()));
    for (int i = 0; i < args.size(); ++i) {
        core::logger::Logger::instance()->info("ScrcpyController",
            tr("scrcpy arg[%1]: \"%2\"").arg(i).arg(args[i]));
    }

    m_process->start(m_scrcpyPath, args);

    if (!m_process->waitForStarted(5000)) {
        core::logger::Logger::instance()->error("ScrcpyController", tr("waitForStarted FAILED"));
        emit error(tr("Failed to start scrcpy"));
        return false;
    }

    core::logger::Logger::instance()->info("ScrcpyController", tr("waitForStarted SUCCESS"));
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
    // Read any remaining stderr output before logging exit
    QString remainingErr = QString::fromUtf8(m_process->readAllStandardError()).trimmed();
    if (!remainingErr.isEmpty()) {
        m_stderrBuffer += remainingErr + "\n";
    }

    core::logger::Logger::instance()->info("ScrcpyController",
        tr("scrcpy process exited: code=%1 status=%2").arg(exitCode)
            .arg(status == QProcess::NormalExit ? "normal" : "crash"));

    // On non-zero exit, emit the full accumulated stderr so the caller
    // can diagnose compatibility issues (e.g. "Unknown option" errors)
    if (exitCode != 0) {
        QString fullStderr = m_stderrBuffer.trimmed();
        if (fullStderr.isEmpty()) {
            fullStderr = tr("(no stderr output captured)");
        }
        QString errorMsg = tr("scrcpy exited with code %1. Full stderr:\n%2")
                               .arg(exitCode)
                               .arg(fullStderr);
        core::logger::Logger::instance()->error("ScrcpyController", errorMsg);
        emit error(errorMsg);
    }

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
    QString text = QString::fromUtf8(m_process->readAllStandardOutput()).trimmed();
    if (!text.isEmpty()) {
        core::logger::Logger::instance()->info("ScrcpyController",
            tr("scrcpy stdout: %1").arg(text));
        emit logOutput(text);
    }
}

void ScrcpyController::onReadyReadStandardError()
{
    QString text = QString::fromUtf8(m_process->readAllStandardError()).trimmed();
    if (!text.isEmpty()) {
        // Accumulate all stderr for error reporting on non-zero exit
        m_stderrBuffer += text + "\n";

        // scrcpy writes info to stderr by default — treat as info, not warning,
        // but flag errors explicitly so they stand out in the log
        if (text.contains("ERROR", Qt::CaseInsensitive)
            || text.contains("error:", Qt::CaseInsensitive)
            || text.contains("Unknown option", Qt::CaseInsensitive)) {
            core::logger::Logger::instance()->error("ScrcpyController",
                tr("scrcpy stderr: %1").arg(text));
        } else {
            core::logger::Logger::instance()->info("ScrcpyController",
                tr("scrcpy stderr: %1").arg(text));
        }
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
    if (!m_containerHwnd) {
        core::logger::Logger::instance()->warning("ScrcpyController", "embedNativeWindow: no container HWND");
        return false;
    }

    // Find scrcpy window by title pattern
    QString expectedTitle = "AMTS_Scrcpy_" + m_deviceId;
    HWND hwnd = FindWindowW(nullptr, reinterpret_cast<const wchar_t*>(expectedTitle.utf16()));
    core::logger::Logger::instance()->info("ScrcpyController",
        tr("Looking for window title='%1' hwnd=%2").arg(expectedTitle).arg(reinterpret_cast<quintptr>(hwnd)));

    if (!hwnd) {
        // Try without device suffix (some scrcpy versions may trim or modify the title)
        hwnd = FindWindowW(nullptr, L"scrcpy");
        core::logger::Logger::instance()->info("ScrcpyController",
            tr("Fallback title 'scrcpy' hwnd=%1").arg(reinterpret_cast<quintptr>(hwnd)));
    }
    if (!hwnd) {
        // Try to find by SDL app class name (scrcpy uses SDL2 internally)
        hwnd = FindWindowW(L"SDL_app", nullptr);
        core::logger::Logger::instance()->info("ScrcpyController",
            tr("Fallback class 'SDL_app' hwnd=%1").arg(reinterpret_cast<quintptr>(hwnd)));
    }
    if (!hwnd) {
        // Enumerate top-level windows — search for ANY window containing "scrcpy"
        // (case-insensitive). This handles all scrcpy version variations.
        EnumWindows([](HWND h, LPARAM lParam) -> BOOL {
            auto *outHwnd = reinterpret_cast<HWND*>(lParam);
            wchar_t buf[256];
            if (GetWindowTextW(h, buf, 256) > 0) {
                wchar_t lower[256];
                for (int i = 0; buf[i] && i < 255; i++) {
                    lower[i] = towlower(buf[i]);
                    lower[i + 1] = L'\0';
                }
                if (wcsstr(lower, L"scrcpy") != nullptr) {
                    *outHwnd = h;
                    core::logger::Logger::instance()->info("ScrcpyController",
                        tr("EnumWindows (title) found scrcpy window: title='%1' hwnd=%2")
                            .arg(QString::fromWCharArray(buf))
                            .arg(reinterpret_cast<quintptr>(h)));
                    return FALSE;
                }
            }
            return TRUE;
        }, reinterpret_cast<LPARAM>(&hwnd));
    }
    if (!hwnd) {
        // Most reliable fallback: find the window by process ID.
        // This works regardless of window title, class name, or scrcpy version.
        DWORD scrcpyPid = static_cast<DWORD>(m_process->processId());
        if (scrcpyPid != 0) {
            struct PidSearchData {
                DWORD pid;
                HWND *outHwnd;
            } pidData = { scrcpyPid, &hwnd };
            EnumWindows([](HWND h, LPARAM lParam) -> BOOL {
                auto *data = reinterpret_cast<PidSearchData*>(lParam);
                DWORD pid = 0;
                GetWindowThreadProcessId(h, &pid);
                if (pid == data->pid && IsWindowVisible(h)) {
                    *data->outHwnd = h;
                    wchar_t buf[256];
                    GetWindowTextW(h, buf, 256);
                    core::logger::Logger::instance()->info("ScrcpyController",
                        tr("EnumWindows (PID) found scrcpy: pid=%1 title='%2' hwnd=%3")
                            .arg(data->pid)
                            .arg(QString::fromWCharArray(buf))
                            .arg(reinterpret_cast<quintptr>(h)));
                    return FALSE;
                }
                return TRUE;
            }, reinterpret_cast<LPARAM>(&pidData));
        }
    }

    if (!hwnd) return false;

    m_scrcpyHwnd = reinterpret_cast<quintptr>(hwnd);
    core::logger::Logger::instance()->info("ScrcpyController", tr("Found scrcpy window, embedding..."));

    // Set parent to our container
    SetParent(hwnd, (HWND)m_containerHwnd);

    // Remove borders and caption from the scrcpy window
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~(WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
    style |= WS_CHILD | WS_CLIPSIBLINGS;
    SetWindowLong(hwnd, GWL_STYLE, style);

    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    exStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE | WS_EX_WINDOWEDGE);
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);

    // Ensure the parent widget clips its painting around the embedded child window.
    // Without WS_CLIPCHILDREN, Qt may paint over the native scrcpy window.
    LONG parentStyle = GetWindowLong((HWND)m_containerHwnd, GWL_STYLE);
    parentStyle |= WS_CLIPCHILDREN;
    SetWindowLong((HWND)m_containerHwnd, GWL_STYLE, parentStyle);

    updateNativeGeometry();
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    emit embedded();
    return true;
}

void ScrcpyController::updateNativeGeometry()
{
    if (!m_scrcpyHwnd || !m_containerHwnd) return;

    HWND hwnd = (HWND)m_scrcpyHwnd;
    RECT rect;
    GetClientRect((HWND)m_containerHwnd, &rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    SetWindowPos(hwnd, HWND_TOP, 0, 0, w, h,
                 SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    updateWindowRgn(w, h);
}

void ScrcpyController::updateWindowRgn(int w, int h)
{
    if (!m_scrcpyHwnd) return;
    HWND hwnd = (HWND)m_scrcpyHwnd;
    int radius = m_embedCornerRadius;
    if (radius > 0 && w > radius * 2 && h > radius * 2) {
        HRGN rgn = CreateRoundRectRgn(0, 0, w, h, radius * 2, radius * 2);
        SetWindowRgn(hwnd, rgn, TRUE);
    } else {
        SetWindowRgn(hwnd, nullptr, TRUE);
    }
}
#endif

} // namespace scrcpy
} // namespace core
