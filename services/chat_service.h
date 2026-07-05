#ifndef SERVICES_CHAT_SERVICE_H
#define SERVICES_CHAT_SERVICE_H

#include <QObject>
#include <QString>
#include <QList>

namespace services {

struct ChatMessage {
    enum Role {
        User,
        Assistant,
        System
    };
    Role role;
    QString content;
    QString timestamp;
    QList<QString> attachments;
};

class ChatService : public QObject
{
    Q_OBJECT

public:
    explicit ChatService(QObject *parent = nullptr);

    void sendMessage(const QString &content, const QList<QString> &attachments = {});
    void clearHistory();
    QList<ChatMessage> history() const;

signals:
    void messageReceived(const ChatMessage &message);
    void messageSent(const ChatMessage &message);
    void streamChunk(const QString &chunk);
    void error(const QString &error);

private:
    QList<ChatMessage> m_history;
};

} // namespace services

#endif // SERVICES_CHAT_SERVICE_H
