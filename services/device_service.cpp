#include "device_service.h"
#include "core/logger/logger.h"
#include <QDebug>

namespace services {

DeviceService::DeviceService(QObject *parent)
    : QObject(parent)
    , m_adb(std::make_unique<core::adb::AdbController>())
    , m_pollTimer(new QTimer(this))
{
    qRegisterMetaType<DeviceInfo>("DeviceInfo");

    connect(m_pollTimer, &QTimer::timeout, this, &DeviceService::onPollTimeout);

    // Initial check
    m_adbAvailable = m_adb->isAvailable();
    if (m_adbAvailable) {
        core::logger::Logger::instance()->info("ADB", tr("ADB available at: %1").arg(m_adb->adbPath()));
        updateDevices();
    } else {
        core::logger::Logger::instance()->warning("ADB", tr("ADB not found in PATH or sdk/adb/"));
    }
}

DeviceService::~DeviceService() = default;

QStringList DeviceService::deviceIds() const
{
    return m_devices.keys();
}

DeviceInfo DeviceService::deviceInfo(const QString &deviceId) const
{
    return m_devices.value(deviceId);
}

QMap<QString, DeviceInfo> DeviceService::devices() const
{
    return m_devices;
}

QString DeviceService::currentDevice() const
{
    return m_currentDevice;
}

void DeviceService::setCurrentDevice(const QString &deviceId)
{
    if (m_currentDevice != deviceId) {
        m_currentDevice = deviceId;
        emit currentDeviceChanged(deviceId);
        core::logger::Logger::instance()->info("Device", tr("Current device changed to: %1").arg(deviceId));
    }
}

bool DeviceService::adbAvailable() const
{
    return m_adbAvailable;
}

void DeviceService::refreshDevices()
{
    bool wasAvailable = m_adbAvailable;
    m_adbAvailable = m_adb->isAvailable();

    if (wasAvailable != m_adbAvailable) {
        emit adbStatusChanged(m_adbAvailable);
    }

    if (m_adbAvailable) {
        updateDevices();
    } else {
        if (!m_devices.isEmpty()) {
            m_devices.clear();
            emit devicesChanged(m_devices);
        }
    }
}

bool DeviceService::connectDevice(const QString &deviceId)
{
    Q_UNUSED(deviceId)
    // ADB devices are auto-connected; this could implement pairing for wireless
    return true;
}

bool DeviceService::disconnectDevice(const QString &deviceId)
{
    Q_UNUSED(deviceId)
    // Could implement adb disconnect for wireless
    return true;
}

void DeviceService::startPolling(int intervalMs)
{
    m_pollTimer->start(intervalMs);
    core::logger::Logger::instance()->info("Device", tr("Started device polling every %1 ms").arg(intervalMs));
}

void DeviceService::stopPolling()
{
    m_pollTimer->stop();
}

void DeviceService::onPollTimeout()
{
    refreshDevices();
}

void DeviceService::updateDevices()
{
    QMap<QString, core::adb::DeviceDetail> details = m_adb->deviceDetails();

    // Detect disconnections
    for (auto it = m_devices.begin(); it != m_devices.end(); ) {
        if (!details.contains(it.key())) {
            QString id = it.key();
            core::logger::Logger::instance()->info("Device", tr("Device disconnected: %1").arg(id));
            emit deviceDisconnected(id);
            it = m_devices.erase(it);
        } else {
            ++it;
        }
    }

    // Detect new connections and updates
    for (auto it = details.begin(); it != details.end(); ++it) {
        const core::adb::DeviceDetail &detail = it.value();
        bool isNew = !m_devices.contains(it.key());

        DeviceInfo info;
        info.id = detail.id;
        info.model = detail.model;
        info.name = detail.deviceName.isEmpty() ? detail.model : detail.deviceName;
        info.androidVersion = detail.androidVersion;
        info.batteryLevel = detail.batteryLevel;
        info.isConnected = (detail.status == "device");
        info.isEmulator = detail.isEmulator;

        m_devices[it.key()] = info;

        if (isNew) {
            core::logger::Logger::instance()->info("Device", tr("Device connected: %1 (%2)").arg(info.name, info.androidVersion));
            emit deviceConnected(it.key());

            // Auto-select first device if none selected
            if (m_currentDevice.isEmpty()) {
                setCurrentDevice(it.key());
            }
        }
    }

    emit devicesChanged(m_devices);
}

} // namespace services
