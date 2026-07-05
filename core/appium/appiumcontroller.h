#ifndef CORE_APPIUM_APPIUMCONTROLLER_H
#define CORE_APPIUM_APPIUMCONTROLLER_H

#include <QObject>
#include <QString>

namespace core {
namespace appium {

class AppiumController : public QObject
{
    Q_OBJECT

public:
    explicit AppiumController(QObject *parent = nullptr);

    bool startServer(const QString &host = "127.0.0.1", int port = 4723);
    bool stopServer();
    bool isRunning() const;

    bool createSession(const QString &deviceId, const QString &appPackage = QString(),
                       const QString &appActivity = QString());
    bool closeSession();
    bool isSessionActive() const;

    bool click(int x, int y);
    bool sendKeys(const QString &text);
    bool swipe(int startX, int startY, int endX, int endY, int durationMs = 300);
    QString getPageSource() const;
    QString takeScreenshot(const QString &savePath = QString()) const;

signals:
    void serverStarted();
    void serverStopped();
    void sessionCreated(const QString &sessionId);
    void sessionClosed();
    void error(const QString &message);

private:
    bool m_serverRunning = false;
    bool m_sessionActive = false;
    QString m_sessionId;
};

} // namespace appium
} // namespace core

#endif // CORE_APPIUM_APPIUMCONTROLLER_H
