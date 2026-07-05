#include "devicepanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QDebug>

DevicePanel::DevicePanel(QWidget *parent)
    : QWidget(parent)
    , m_scrcpy(new core::scrcpy::ScrcpyController(this))
{
    setupUI();
    setupStyle();

    connect(m_scrcpy, &core::scrcpy::ScrcpyController::started, this, [this]() {
        m_phoneScreen->setText(tr(""));
    });
    connect(m_scrcpy, &core::scrcpy::ScrcpyController::stopped, this, [this]() {
        m_phoneScreen->setText(tr("手机屏幕画面\n(Scrcpy 未启动)"));
    });
    connect(m_scrcpy, &core::scrcpy::ScrcpyController::error, this, [this](const QString &msg) {
        m_phoneScreen->setText(tr("Scrcpy 启动失败\n%1").arg(msg));
        qWarning() << "Scrcpy error:" << msg;
    });
}

DevicePanel::~DevicePanel()
{
    stopScrcpy();
}

void DevicePanel::setupUI()
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 12, 16, 12);
    mainLayout->setSpacing(12);

    // Top bar: device info + controls
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setSpacing(8);

    m_deviceNameLabel = new QLabel(tr("未选择设备"), this);
    m_deviceNameLabel->setStyleSheet("font-size: 14px; font-weight: 600; color: #1F2937;");

    m_statusLabel = new QLabel(tr("● 等待连接"), this);
    m_statusLabel->setStyleSheet("font-size: 12px; color: #9CA3AF;");

    topLayout->addWidget(m_deviceNameLabel);
    topLayout->addWidget(m_statusLabel);
    topLayout->addStretch();

    m_refreshBtn = new QPushButton(tr("刷新"), this);
    m_refreshBtn->setFixedSize(56, 28);

    m_screenshotBtn = new QPushButton(tr("截图"), this);
    m_screenshotBtn->setFixedSize(56, 28);

    m_recordBtn = new QPushButton(tr("录屏"), this);
    m_recordBtn->setFixedSize(56, 28);

    m_moreBtn = new QPushButton(tr("更多"), this);
    m_moreBtn->setFixedSize(56, 28);

    topLayout->addWidget(m_refreshBtn);
    topLayout->addWidget(m_screenshotBtn);
    topLayout->addWidget(m_recordBtn);
    topLayout->addWidget(m_moreBtn);

    // Phone screen area with side controls
    QHBoxLayout *phoneLayout = new QHBoxLayout();
    phoneLayout->setSpacing(12);

    // Left: Phone screen with bezel frame
    m_phoneFrame = new QFrame(this);
    m_phoneFrame->setStyleSheet(R"(
        QFrame {
            background-color: #1F2937;
            border-radius: 28px;
            border: 3px solid #374151;
        }
    )");
    QVBoxLayout *phoneFrameLayout = new QVBoxLayout(m_phoneFrame);
    phoneFrameLayout->setContentsMargins(6, 6, 6, 6);

    m_phoneScreen = new QLabel(this);
    m_phoneScreen->setMinimumSize(260, 520);
    m_phoneScreen->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_phoneScreen->setAlignment(Qt::AlignCenter);
    m_phoneScreen->setText(tr("手机屏幕画面\n(Scrcpy 未启动)"));
    m_phoneScreen->setStyleSheet(R"(
        background-color: #111827;
        border-radius: 22px;
        color: #6B7280;
        font-size: 14px;
    )");

    // Add shadow effect to phone
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(32);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(0, 10);
    m_phoneFrame->setGraphicsEffect(shadow);

    phoneFrameLayout->addWidget(m_phoneScreen);
    phoneLayout->addWidget(m_phoneFrame, 1);

    // Scrcpy embed container
    m_scrcpy->setEmbedContainer(m_phoneFrame);

    // Right: Side control buttons
    QVBoxLayout *sideLayout = new QVBoxLayout();
    sideLayout->setSpacing(8);
    sideLayout->setAlignment(Qt::AlignVCenter);

    m_backBtn = new QPushButton(tr("←"), this);
    m_backBtn->setFixedSize(40, 40);
    m_backBtn->setToolTip(tr("返回"));

    m_homeBtn = new QPushButton(tr("⌂"), this);
    m_homeBtn->setFixedSize(40, 40);
    m_homeBtn->setToolTip(tr("Home"));

    m_recentBtn = new QPushButton(tr("▢"), this);
    m_recentBtn->setFixedSize(40, 40);
    m_recentBtn->setToolTip(tr("多任务"));

    m_rotateBtn = new QPushButton(tr("↻"), this);
    m_rotateBtn->setFixedSize(40, 40);
    m_rotateBtn->setToolTip(tr("旋转"));

    m_volUpBtn = new QPushButton(tr("+"), this);
    m_volUpBtn->setFixedSize(40, 40);
    m_volUpBtn->setToolTip(tr("音量+"));

    m_volDownBtn = new QPushButton(tr("−"), this);
    m_volDownBtn->setFixedSize(40, 40);
    m_volDownBtn->setToolTip(tr("音量−"));

    m_moreSideBtn = new QPushButton(tr("⋯"), this);
    m_moreSideBtn->setFixedSize(40, 40);
    m_moreSideBtn->setToolTip(tr("更多"));

    sideLayout->addWidget(m_backBtn);
    sideLayout->addWidget(m_homeBtn);
    sideLayout->addWidget(m_recentBtn);
    sideLayout->addSpacing(16);
    sideLayout->addWidget(m_rotateBtn);
    sideLayout->addSpacing(16);
    sideLayout->addWidget(m_volUpBtn);
    sideLayout->addWidget(m_volDownBtn);
    sideLayout->addSpacing(16);
    sideLayout->addWidget(m_moreSideBtn);
    sideLayout->addStretch();

    phoneLayout->addLayout(sideLayout, 0);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(phoneLayout, 1);

    // Connections
    connect(m_refreshBtn, &QPushButton::clicked, this, &DevicePanel::refreshClicked);
    connect(m_screenshotBtn, &QPushButton::clicked, this, &DevicePanel::screenshotClicked);
    connect(m_recordBtn, &QPushButton::clicked, this, &DevicePanel::recordClicked);
    connect(m_moreBtn, &QPushButton::clicked, this, &DevicePanel::moreClicked);
    connect(m_backBtn, &QPushButton::clicked, this, &DevicePanel::backClicked);
    connect(m_homeBtn, &QPushButton::clicked, this, &DevicePanel::homeClicked);
    connect(m_recentBtn, &QPushButton::clicked, this, &DevicePanel::recentClicked);
    connect(m_rotateBtn, &QPushButton::clicked, this, &DevicePanel::rotateClicked);
    connect(m_volUpBtn, &QPushButton::clicked, this, &DevicePanel::volumeUpClicked);
    connect(m_volDownBtn, &QPushButton::clicked, this, &DevicePanel::volumeDownClicked);
}

