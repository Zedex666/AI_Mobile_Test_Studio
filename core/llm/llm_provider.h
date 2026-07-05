#ifndef CORE_LLM_LLM_PROVIDER_H
#define CORE_LLM_LLM_PROVIDER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QJsonObject>

namespace core {
namespace llm {

struct LLMConfig {
    QString name;
    QString apiUrl;
    QString apiKey;
    QString model;
    QMap<QString, QString> extraParams;
};

class LLMProvider : public QObject
{
    Q_OBJECT

public:
    explicit LLMProvider(const LLMConfig &config, QObject *parent = nullptr);
    virtual ~LLMProvider();

    virtual bool isAvailable() const = 0;
    virtual void sendMessage(const QString &message, const QJsonObject &context = QJsonObject()) = 0;
    virtual void sendMessageWithImage(const QString &message, const QString &imagePath, const QJsonObject &context = QJsonObject()) = 0;
    virtual void streamMessage(const QString &message, const QJsonObject &context = QJsonObject()) = 0;

    QString name() const { return m_config.name; }
    QString model() const { return m_config.model; }

signals:
    void responseReceived(const QString &response);
    void streamChunkReceived(const QString &chunk);
    void errorOccurred(const QString &error);

protected:
    LLMConfig m_config;
};

} // namespace llm
} // namespace core

#endif // CORE_LLM_LLM_PROVIDER_H
