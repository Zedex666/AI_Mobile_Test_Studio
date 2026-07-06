#include "bottompanel.h"
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QLabel>

BottomPanel::BottomPanel(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupStyle();
}

void BottomPanel::setupUI()
{
    setMinimumHeight(160);
    setMaximumHeight(400);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Log tab
    m_logEdit = new QTextEdit(this);
    m_logEdit->setReadOnly(true);
    m_logEdit->setPlaceholderText(tr("日志输出..."));
    m_tabWidget->addTab(m_logEdit, tr("📋 日志"));

    // Terminal tab
    m_terminalEdit = new QTextEdit(this);
    m_terminalEdit->setReadOnly(true);
    m_terminalEdit->setPlaceholderText(tr("终端输出..."));
    m_tabWidget->addTab(m_terminalEdit, tr("💻 终端"));

    // Result tab
    m_resultEdit = new QTextEdit(this);
    m_resultEdit->setReadOnly(true);
    m_resultEdit->setPlaceholderText(tr("运行结果..."));
    m_tabWidget->addTab(m_resultEdit, tr("📊 运行结果"));

    mainLayout->addWidget(m_tabWidget);
}

void BottomPanel::setupStyle()
{
    setStyleSheet(R"(
        BottomPanel {
            background-color: #FFFFFF;
            border-top: 1px solid #E5E7EB;
        }
        QTabWidget::pane {
            border: none;
            background-color: #FFFFFF;
        }
        QTabBar::tab {
            background-color: #F9FAFB;
            border: none;
            border-bottom: 2px solid transparent;
            padding: 8px 16px;
            font-size: 12px;
            color: #6B7280;
        }
        QTabBar::tab:selected {
            background-color: #FFFFFF;
            border-bottom: 2px solid #4F6EF7;
            color: #4F6EF7;
            font-weight: 500;
        }
        QTabBar::tab:hover:!selected {
            background-color: #F3F4F6;
            color: #374151;
        }
        QTextEdit {
            background-color: #FFFFFF;
            border: none;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 12px;
            color: #374151;
            padding: 8px;
        }
    )");
}

void BottomPanel::appendLog(const QString &text)
{
    m_logEdit->append(text);
}

void BottomPanel::appendTerminal(const QString &text)
{
    m_terminalEdit->append(text);
}

void BottomPanel::appendResult(const QString &text)
{
    m_resultEdit->append(text);
}

void BottomPanel::setActiveTab(TabIndex tab)
{
    m_tabWidget->setCurrentIndex(static_cast<int>(tab));
}

void BottomPanel::onLogEmitted(const QString &text)
{
    m_logEdit->append(text);
}
