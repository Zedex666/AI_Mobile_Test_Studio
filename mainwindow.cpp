#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "app/widgets/titlebar.h"
#include "app/widgets/navigationbar.h"
#include "app/widgets/devicepanel.h"
#include "app/widgets/chatpanel.h"
#include "app/widgets/bottompanel.h"
#include "core/logger/logger.h"
#include "core/config/configmanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>
#include <QStatusBar>
#include <QFrame>
#include <QMessageBox>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupUI();
    setupServices();
    setupStatusBar();
    setupStyle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle(tr("AI Mobile Test Studio"));
    resize(1440, 900);
    setMinimumSize(1100, 800);

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 1. Title bar
    m_titleBar = new TitleBar(this);
    mainLayout->addWidget(m_titleBar);

    // Separator
    QFrame *sep1 = new QFrame(this);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setStyleSheet("color: #E5E7EB;");
    sep1->setFixedHeight(1);
    mainLayout->addWidget(sep1);

    // 2. Center area: Navigation + Content
    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->setSpacing(0);
    centerLayout->setContentsMargins(0, 0, 0, 0);

    // Left: Navigation bar
    m_navBar = new NavigationBar(this);
    centerLayout->addWidget(m_navBar);

    // Separator
    QFrame *sepNav = new QFrame(this);
    sepNav->setFrameShape(QFrame::VLine);
    sepNav->setStyleSheet("color: #E5E7EB;");
    sepNav->setFixedWidth(1);
    centerLayout->addWidget(sepNav);

    // Center: Device + Chat (with splitter)
    m_centerSplitter = new QSplitter(Qt::Horizontal, this);
    m_centerSplitter->setHandleWidth(1);
    m_centerSplitter->setStyleSheet("QSplitter::handle { background-color: #E5E7EB; }");

    // Device panel
    m_devicePanel = new DevicePanel(this);
    m_devicePanel->setMinimumWidth(360);
    m_centerSplitter->addWidget(m_devicePanel);

    // Chat panel
    m_chatPanel = new ChatPanel(this);
    m_chatPanel->setMinimumWidth(400);
    m_centerSplitter->addWidget(m_chatPanel);

    // Set splitter sizes (device:chat = 45:55)
    m_centerSplitter->setSizes({650, 790});

    centerLayout->addWidget(m_centerSplitter, 1);

    mainLayout->addLayout(centerLayout, 1);

    // Separator
    QFrame *sep2 = new QFrame(this);
    sep2->setFrameShape(QFrame::HLine);
    sep2->setStyleSheet("color: #E5E7EB;");
    sep2->setFixedHeight(1);
    mainLayout->addWidget(sep2);

    // 3. Bottom panel
    m_bottomPanel = new BottomPanel(this);
    mainLayout->addWidget(m_bottomPanel);

    // Connections
    connect(m_titleBar, &TitleBar::refreshClicked, this, [this]() {
        if (m_deviceService) {
            m_deviceService->refreshDevices();
        }
    });
    connect(m_navBar, &NavigationBar::itemClicked, this, [](NavigationBar::NavItem item) {
        Q_UNUSED(item)
    });
    connect(m_chatPanel, &ChatPanel::sendMessage, this, [](const QString &msg) {
        Q_UNUSED(msg)
    });
}

