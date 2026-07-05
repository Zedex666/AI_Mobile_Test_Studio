#include "plugin_service.h"
#include <QDir>

namespace services {

PluginService::PluginService(QObject *parent)
    : QObject(parent)
{
}

void PluginService::loadPlugins()
{
    QDir pluginsDir("plugins");
    if (!pluginsDir.exists()) return;

    // TODO: Scan plugin directories and load valid plugins
    for (const QString &dirName : pluginsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        PluginInfo info;
        info.id = dirName;
        info.name = dirName;
        info.isEnabled = true;
        info.path = pluginsDir.absoluteFilePath(dirName);
        m_plugins.insert(info.id, info);
        emit pluginLoaded(info);
    }
}

void PluginService::unloadPlugins()
{
    for (const QString &id : m_plugins.keys()) {
        emit pluginUnloaded(id);
    }
    m_plugins.clear();
}

QList<PluginInfo> PluginService::plugins() const
{
    return m_plugins.values();
}

bool PluginService::enablePlugin(const QString &id)
{
    if (!m_plugins.contains(id)) return false;
    m_plugins[id].isEnabled = true;
    return true;
}

bool PluginService::disablePlugin(const QString &id)
{
    if (!m_plugins.contains(id)) return false;
    m_plugins[id].isEnabled = false;
    return true;
}

} // namespace services
