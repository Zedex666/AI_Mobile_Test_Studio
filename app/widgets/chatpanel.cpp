#include "chatpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>
#include <QFrame>
#include <QScrollBar>
#include <QDateTime>

ChatPanel::ChatPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupStyle();
    addWelcomeMessage();

    // Add demo task items
    addTaskItem(tr("1. 检查蓝牙是否开启"), tr("已完成"));
    addTaskItem(tr("2. 搜索可用设备"), tr("执行中..."));
    addTaskItem(tr("3. 连接目标设备"), tr("等待中"));
    addTaskItem(tr("4. 验证连接状态"), tr("等待中"));
    addTaskItem(tr("5. 断开连接"), tr("等待中"));

    addFileAttachment(tr("蓝牙测试用例.xlsx"), tr("12.4 KB"));
    addFileAttachment(tr("蓝牙测试需求文档.pdf"), tr("256 KB"));
}

void ChatPanel::setupUI()
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Chat list area
    m_chatList = new QListWidget(this);
    m_chatList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_chatList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_chatList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_chatList->verticalScrollBar()->setSingleStep(12);

    mainLayout->addWidget(m_chatList, 1);

    // Input area
    m_inputArea = new QWidget(this);
    m_inputArea->setFixedHeight(120);
    m_inputArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout *inputLayout = new QVBoxLayout(m_inputArea);
    inputLayout->setContentsMargins(16, 12, 16, 12);
    inputLayout->setSpacing(8);

    // Text input
    m_inputEdit = new QTextEdit(m_inputArea);
    m_inputEdit->setPlaceholderText(tr("输入你的需求或问题，支持拖拽文件，回车发送，Shift+Enter 换行"));
    m_inputEdit->setFixedHeight(56);
    m_inputEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Bottom toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    toolbarLayout->setSpacing(8);

    m_attachBtn = new QPushButton(tr("+"), m_inputArea);
    m_attachBtn->setFixedSize(28, 28);
    m_attachBtn->setToolTip(tr("添加"));

    m_fileBtn = new QPushButton(tr("📁"), m_inputArea);
    m_fileBtn->setFixedSize(28, 28);
    m_fileBtn->setToolTip(tr("文件"));

    m_codeBtn = new QPushButton(tr("</>"), m_inputArea);
    m_codeBtn->setFixedSize(28, 28);
    m_codeBtn->setToolTip(tr("代码"));

    m_mentionBtn = new QPushButton(tr("@"), m_inputArea);
    m_mentionBtn->setFixedSize(28, 28);
    m_mentionBtn->setToolTip(tr("提及"));

    toolbarLayout->addWidget(m_attachBtn);
    toolbarLayout->addWidget(m_fileBtn);
    toolbarLayout->addWidget(m_codeBtn);
    toolbarLayout->addWidget(m_mentionBtn);
    toolbarLayout->addStretch();

    m_sendBtn = new QPushButton(tr("▶"), m_inputArea);
    m_sendBtn->setFixedSize(36, 28);
    m_sendBtn->setToolTip(tr("发送"));
    toolbarLayout->addWidget(m_sendBtn);

    inputLayout->addWidget(m_inputEdit);
    inputLayout->addLayout(toolbarLayout);

    mainLayout->addWidget(m_inputArea, 0);

    // Connections
    connect(m_sendBtn, &QPushButton::clicked, this, [this]() {
        QString text = m_inputEdit->toPlainText().trimmed();
        if (!text.isEmpty()) {
            emit sendMessage(text);
            m_inputEdit->clear();
        }
    });
    connect(m_fileBtn, &QPushButton::clicked, this, &ChatPanel::attachFile);
}

