#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QMap>
#include "services/device_service.h"

class QLabel;
class QPushButton;
class QComboBox;

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);

    void setDevices(const QMap<QString, services::DeviceInfo> &devices);
    void setCurrentDevice(const QString &deviceId);
    void setAdbStatus(bool available);

signals:
    void refreshClicked();
    void screenshotClicked();
    void recordClicked();
    void moreClicked();
    void notificationClicked();
    void settingsClicked();
    void userProfileClicked();
    void deviceSelected(const QString &deviceId);

private:
    void setupUI();
    void setupStyle();

    QLabel *m_logoLabel;
    QLabel *m_titleLabel;
    QComboBox *m_deviceCombo;
    QPushButton *m_refreshBtn;
    QPushButton *m_screenshotBtn;
    QPushButton *m_recordBtn;
    QPushButton *m_moreBtn;
    QPushButton *m_notificationBtn;
    QPushButton *m_settingsBtn;
    QPushButton *m_userBtn;

    QMap<QString, services::DeviceInfo> m_devices;
};

#endif // TITLEBAR_H
