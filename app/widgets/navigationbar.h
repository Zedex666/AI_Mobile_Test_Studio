#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include <QWidget>

class QButtonGroup;

class NavigationBar : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationBar(QWidget *parent = nullptr);

    enum NavItem {
        Chat,
        ChatList,
        Cases,
        Plugins,
        Skills,
        Settings,
        About
    };

signals:
    void itemClicked(NavItem item);

private:
    void setupUI();
    void setupStyle();
    void createNavItem(const QString &icon, const QString &text, NavItem item);

    QButtonGroup *m_buttonGroup;
};

#endif // NAVIGATIONBAR_H
