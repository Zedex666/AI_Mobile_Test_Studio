#ifndef SERVICES_DEVICE_SERVICE_H
#define SERVICES_DEVICE_SERVICE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QTimer>
#include <memory>

#include "core/adb/adbcontroller.h"

namespace services {

enum class DeviceType {
    Unknown = 0,
    Phone,
    Tablet,
    Watch
};

struct DeviceInfo {
    QString id;
    QString name;
    QString model;
    QString androidVersion;
    int batteryLevel = -1;
    bool isConnected = false;
    bool isEmulator = false;
    // Screen info for adaptive frame shape
    DeviceType deviceType = DeviceType::Unknown;
    int screenWidth = 0;
    int screenHeight = 0;
};

class DeviceService : public QObject
{
    Q_OBJECT

public:
    explicit DeviceService(QObject *parent = nullptr);
    ~DeviceService();

    QStringList deviceIds() const;
    DeviceInfo deviceInfo(const QString &deviceId) const;
    QMap<QString, DeviceInfo> devices() const;

    QString currentDevice() const;
    void setCurrentDevice(const QString &deviceId);

    bool adbAvailable() const;

    void refreshDevices();
    bool connectDevice(const QString &deviceId);
    bool disconnectDevice(const QString &deviceId);

    void startPolling(int intervalMs = 3000);
    void stopPolling();

    // Device control via ADB key events
    bool sendKeyEvent(const QString &deviceId, int keyCode);
    void toggleRotation(const QString &deviceId);

signals:
    void devicesChanged(const QMap<QString, DeviceInfo> &devices);
    void deviceConnected(const QString &deviceId);
    void deviceDisconnected(const QString &deviceId);
    void currentDeviceChanged(const QString &deviceId);
    void adbStatusChanged(bool available);
    void logOutput(const QString &text);

private slots:
    void onPollTimeout();

private:
    void updateDevices();

    std::unique_ptr<core::adb::AdbController> m_adb;
    QMap<QString, DeviceInfo> m_devices;
    QString m_currentDevice;
    QTimer *m_pollTimer = nullptr;
    bool m_adbAvailable = false;
};

} // namespace services

Q_DECLARE_METATYPE(services::DeviceInfo)

#endif // SERVICES_DEVICE_SERVICE_H