void MainWindow::setupServices()
{
    // Load configuration
    core::config::ConfigManager::instance()->loadDefault();

    // Setup logger file output
    QString logDir = core::config::ConfigManager::instance()->logsPath();
    QDir().mkpath(logDir);
    QString logFile = QDir(logDir).filePath("app.log");
    core::logger::Logger::instance()->setLogFile(logFile);

    // Connect logger to bottom panel
    connect(core::logger::Logger::instance(), &core::logger::Logger::logEmitted,
            m_bottomPanel, &BottomPanel::onLogEmitted);

    // Device service
    m_deviceService = new services::DeviceService(this);

    // Connect device service to title bar
    connect(m_deviceService, &services::DeviceService::devicesChanged,
            m_titleBar, &TitleBar::setDevices);
    connect(m_deviceService, &services::DeviceService::currentDeviceChanged,
            m_titleBar, &TitleBar::setCurrentDevice);
    connect(m_deviceService, &services::DeviceService::adbStatusChanged,
            m_titleBar, &TitleBar::setAdbStatus);

    // Connect title bar device selection to device service
    connect(m_titleBar, &TitleBar::deviceSelected,
            m_deviceService, &services::DeviceService::setCurrentDevice);

    // Connect device service to device panel
    connect(m_deviceService, &services::DeviceService::currentDeviceChanged,
            this, [this](const QString &deviceId) {
        services::DeviceInfo info = m_deviceService->deviceInfo(deviceId);
        m_devicePanel->setDeviceInfo(info);
    });
    connect(m_deviceService, &services::DeviceService::adbStatusChanged,
            m_devicePanel, &DevicePanel::setAdbAvailable);

    // --- DevicePanel control button connections ---
    // Architecture: UI (DevicePanel) → MainWindow (coordination) → DeviceService → AdbController
    // All buttons target the currently selected device. If no device is selected,
    // the operation is silently skipped (sendKeyEvent checks for empty deviceId).

    // Refresh button — refresh device list and restart scrcpy
    connect(m_devicePanel, &DevicePanel::refreshClicked, this, [this]() {
        core::logger::Logger::instance()->info("MainWindow", tr("DevicePanel refresh clicked"));
        m_deviceService->refreshDevices();
        m_devicePanel->stopScrcpy();
        QString currentId = m_deviceService->currentDevice();
        if (!currentId.isEmpty()) {
            services::DeviceInfo info = m_deviceService->deviceInfo(currentId);
            if (info.isConnected) {
                m_devicePanel->setDeviceInfo(info);
            }
        }
    });

    // Back button — KEYCODE_BACK (4)
    connect(m_devicePanel, &DevicePanel::backClicked, this, [this]() {
        m_deviceService->sendKeyEvent(m_deviceService->currentDevice(), 4);
    });

    // Home button — KEYCODE_HOME (3)
    connect(m_devicePanel, &DevicePanel::homeClicked, this, [this]() {
        m_deviceService->sendKeyEvent(m_deviceService->currentDevice(), 3);
    });

    // Recents / App switch button — KEYCODE_APP_SWITCH (187)
    connect(m_devicePanel, &DevicePanel::recentClicked, this, [this]() {
        m_deviceService->sendKeyEvent(m_deviceService->currentDevice(), 187);
    });

    // Rotate button — toggle between portrait and landscape
    connect(m_devicePanel, &DevicePanel::rotateClicked, this, [this]() {
        m_deviceService->toggleRotation(m_deviceService->currentDevice());
    });

    // Volume Up button — KEYCODE_VOLUME_UP (24)
    connect(m_devicePanel, &DevicePanel::volumeUpClicked, this, [this]() {
        m_deviceService->sendKeyEvent(m_deviceService->currentDevice(), 24);
    });

    // Volume Down button — KEYCODE_VOLUME_DOWN (25)
    connect(m_devicePanel, &DevicePanel::volumeDownClicked, this, [this]() {
        m_deviceService->sendKeyEvent(m_deviceService->currentDevice(), 25);
    });

    // Start polling
    m_deviceService->startPolling(3000);

    // IMPORTANT: trigger an immediate refresh AFTER all signals are connected
    // so that already-connected devices are detected and auto-start scrcpy
    m_deviceService->refreshDevices();

    core::logger::Logger::instance()->info("MainWindow", tr("Services initialized, device polling started"));
}

void MainWindow::setupStatusBar()
{
    QStatusBar *status = statusBar();
    status->setFixedHeight(28);
    status->setStyleSheet(R"(
        QStatusBar {
            background-color: #F9FAFB;
            border-top: 1px solid #E5E7EB;
            font-size: 12px;
            color: #6B7280;
        }
        QLabel {
            font-size: 12px;
            color: #6B7280;
            padding: 0 12px;
        }
    )");

    m_adbStatusLabel = new QLabel(tr("● ADB 检测中..."), this);
    m_adbStatusLabel->setStyleSheet("color: #9CA3AF; font-size: 12px;");
    status->addWidget(m_adbStatusLabel);

    m_deviceInfoLabel = new QLabel(tr("未连接设备"), this);
    status->addWidget(m_deviceInfoLabel);

    status->addPermanentWidget(new QLabel(tr("AMTS v0.2"), this));

    // Update status bar based on device service
    connect(m_deviceService, &services::DeviceService::adbStatusChanged,
            this, [this](bool available) {
        if (available) {
            m_adbStatusLabel->setText(tr("● ADB 连接正常"));
            m_adbStatusLabel->setStyleSheet("color: #10B981; font-size: 12px;");
        } else {
            m_adbStatusLabel->setText(tr("● ADB 未找到"));
            m_adbStatusLabel->setStyleSheet("color: #EF4444; font-size: 12px;");
        }
    });

    connect(m_deviceService, &services::DeviceService::currentDeviceChanged,
            this, [this](const QString &deviceId) {
        services::DeviceInfo info = m_deviceService->deviceInfo(deviceId);
        if (!info.id.isEmpty()) {
            m_deviceInfoLabel->setText(tr("Android %1  |  电量 %2%  |  %3")
                .arg(info.androidVersion)
                .arg(info.batteryLevel)
                .arg(info.isEmulator ? tr("模拟器") : tr("真机")));
        } else {
            m_deviceInfoLabel->setText(tr("未连接设备"));
        }
    });
}

void MainWindow::setupStyle()
{
    setStyleSheet(R"(
        MainWindow {
            background-color: #FFFFFF;
        }
    )");
}