void DevicePanel::setupStyle()
{
    setStyleSheet(R"(
        DevicePanel {
            background-color: #FFFFFF;
        }
        QPushButton {
            background-color: #F3F4F6;
            border: 1px solid #E5E7EB;
            border-radius: 8px;
            font-size: 13px;
            color: #4B5563;
        }
        QPushButton:hover {
            background-color: #E5E7EB;
            color: #1F2937;
        }
        QPushButton:pressed {
            background-color: #D1D5DB;
        }
    )");
}

void DevicePanel::setDeviceInfo(const services::DeviceInfo &info)
{
    m_currentInfo = info;

    if (info.id.isEmpty()) {
        m_deviceNameLabel->setText(tr("未选择设备"));
        updateStatusLabel(false);
        stopScrcpy();
        return;
    }

    m_deviceNameLabel->setText(info.name);
    updateStatusLabel(info.isConnected);

    // Auto-start scrcpy if connected and not already running
    if (info.isConnected && !isScrcpyRunning()) {
        startScrcpy(info.id);
    }
}

void DevicePanel::setAdbAvailable(bool available)
{
    m_adbAvailable = available;
    if (!available) {
        updateStatusLabel(false);
        stopScrcpy();
    }
}

bool DevicePanel::startScrcpy(const QString &deviceId)
{
    if (!m_scrcpy->isAvailable()) {
        m_phoneScreen->setText(tr("Scrcpy 未找到\n请安装 scrcpy 并确保在 PATH 中"));
        return false;
    }

    m_scrcpy->setDevice(deviceId);
    return m_scrcpy->start();
}

void DevicePanel::stopScrcpy()
{
    m_scrcpy->stop();
}

bool DevicePanel::isScrcpyRunning() const
{
    return m_scrcpy->isRunning();
}

void DevicePanel::updateStatusLabel(bool connected)
{
    if (connected) {
        m_statusLabel->setText(tr("● 已连接"));
        m_statusLabel->setStyleSheet("font-size: 12px; color: #10B981;");
    } else if (m_adbAvailable) {
        m_statusLabel->setText(tr("● 未连接"));
        m_statusLabel->setStyleSheet("font-size: 12px; color: #EF4444;");
    } else {
        m_statusLabel->setText(tr("● ADB 不可用"));
        m_statusLabel->setStyleSheet("font-size: 12px; color: #9CA3AF;");
    }
}

void DevicePanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_scrcpy->updateEmbedGeometry();
}
