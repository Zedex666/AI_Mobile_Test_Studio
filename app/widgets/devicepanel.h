#ifndef DEVICEPANEL_H
#define DEVICEPANEL_H

#include <QWidget>
#include <QMap>
#include "services/device_service.h"
#include "core/scrcpy/scrcpycontroller.h"

class QLabel;
class QPushButton;
class QFrame;

class DevicePanel : public QWidget
{
    Q_OBJECT

public:
    explicit DevicePanel(QWidget *parent = nullptr);
    ~DevicePanel();

    void setDeviceInfo(const services::DeviceInfo &info);
    void setAdbAvailable(bool available);

    bool startScrcpy(const QString &deviceId);
    void stopScrcpy();
    bool isScrcpyRunning() const;

signals:
    void refreshClicked();
    void screenshotClicked();
    void recordClicked();
    void moreClicked();
    void homeClicked();
    void backClicked();
    void recentClicked();
    void rotateClicked();
    void volumeUpClicked();
    void volumeDownClicked();

private:
    void setupUI();
    void setupStyle();
    void updateStatusLabel(bool connected);

protected:
    void resizeEvent(QResizeEvent *event) override;

    QLabel *m_deviceNameLabel;
    QLabel *m_statusLabel;
    QPushButton *m_refreshBtn;
    QPushButton *m_screenshotBtn;
    QPushButton *m_recordBtn;
    QPushButton *m_moreBtn;

    // Phone screen placeholder / scrcpy container
    QFrame *m_phoneFrame;
    QLabel *m_phoneScreen;

    // Side controls
    QPushButton *m_backBtn;
    QPushButton *m_homeBtn;
    QPushButton *m_recentBtn;
    QPushButton *m_rotateBtn;
    QPushButton *m_volUpBtn;
    QPushButton *m_volDownBtn;
    QPushButton *m_moreSideBtn;

    core::scrcpy::ScrcpyController *m_scrcpy = nullptr;
    services::DeviceInfo m_currentInfo;
    bool m_adbAvailable = false;
};

#endif // DEVICEPANEL_H
