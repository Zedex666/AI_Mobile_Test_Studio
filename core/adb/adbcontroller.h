#ifndef CORE_ADB_ADBCONTROLLER_H
#define CORE_ADB_ADBCONTROLLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QSize>

namespace core {
namespace adb {

struct DeviceDetail {
    QString id;
    QString status;       // device, offline, unauthorized
    QString model;
    QString deviceName;
    QString androidVersion;
    QString sdkVersion;
    int batteryLevel = -1;
    bool isEmulator = false;
};

class AdbController : public QObject
{
    Q_OBJECT

public:
    explicit AdbController(QObject *parent = nullptr);

    bool isAvailable() const;
    QString adbPath() const;

    QStringList devices() const;
    QMap<QString, DeviceDetail> deviceDetails() const;
    DeviceDetail getDeviceDetail(const QString &deviceId) const;

    bool shell(const QString &deviceId, const QString &command, QString *output = nullptr) const;
    bool install(const QString &deviceId, const QString &apkPath) const;
    bool pull(const QString &deviceId, const QString &remotePath, const QString &localPath) const;
    bool push(const QString &deviceId, const QString &localPath, const QString &remotePath) const;
    bool logcat(const QString &deviceId, QString *output) const;
    bool uiautomatorDump(const QString &deviceId, QString *xmlOutput) const;
    bool dumpsys(const QString &deviceId, const QString &service, QString *output) const;

    // Device screen info for adaptive frame (phone/tablet/watch)
    QSize screenSize(const QString &deviceId) const;
    QString deviceCharacteristics(const QString &deviceId) const;

signals:
    void deviceConnected(const QString &deviceId);
    void deviceDisconnected(const QString &deviceId);
    void logOutput(const QString &text);
};

} // namespace adb
} // namespace core

#endif // CORE_ADB_ADBCONTROLLER_H
