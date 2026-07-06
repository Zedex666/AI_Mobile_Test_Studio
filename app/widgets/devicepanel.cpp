#include "devicepanel.h"
#include "core/logger/logger.h"
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
        // Keep placeholder visible until window is actually embedded
    });
    connect(m_scrcpy, &core::scrcpy::ScrcpyController::embedded, this, [this]() {
        m_phoneScreen->hide();
    });
    connect(m_scrcpy, &core::scrcpy::ScrcpyController::stopped, this, [this]() {
        m_phoneScreen->show();
        m_phoneScreen->setText(tr("手机屏幕画面\n(Scrcpy 未启动)"));
    });
    connect(m_scrcpy, &core::scrcpy::ScrcpyController::error, this, [this](const QString &msg) {
        m_phoneScreen->show();
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

    // Wrapper widget for the drop shadow.
    // IMPORTANT: the shadow effect must NOT be on m_phoneFrame itself,
    // because m_phoneFrame hosts the natively-embedded Scrcpy window.
    // QGraphicsEffect redirects painting through an offscreen buffer
    // which can hide native child windows on Windows.
    QWidget *phoneShadowWrapper = new QWidget(this);
    QVBoxLayout *wrapperLayout = new QVBoxLayout(phoneShadowWrapper);
    // Padding accommodates the shadow blur (32px) and offset (0, 10)
    wrapperLayout->setContentsMargins(16, 16, 16, 26);
    wrapperLayout->setSpacing(0);

    // Phone bezel frame — hosts the embedded Scrcpy native window.
    // WA_NativeWindow ensures a stable HWND that persists across layout changes.
    m_phoneFrame = new QFrame(phoneShadowWrapper);
    m_phoneFrame->setAttribute(Qt::WA_NativeWindow, true);
    m_phoneFrame->setStyleSheet(R"(
        QFrame {
            background-color: #1F2937;
            border-radius: 28px;
            border: 3px solid #374151;
        }
    )");
    QVBoxLayout *phoneFrameLayout = new QVBoxLayout(m_phoneFrame);
    phoneFrameLayout->setContentsMargins(6, 6, 6, 6);

    m_phoneScreen = new QLabel(phoneShadowWrapper);
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

    // Apply drop shadow to the WRAPPER, not to m_phoneFrame.
    // The wrapper only contains the phone bezel frame — no native child windows.
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(32);
    shadow->setColor(QColor(0, 0, 0, 80));
    shadow->setOffset(0, 10);
    phoneShadowWrapper->setGraphicsEffect(shadow);

    phoneFrameLayout->addWidget(m_phoneScreen);
    wrapperLayout->addWidget(m_phoneFrame, 0, Qt::AlignCenter);
    phoneLayout->addWidget(phoneShadowWrapper, 1);

    // Scrcpy embed container — m_phoneFrame has no graphics effect, safe for native embedding
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
    core::logger::Logger::instance()->info("DevicePanel",
        tr("setDeviceInfo id=%1 connected=%2 running=%3").arg(info.id).arg(info.isConnected).arg(isScrcpyRunning()));
    m_currentInfo = info;

    if (info.id.isEmpty()) {
        m_deviceNameLabel->setText(tr("未选择设备"));
        updateStatusLabel(false);
        stopScrcpy();
        // Reset to default phone frame shape
        m_deviceType = services::DeviceType::Phone;
        m_targetAspectRatio = 0.45;
        updateFrameShape();
        return;
    }

    m_deviceNameLabel->setText(info.name);
    updateStatusLabel(info.isConnected);

    // Apply device-type-specific frame shape (phone/tablet/watch)
    applyDeviceShape(info);

    // Auto-start scrcpy if connected and not already running
    if (info.isConnected && !isScrcpyRunning()) {
        core::logger::Logger::instance()->info("DevicePanel", tr("Auto-starting scrcpy for %1").arg(info.id));
        startScrcpy(info.id);
    } else {
        core::logger::Logger::instance()->info("DevicePanel",
            tr("Skip scrcpy: isConnected=%1 isRunning=%2").arg(info.isConnected).arg(isScrcpyRunning()));
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
        m_phoneScreen->show();
        m_phoneScreen->setText(tr("Scrcpy 未找到\n请安装 scrcpy 并确保在 PATH 中"));
        return false;
    }

    m_phoneScreen->show();
    m_phoneScreen->setText(tr("正在启动 Scrcpy..."));

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

void DevicePanel::applyDeviceShape(const services::DeviceInfo &info)
{
    m_deviceType = info.deviceType;
    m_currentInfo.deviceType = info.deviceType;
    m_currentInfo.screenWidth = info.screenWidth;
    m_currentInfo.screenHeight = info.screenHeight;

    // Calculate target aspect ratio (width / height) based on device type
    // This determines the frame shape — phone=tall, tablet=wide, watch=square
    if (info.screenWidth > 0 && info.screenHeight > 0) {
        // Use actual device screen aspect ratio for best fit
        m_targetAspectRatio = static_cast<double>(info.screenWidth) / info.screenHeight;
    } else {
        // Fallback: default aspect ratios by device type
        switch (info.deviceType) {
        case services::DeviceType::Watch:
            m_targetAspectRatio = 1.0;   // square (e.g. 390x390)
            break;
        case services::DeviceType::Tablet:
            m_targetAspectRatio = 1.33;  // wider frame, ~4:3 (e.g. 2560x1600 → W/H=1.6, use 1.33)
            break;
        case services::DeviceType::Phone:
        default:
            m_targetAspectRatio = 0.45;  // tall (~9:20, e.g. 1080x2400)
            break;
        }
    }

    updateFrameShape();
    core::logger::Logger::instance()->info("DevicePanel",
        tr("Device shape applied: type=%1 aspect=%2").arg(static_cast<int>(m_deviceType)).arg(m_targetAspectRatio));
}

void DevicePanel::updateFrameShape()
{
    if (!m_phoneFrame) return;

    // Build device-specific stylesheet for the phone bezel frame
    QString frameStyle;
    int maxWidth;

    switch (m_deviceType) {
    case services::DeviceType::Watch:
        // Small, highly rounded (nearly circular) for watches
        maxWidth = 280;
        frameStyle = R"(
            QFrame {
                background-color: #1F2937;
                border-radius: 50px;
                border: 4px solid #374151;
            }
        )";
        break;
    case services::DeviceType::Tablet:
        // Wider, less rounded for tablets
        maxWidth = 700;
        frameStyle = R"(
            QFrame {
                background-color: #1F2937;
                border-radius: 18px;
                border: 4px solid #374151;
            }
        )";
        break;
    case services::DeviceType::Phone:
    default:
        // Tall, moderately rounded for phones
        maxWidth = 340;
        frameStyle = R"(
            QFrame {
                background-color: #1F2937;
                border-radius: 28px;
                border: 3px solid #374151;
            }
        )";
        break;
    }

    m_phoneFrame->setMaximumWidth(maxWidth);
    m_phoneFrame->setStyleSheet(frameStyle);

    // Set min/max dimensions directly on the phone frame.
    // The max height will be updated dynamically in resizeEvent().
    int minW, minH;
    switch (m_deviceType) {
    case services::DeviceType::Watch:
        minW = 180; minH = 200; break;
    case services::DeviceType::Tablet:
        minW = 380; minH = 280; break;
    case services::DeviceType::Phone:
    default:
        minW = 260; minH = 400; break;
    }
    m_phoneFrame->setMinimumSize(minW, minH);

    // Adjust inner screen placeholder size and border-radius to match device type
    if (m_phoneScreen) {
        int innerRadius = (m_deviceType == services::DeviceType::Watch) ? 46
                        : (m_deviceType == services::DeviceType::Tablet) ? 14
                        : 22;
        int scrMinW = (m_deviceType == services::DeviceType::Watch) ? 180
                    : (m_deviceType == services::DeviceType::Tablet) ? 380
                    : 260;
        int scrMinH = (m_deviceType == services::DeviceType::Watch) ? 180
                    : (m_deviceType == services::DeviceType::Tablet) ? 260
                    : 400;
        m_phoneScreen->setMinimumSize(scrMinW, scrMinH);
        m_phoneScreen->setStyleSheet(QString(R"(
            background-color: #111827;
            border-radius: %1px;
            color: #6B7280;
            font-size: 14px;
        )").arg(innerRadius));
    }

    // Notify scrcpy to re-fit after shape change
    m_scrcpy->updateEmbedGeometry();
}

void DevicePanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // Keep the phone frame within the wrapper's bounds.
    // The maximum height is capped at the wrapper height so the bottom
    // panel (Log/Terminal/Results, 180px) remains visible.
    // The frame's minimum height ensures it doesn't collapse.
    // Scrcpy renders the device content at its native aspect ratio inside
    // the frame — letterboxing (black bars) may appear if aspect ratios differ,
    // which is normal and expected behavior (like a video player).
    if (m_phoneFrame) {
        QWidget *wrapper = m_phoneFrame->parentWidget();
        if (wrapper && wrapper->height() > 0) {
            int minHeight = (m_deviceType == services::DeviceType::Watch) ? 240
                          : (m_deviceType == services::DeviceType::Tablet) ? 350
                          : 450;
            m_phoneFrame->setMinimumHeight(minHeight);
            m_phoneFrame->setMaximumHeight(wrapper->height());
        }
    }

    m_scrcpy->updateEmbedGeometry();
}
