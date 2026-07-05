#include "chat_service.h"
#include <QDateTime>

namespace services {

ChatService::ChatService(QObject *parent)
    : QObject(parent)
{
}

void ChatService::sendMessage(const QString &content, const QList<QString> &attachments)
{
    ChatMessage msg;
    msg.role = ChatMessage::User;
    msg.content = content;
    msg.timestamp = QDateTime::currentDateTime().toString("hh:mm");
    msg.attachments = attachments;

    m_history.append(msg);
    emit messageSent(msg);

    // TODO: Integrate with LLM provider to get AI response
    ChatMessage reply;
    reply.role = ChatMessage::Assistant;
    reply.content = tr("收到消息: %1").arg(content);
    reply.timestamp = QDateTime::currentDateTime().toString("hh:mm");

    m_history.append(reply);
    emit messageReceived(reply);
}

void ChatService::clearHistory()
{
    m_history.clear();
}

QList<ChatMessage> ChatService::history() const
{
    return m_history;
}

} // namespace services
