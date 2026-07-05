#include "navigationbar.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>
#include <QSpacerItem>

NavigationBar::NavigationBar(QWidget *parent)
    : QWidget(parent)
    , m_buttonGroup(new QButtonGroup(this))
{
    setupUI();
    setupStyle();
}

void NavigationBar::setupUI()
{
    setFixedWidth(64);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 16, 8, 16);
    mainLayout->setSpacing(4);
    mainLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    // Top spacer for visual balance
    mainLayout->addSpacing(8);

    // Create nav items
    createNavItem("💬", tr("对话"), Chat);
    createNavItem("☰", tr("对话列表"), ChatList);
    createNavItem("📋", tr("对案"), Cases);
    createNavItem("🧩", tr("插件"), Plugins);
    createNavItem("🛠", tr("技能"), Skills);

    mainLayout->addStretch();

    createNavItem("⚙", tr("设置"), Settings);
    createNavItem("ⓘ", tr("关于"), About);

    m_buttonGroup->setExclusive(true);
}

void NavigationBar::createNavItem(const QString &icon, const QString &text, NavItem item)
{
    QPushButton *btn = new QPushButton(this);
    btn->setCheckable(true);
    btn->setFixedSize(48, 48);
    btn->setText(icon);
    btn->setToolTip(text);
    btn->setProperty("navItem", item);

    if (item == Chat) {
        btn->setChecked(true);
    }

    m_buttonGroup->addButton(btn, item);

    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->layout());
    if (layout) {
        layout->addWidget(btn, 0, Qt::AlignHCenter);
    }

    connect(btn, &QPushButton::clicked, this, [this, item]() {
        emit itemClicked(item);
    });
}

void NavigationBar::setupStyle()
{
    setStyleSheet(R"(
        NavigationBar {
            background-color: #F9FAFB;
            border-right: 1px solid #E5E7EB;
        }
        QPushButton {
            background-color: transparent;
            border: none;
            border-radius: 10px;
            font-size: 18px;
            color: #9CA3AF;
        }
        QPushButton:hover {
            background-color: #F3F4F6;
            color: #6B7280;
        }
        QPushButton:checked {
            background-color: #EFF6FF;
            color: #4F6EF7;
        }
        QPushButton:pressed {
            background-color: #DBEAFE;
        }
    )");
}
