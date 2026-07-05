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

struct DeviceInfo {
    QString id;
    QString name;
    QString model;
    QString androidVersion;
    int batteryLevel = -1;
    bool isConnected = false;
    bool isEmulator = false;
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
