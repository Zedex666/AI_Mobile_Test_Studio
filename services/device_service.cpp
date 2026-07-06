#include "device_service.h"
#include "core/logger/logger.h"
#include <QDebug>
#include <QSize>

namespace services {

DeviceService::DeviceService(QObject *parent)
    : QObject(parent)
    , m_adb(std::make_unique<core::adb::AdbController>())
    , m_pollTimer(new QTimer(this))
{
    qRegisterMetaType<DeviceInfo>("DeviceInfo");

    connect(m_pollTimer, &QTimer::timeout, this, &DeviceService::onPollTimeout);

    // Initial check (only check adb availability, do not update devices yet
    // because signal connections in MainWindow may not be ready)
    m_adbAvailable = m_adb->isAvailable();
    if (m_adbAvailable) {
        core::logger::Logger::instance()->info("ADB", tr("ADB available at: %1").arg(m_adb->adbPath()));
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

    core::logger::Logger::instance()->info("DeviceService", tr("refreshDevices: adbAvailable=%1").arg(m_adbAvailable));

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

    core::logger::Logger::instance()->info("DeviceService", tr("updateDevices: found %1 device(s)").arg(details.size()));

    // Detect disconnections
    bool currentDisconnected = false;
    for (auto it = m_devices.begin(); it != m_devices.end(); ) {
        if (!details.contains(it.key())) {
            QString id = it.key();
            if (id == m_currentDevice) {
                currentDisconnected = true;
            }
            core::logger::Logger::instance()->info("Device", tr("Device disconnected: %1").arg(id));
            emit deviceDisconnected(id);
            it = m_devices.erase(it);
        } else {
            ++it;
        }
    }

    // Clear current device if it was disconnected
    if (currentDisconnected) {
        setCurrentDevice(QString());
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

        // Detect screen size and device type for adaptive frame
        QSize screen = m_adb->screenSize(info.id);
        info.screenWidth = screen.width();
        info.screenHeight = screen.height();

        QString characteristics = m_adb->deviceCharacteristics(info.id).toLower();
        if (characteristics.contains("watch")) {
            info.deviceType = DeviceType::Watch;
        } else if (characteristics.contains("tablet")) {
            info.deviceType = DeviceType::Tablet;
        } else if (info.screenWidth > 0 && info.screenHeight > 0) {
            // Heuristic classification based on screen dimensions
            double aspectRatio = static_cast<double>(info.screenWidth) / info.screenHeight;
            if (info.screenWidth <= 500 && info.screenHeight <= 500) {
                info.deviceType = DeviceType::Watch;
            } else if (info.screenWidth >= 600 && aspectRatio > 0.6 && aspectRatio < 1.7) {
                info.deviceType = DeviceType::Tablet;
            } else {
                info.deviceType = DeviceType::Phone;
            }
        } else {
            info.deviceType = DeviceType::Phone; // default
        }

        core::logger::Logger::instance()->info("DeviceService",
            tr("Device %1 status=%2 connected=%3 type=%4 size=%5x%6")
                .arg(info.id, detail.status).arg(info.isConnected)
                .arg(static_cast<int>(info.deviceType))
                .arg(info.screenWidth).arg(info.screenHeight));

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
