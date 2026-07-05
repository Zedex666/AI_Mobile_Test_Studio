#include "adbcontroller.h"
#include "core/config/configmanager.h"
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>

namespace core {
namespace adb {

AdbController::AdbController(QObject *parent)
    : QObject(parent)
{
}

bool AdbController::isAvailable() const
{
    QString path = adbPath();
    if (path.isEmpty()) return false;

    QProcess process;
    process.start(path, QStringList() << "version");
    process.waitForFinished(3000);
    return process.exitCode() == 0;
}

QString AdbController::adbPath() const
{
    return core::config::ConfigManager::instance()->adbPath();
}

QStringList AdbController::devices() const
{
    QString path = adbPath();
    if (path.isEmpty()) return QStringList();

    QProcess process;
    process.start(path, QStringList() << "devices");
    process.waitForFinished(5000);

    QStringList result;
    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    QStringList lines = output.split('\n');
    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty() || trimmed.startsWith("List of")) continue;
        if (trimmed.contains('\t')) {
            result << trimmed.split('\t').first().trimmed();
        }
    }
    return result;
}

QMap<QString, DeviceDetail> AdbController::deviceDetails() const
{
    QMap<QString, DeviceDetail> result;
    QStringList ids = devices();
    for (const QString &id : ids) {
        result.insert(id, getDeviceDetail(id));
    }
    return result;
}

DeviceDetail AdbController::getDeviceDetail(const QString &deviceId) const
{
    DeviceDetail detail;
    detail.id = deviceId;
    detail.isEmulator = deviceId.startsWith("emulator-");

    QString path = adbPath();
    if (path.isEmpty()) return detail;

    // Get device state
    QProcess stateProc;
    stateProc.start(path, QStringList() << "-s" << deviceId << "get-state");
    stateProc.waitForFinished(3000);
    detail.status = QString::fromLocal8Bit(stateProc.readAllStandardOutput()).trimmed();
    if (detail.status.isEmpty()) detail.status = "unknown";

    // Get model
    QProcess modelProc;
    modelProc.start(path, QStringList() << "-s" << deviceId << "shell" << "getprop" << "ro.product.model");
    modelProc.waitForFinished(3000);
    detail.model = QString::fromLocal8Bit(modelProc.readAllStandardOutput()).trimmed();

    // Get device name (market name)
    QProcess nameProc;
    nameProc.start(path, QStringList() << "-s" << deviceId << "shell" << "getprop" << "ro.product.name");
    nameProc.waitForFinished(3000);
    detail.deviceName = QString::fromLocal8Bit(nameProc.readAllStandardOutput()).trimmed();

    if (detail.model.isEmpty()) {
        detail.model = detail.deviceName.isEmpty() ? deviceId : detail.deviceName;
    }

    // Get Android version
    QProcess verProc;
    verProc.start(path, QStringList() << "-s" << deviceId << "shell" << "getprop" << "ro.build.version.release");
    verProc.waitForFinished(3000);
    detail.androidVersion = QString::fromLocal8Bit(verProc.readAllStandardOutput()).trimmed();

    // Get SDK version
    QProcess sdkProc;
    sdkProc.start(path, QStringList() << "-s" << deviceId << "shell" << "getprop" << "ro.build.version.sdk");
    sdkProc.waitForFinished(3000);
    detail.sdkVersion = QString::fromLocal8Bit(sdkProc.readAllStandardOutput()).trimmed();

    // Get battery level
    QProcess batProc;
    batProc.start(path, QStringList() << "-s" << deviceId << "shell" << "dumpsys" << "battery");
    batProc.waitForFinished(3000);
    QString batOutput = QString::fromLocal8Bit(batProc.readAllStandardOutput());
    QRegularExpression re("level: (\\d+)");
    QRegularExpressionMatch match = re.match(batOutput);
    if (match.hasMatch()) {
        detail.batteryLevel = match.captured(1).toInt();
    }

    return detail;
}

bool AdbController::shell(const QString &deviceId, const QString &command, QString *output) const
{
    QString path = adbPath();
    if (path.isEmpty()) return false;

    QProcess process;
    process.start(path, QStringList() << "-s" << deviceId << "shell" << command);
    process.waitForFinished(10000);
    if (output) {
        *output = QString::fromLocal8Bit(process.readAllStandardOutput());
    }
    return process.exitCode() == 0;
}

bool AdbController::install(const QString &deviceId, const QString &apkPath) const
{
    QString path = adbPath();
    if (path.isEmpty()) return false;

    QProcess process;
    process.start(path, QStringList() << "-s" << deviceId << "install" << apkPath);
    process.waitForFinished(60000);
    return process.exitCode() == 0;
}

bool AdbController::pull(const QString &deviceId, const QString &remotePath, const QString &localPath) const
{
    QString path = adbPath();
    if (path.isEmpty()) return false;

    QProcess process;
    process.start(path, QStringList() << "-s" << deviceId << "pull" << remotePath << localPath);
    process.waitForFinished(30000);
    return process.exitCode() == 0;
}

bool AdbController::push(const QString &deviceId, const QString &localPath, const QString &remotePath) const
{
    QString path = adbPath();
    if (path.isEmpty()) return false;

    QProcess process;
    process.start(path, QStringList() << "-s" << deviceId << "push" << localPath << remotePath);
    process.waitForFinished(30000);
    return process.exitCode() == 0;
}

bool AdbController::logcat(const QString &deviceId, QString *output) const
{
    QString path = adbPath();
    if (path.isEmpty()) return false;

    QProcess process;
    process.start(path, QStringList() << "-s" << deviceId << "logcat" << "-d" << "-t" << "200");
    process.waitForFinished(10000);
    if (output) {
        *output = QString::fromLocal8Bit(process.readAllStandardOutput());
    }
    return process.exitCode() == 0;
}

bool AdbController::uiautomatorDump(const QString &deviceId, QString *xmlOutput) const
{
    QString path = adbPath();
    if (path.isEmpty()) return false;

    QProcess process;
    process.start(path, QStringList() << "-s" << deviceId << "shell" << "uiautomator" << "dump" << "/sdcard/window_dump.xml");
    process.waitForFinished(10000);

    if (xmlOutput) {
        QProcess catProcess;
        catProcess.start(path, QStringList() << "-s" << deviceId << "shell" << "cat" << "/sdcard/window_dump.xml");
        catProcess.waitForFinished(10000);
        *xmlOutput = QString::fromLocal8Bit(catProcess.readAllStandardOutput());
    }
    return process.exitCode() == 0;
}

bool AdbController::dumpsys(const QString &deviceId, const QString &service, QString *output) const
{
    QString path = adbPath();
    if (path.isEmpty()) return false;

    QProcess process;
    process.start(path, QStringList() << "-s" << deviceId << "shell" << "dumpsys" << service);
    process.waitForFinished(10000);
    if (output) {
        *output = QString::fromLocal8Bit(process.readAllStandardOutput());
    }
    return process.exitCode() == 0;
}

} // namespace adb
} // namespace core