void ChatPanel::setupStyle()
{
    setStyleSheet(R"(
        ChatPanel {
            background-color: #FFFFFF;
        }
        QListWidget {
            background-color: #FFFFFF;
            border: none;
            outline: none;
        }
        QListWidget::item {
            background: transparent;
            border: none;
            padding: 4px 8px;
        }
        QTextEdit {
            background-color: #F9FAFB;
            border: 1px solid #E5E7EB;
            border-radius: 10px;
            padding: 8px 12px;
            font-size: 13px;
            color: #1F2937;
        }
        QTextEdit:focus {
            border-color: #4F6EF7;
            background-color: #FFFFFF;
        }
        QPushButton {
            background-color: transparent;
            border: none;
            border-radius: 6px;
            font-size: 13px;
            color: #9CA3AF;
        }
        QPushButton:hover {
            background-color: #F3F4F6;
            color: #6B7280;
        }
        QPushButton#sendBtn {
            background-color: #4F6EF7;
            color: #FFFFFF;
            border-radius: 8px;
            font-size: 12px;
        }
        QPushButton#sendBtn:hover {
            background-color: #3D5CE5;
        }
    )");

    m_sendBtn->setObjectName("sendBtn");
}

void ChatPanel::addWelcomeMessage()
{
    // Welcome header item
    QWidget *welcomeWidget = new QWidget(m_chatList);
    QHBoxLayout *layout = new QHBoxLayout(welcomeWidget);
    layout->setContentsMargins(16, 12, 16, 8);

    QLabel *iconLabel = new QLabel(welcomeWidget);
    iconLabel->setFixedSize(36, 36);
    iconLabel->setStyleSheet("background-color: #4F6EF7; border-radius: 18px;");
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setText("🤖");

    QLabel *titleLabel = new QLabel(tr("欢迎使用 AI 助手"), welcomeWidget);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: 600; color: #1F2937;");

    layout->addWidget(iconLabel);
    layout->addWidget(titleLabel);
    layout->addStretch();

    QListWidgetItem *item = new QListWidgetItem(m_chatList);
    item->setSizeHint(welcomeWidget->sizeHint());
    m_chatList->addItem(item);
    m_chatList->setItemWidget(item, welcomeWidget);

    // AI greeting message
    QWidget *msgWidget = new QWidget(m_chatList);
    QHBoxLayout *msgLayout = new QHBoxLayout(msgWidget);
    msgLayout->setContentsMargins(16, 4, 64, 4);

    QLabel *msgLabel = new QLabel(tr("你好！我是你的 AI 测试助手，有什么可以帮助你的吗？"), msgWidget);
    msgLabel->setWordWrap(true);
    msgLabel->setStyleSheet(R"(
        background-color: #F3F4F6;
        border-radius: 12px;
        padding: 10px 14px;
        font-size: 13px;
        color: #374151;
    )");
    msgLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    msgLayout->addWidget(msgLabel);
    msgLayout->addStretch();

    QListWidgetItem *msgItem = new QListWidgetItem(m_chatList);
    m_chatList->addItem(msgItem);
    m_chatList->setItemWidget(msgItem, msgWidget);

    // User message demo
    QWidget *userWidget = new QWidget(m_chatList);
    QHBoxLayout *userLayout = new QHBoxLayout(userWidget);
    userLayout->setContentsMargins(64, 4, 16, 4);

    QLabel *userLabel = new QLabel(tr("请帮我测试蓝牙连接功能"), userWidget);
    userLabel->setWordWrap(true);
    userLabel->setStyleSheet(R"(
        background-color: #4F6EF7;
        border-radius: 12px;
        padding: 10px 14px;
        font-size: 13px;
        color: #FFFFFF;
    )");
    userLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    userLabel->setAlignment(Qt::AlignRight);

    userLayout->addStretch();
    userLayout->addWidget(userLabel);

    QListWidgetItem *userItem = new QListWidgetItem(m_chatList);
    m_chatList->addItem(userItem);
    m_chatList->setItemWidget(userItem, userWidget);

    // AI response with tasks
    QWidget *respWidget = new QWidget(m_chatList);
    QHBoxLayout *respLayout = new QHBoxLayout(respWidget);
    respLayout->setContentsMargins(16, 4, 64, 4);

    QLabel *respLabel = new QLabel(tr("好的，我将为你生成蓝牙连接测试脚本并开始执行。"), respWidget);
    respLabel->setWordWrap(true);
    respLabel->setStyleSheet(R"(
        background-color: #F3F4F6;
        border-radius: 12px;
        padding: 10px 14px;
        font-size: 13px;
        color: #374151;
    )");
    respLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    respLayout->addWidget(respLabel);
    respLayout->addStretch();

    QListWidgetItem *respItem = new QListWidgetItem(m_chatList);
    m_chatList->addItem(respItem);
    m_chatList->setItemWidget(respItem, respWidget);
}

