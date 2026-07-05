#ifndef SERVICES_PLUGIN_SERVICE_H
#define SERVICES_PLUGIN_SERVICE_H

#include <QObject>
#include <QString>
#include <QMap>

namespace services {

struct PluginInfo {
    QString id;
    QString name;
    QString version;
    QString description;
    QString author;
    bool isEnabled;
    QString path;
};

class PluginService : public QObject
{
    Q_OBJECT

public:
    explicit PluginService(QObject *parent = nullptr);

    void loadPlugins();
    void unloadPlugins();
    QList<PluginInfo> plugins() const;
    bool enablePlugin(const QString &id);
    bool disablePlugin(const QString &id);

signals:
    void pluginLoaded(const PluginInfo &info);
    void pluginUnloaded(const QString &id);
    void pluginError(const QString &id, const QString &error);

private:
    QMap<QString, PluginInfo> m_plugins;
};

} // namespace services

#endif // SERVICES_PLUGIN_SERVICE_H
