#ifndef CORE_CONFIG_CONFIGMANAGER_H
#define CORE_CONFIG_CONFIGMANAGER_H

#include <QObject>
#include <QString>
#include <QJsonObject>

namespace core {
namespace config {

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    static ConfigManager* instance();

    bool load(const QString &filePath);
    bool loadDefault();

    QString adbPath() const;
    QString scrcpyPath() const;
    QString pythonPath() const;
    QString nodePath() const;
    QString ffmpegPath() const;
    QString appiumPath() const;

    QString defaultLlmProvider() const;
    QJsonObject llmProviderConfig(const QString &provider) const;

    QString appiumServerUrl() const;
    bool appiumAutoStart() const;

    QString workspacePath() const;
    QString logsPath() const;

    QJsonObject rawConfig() const;

private:
    explicit ConfigManager(QObject *parent = nullptr);

    static ConfigManager *s_instance;
    QJsonObject m_config;
    QString m_configPath;

    QString findInPath(const QString &name) const;
    QString findScrcpy() const;
    QString findAdb() const;
    QString findAppium() const;
};

} // namespace config
} // namespace core

#endif // CORE_CONFIG_CONFIGMANAGER_H