void ChatPanel::addTaskItem(const QString &text, const QString &status)
{
    QWidget *taskWidget = new QWidget(m_chatList);
    QHBoxLayout *layout = new QHBoxLayout(taskWidget);
    layout->setContentsMargins(48, 2, 64, 2);

    QString icon = "○";
    QString color = "#9CA3AF";
    if (status == tr("已完成")) {
        icon = "✓";
        color = "#10B981";
    } else if (status == tr("执行中...")) {
        icon = "◐";
        color = "#4F6EF7";
    }

    QLabel *iconLabel = new QLabel(icon, taskWidget);
    iconLabel->setStyleSheet(QString("color: %1; font-size: 13px;").arg(color));
    iconLabel->setFixedWidth(20);

    QLabel *textLabel = new QLabel(text, taskWidget);
    textLabel->setStyleSheet("font-size: 13px; color: #4B5563;");

    QLabel *statusLabel = new QLabel(status, taskWidget);
    statusLabel->setStyleSheet(QString("font-size: 12px; color: %1;").arg(color));

    layout->addWidget(iconLabel);
    layout->addWidget(textLabel);
    layout->addStretch();
    layout->addWidget(statusLabel);

    QListWidgetItem *item = new QListWidgetItem(m_chatList);
    m_chatList->addItem(item);
    m_chatList->setItemWidget(item, taskWidget);
}

void ChatPanel::addFileAttachment(const QString &name, const QString &size)
{
    QWidget *fileWidget = new QWidget(m_chatList);
    QHBoxLayout *layout = new QHBoxLayout(fileWidget);
    layout->setContentsMargins(48, 4, 64, 4);

    QFrame *frame = new QFrame(fileWidget);
    frame->setStyleSheet(R"(
        QFrame {
            background-color: #F9FAFB;
            border: 1px solid #E5E7EB;
            border-radius: 8px;
            padding: 8px 12px;
        }
    )");

    QHBoxLayout *frameLayout = new QHBoxLayout(frame);
    frameLayout->setContentsMargins(8, 6, 8, 6);
    frameLayout->setSpacing(8);

    QString ext = name.split(".").last().toLower();
    QString fileIcon = "📄";
    if (ext == "xlsx" || ext == "xls") fileIcon = "📊";
    else if (ext == "pdf") fileIcon = "📕";

    QLabel *iconLabel = new QLabel(fileIcon, frame);
    iconLabel->setFixedSize(24, 24);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(2);

    QLabel *nameLabel = new QLabel(name, frame);
    nameLabel->setStyleSheet("font-size: 12px; font-weight: 500; color: #374151;");

    QLabel *sizeLabel = new QLabel(size, frame);
    sizeLabel->setStyleSheet("font-size: 11px; color: #9CA3AF;");

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(sizeLabel);

    QPushButton *closeBtn = new QPushButton("✕", frame);
    closeBtn->setFixedSize(20, 20);
    closeBtn->setStyleSheet("border: none; color: #9CA3AF; font-size: 11px;");

    frameLayout->addWidget(iconLabel);
    frameLayout->addLayout(infoLayout);
    frameLayout->addStretch();
    frameLayout->addWidget(closeBtn);

    layout->addWidget(frame);
    layout->addStretch();

    QListWidgetItem *item = new QListWidgetItem(m_chatList);
    m_chatList->addItem(item);
    m_chatList->setItemWidget(item, fileWidget);
}
