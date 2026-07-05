#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QLabel;
class TitleBar;
class NavigationBar;
class DevicePanel;
class ChatPanel;
class BottomPanel;
class QSplitter;

namespace services {
class DeviceService;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void setupServices();
    void setupStatusBar();
    void setupStyle();

    Ui::MainWindow *ui;

    TitleBar *m_titleBar;
    NavigationBar *m_navBar;
    DevicePanel *m_devicePanel;
    ChatPanel *m_chatPanel;
    BottomPanel *m_bottomPanel;

    QSplitter *m_mainSplitter;
    QSplitter *m_centerSplitter;

    services::DeviceService *m_deviceService = nullptr;
    QLabel *m_adbStatusLabel = nullptr;
    QLabel *m_deviceInfoLabel = nullptr;
};
#endif // MAINWINDOW_H
