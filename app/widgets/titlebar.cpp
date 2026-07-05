#include "titlebar.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QFrame>

TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupStyle();
}

void TitleBar::setupUI()
{
    setFixedHeight(56);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(16, 0, 16, 0);
    mainLayout->setSpacing(12);

    // Left: Logo + Title
    QHBoxLayout *leftLayout = new QHBoxLayout();
    leftLayout->setSpacing(8);

    m_logoLabel = new QLabel(this);
    m_logoLabel->setFixedSize(28, 28);
    m_logoLabel->setStyleSheet("background-color: #4F6EF7; border-radius: 6px;");

    m_titleLabel = new QLabel(tr("AI Mobile Test Studio"), this);
    m_titleLabel->setStyleSheet("font-size: 15px; font-weight: 600; color: #1F2937;");

    leftLayout->addWidget(m_logoLabel);
    leftLayout->addWidget(m_titleLabel);
    leftLayout->addStretch();

    // Center: Device selector + controls
    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->setSpacing(8);

    m_deviceCombo = new QComboBox(this);
    m_deviceCombo->setFixedWidth(220);
    m_deviceCombo->addItem(tr("🔍 正在检测设备..."));
    m_deviceCombo->setEnabled(false);

    m_refreshBtn = new QPushButton(this);
    m_refreshBtn->setFixedSize(32, 32);
    m_refreshBtn->setToolTip(tr("刷新设备列表"));
    m_refreshBtn->setText("↻");

    m_screenshotBtn = new QPushButton(this);
    m_screenshotBtn->setFixedSize(32, 32);
    m_screenshotBtn->setToolTip(tr("截图"));
    m_screenshotBtn->setText("📷");

    m_recordBtn = new QPushButton(this);
    m_recordBtn->setFixedSize(32, 32);
    m_recordBtn->setToolTip(tr("录屏"));
    m_recordBtn->setText("⏺");

    m_moreBtn = new QPushButton(this);
    m_moreBtn->setFixedSize(32, 32);
    m_moreBtn->setToolTip(tr("更多"));
    m_moreBtn->setText("⋯");

    centerLayout->addWidget(m_deviceCombo);
    centerLayout->addSpacing(12);
    centerLayout->addWidget(m_refreshBtn);
    centerLayout->addWidget(m_screenshotBtn);
    centerLayout->addWidget(m_recordBtn);
    centerLayout->addWidget(m_moreBtn);
    centerLayout->addStretch();

    // Right: Notification + Settings + User
    QHBoxLayout *rightLayout = new QHBoxLayout();
    rightLayout->setSpacing(8);

    m_notificationBtn = new QPushButton(this);
    m_notificationBtn->setFixedSize(32, 32);
    m_notificationBtn->setToolTip(tr("通知"));
    m_notificationBtn->setText("🔔");

    m_settingsBtn = new QPushButton(this);
    m_settingsBtn->setFixedSize(32, 32);
    m_settingsBtn->setToolTip(tr("设置"));
    m_settingsBtn->setText("⚙");

    m_userBtn = new QPushButton(this);
    m_userBtn->setFixedSize(32, 32);
    m_userBtn->setToolTip(tr("用户"));
    m_userBtn->setText("👤");
    m_userBtn->setStyleSheet("border-radius: 16px; background-color: #E5E7EB;");

    rightLayout->addWidget(m_notificationBtn);
    rightLayout->addWidget(m_settingsBtn);
    rightLayout->addWidget(m_userBtn);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(centerLayout, 2);
    mainLayout->addLayout(rightLayout, 0);

    // Connections
    connect(m_refreshBtn, &QPushButton::clicked, this, &TitleBar::refreshClicked);
    connect(m_screenshotBtn, &QPushButton::clicked, this, &TitleBar::screenshotClicked);
    connect(m_recordBtn, &QPushButton::clicked, this, &TitleBar::recordClicked);
    connect(m_moreBtn, &QPushButton::clicked, this, &TitleBar::moreClicked);
    connect(m_notificationBtn, &QPushButton::clicked, this, &TitleBar::notificationClicked);
    connect(m_settingsBtn, &QPushButton::clicked, this, &TitleBar::settingsClicked);
    connect(m_userBtn, &QPushButton::clicked, this, &TitleBar::userProfileClicked);

    connect(m_deviceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index) {
        if (index >= 0 && m_deviceCombo->count() > 0) {
            QString id = m_deviceCombo->itemData(index).toString();
            if (!id.isEmpty()) {
                emit deviceSelected(id);
            }
        }
    });
}

void TitleBar::setupStyle()
{
    setStyleSheet(R"(
        TitleBar {
            background-color: #FFFFFF;
            border-bottom: 1px solid #E5E7EB;
        }
        QComboBox {
            background-color: #F3F4F6;
            border: 1px solid #E5E7EB;
            border-radius: 6px;
            padding: 4px 8px;
            font-size: 13px;
            color: #374151;
        }
        QComboBox::drop-down {
            border: none;
            width: 24px;
        }
        QComboBox QAbstractItemView {
            background-color: #FFFFFF;
            border: 1px solid #E5E7EB;
            selection-background-color: #EFF6FF;
        }
        QPushButton {
            background-color: transparent;
            border: none;
            border-radius: 6px;
            font-size: 14px;
            color: #6B7280;
        }
        QPushButton:hover {
            background-color: #F3F4F6;
            color: #374151;
        }
        QPushButton:pressed {
            background-color: #E5E7EB;
        }
    )");
}

void TitleBar::setDevices(const QMap<QString, services::DeviceInfo> &devices)
{
    m_devices = devices;

    QString currentId = m_deviceCombo->currentData().toString();
    m_deviceCombo->clear();

    if (devices.isEmpty()) {
        m_deviceCombo->addItem(tr("❌ 未检测到设备"));
        m_deviceCombo->setEnabled(false);
        return;
    }

    m_deviceCombo->setEnabled(true);
    for (auto it = devices.begin(); it != devices.end(); ++it) {
        const services::DeviceInfo &info = it.value();
        QString label;
        if (info.isEmulator) {
            label = tr("📱 %1 (模拟器)").arg(info.name);
        } else {
            label = tr("📱 %1 | Android %2 | 🔋%3%").arg(info.name, info.androidVersion).arg(info.batteryLevel);
        }
        m_deviceCombo->addItem(label, info.id);
    }

    // Restore selection if still present
    if (!currentId.isEmpty()) {
        int idx = m_deviceCombo->findData(currentId);
        if (idx >= 0) {
            m_deviceCombo->setCurrentIndex(idx);
        }
    }
}

void TitleBar::setCurrentDevice(const QString &deviceId)
{
    int idx = m_deviceCombo->findData(deviceId);
    if (idx >= 0) {
        m_deviceCombo->setCurrentIndex(idx);
    }
}

void TitleBar::setAdbStatus(bool available)
{
    if (!available && m_deviceCombo->count() == 0) {
        m_deviceCombo->clear();
        m_deviceCombo->addItem(tr("❌ ADB 未安装"));
        m_deviceCombo->setEnabled(false);
    }
}
