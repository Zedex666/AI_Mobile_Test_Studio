#include "configmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QStandardPaths>
#include <QDir>
#include <QProcessEnvironment>
#include <QDebug>
#include <QCoreApplication>

namespace core {
namespace config {

ConfigManager* ConfigManager::s_instance = nullptr;

ConfigManager* ConfigManager::instance()
{
    if (!s_instance) {
        s_instance = new ConfigManager();
        s_instance->loadDefault();
    }
    return s_instance;
}

ConfigManager::ConfigManager(QObject *parent)
    : QObject(parent)
{
}

bool ConfigManager::load(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "ConfigManager: Failed to open" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "ConfigManager: Invalid JSON in" << filePath;
        return false;
    }

    m_config = doc.object();
    m_configPath = filePath;
    return true;
}

bool ConfigManager::loadDefault()
{
    QString configPath = QDir(QCoreApplication::applicationDirPath()).filePath("config/settings.json");
    if (QFile::exists(configPath)) {
        return load(configPath);
    }
    // Fallback to source directory for development
    configPath = QDir(QCoreApplication::applicationDirPath()).filePath("../config/settings.json");
    if (QFile::exists(configPath)) {
        return load(configPath);
    }
    qWarning() << "ConfigManager: settings.json not found";
    return false;
}

QString ConfigManager::adbPath() const
{
    QJsonObject paths = m_config.value("paths").toObject();
    QString custom = paths.value("adb").toString();
    if (!custom.isEmpty() && QFile::exists(custom)) return custom;
    return findAdb();
}

QString ConfigManager::scrcpyPath() const
{
    QJsonObject paths = m_config.value("paths").toObject();
    QString custom = paths.value("scrcpy").toString();
    if (!custom.isEmpty() && QFile::exists(custom)) return custom;
    return findScrcpy();
}

QString ConfigManager::pythonPath() const
{
    QJsonObject paths = m_config.value("paths").toObject();
    QString custom = paths.value("python").toString();
    if (!custom.isEmpty() && QFile::exists(custom)) return custom;
    return findInPath("python.exe");
}

QString ConfigManager::nodePath() const
{
    QJsonObject paths = m_config.value("paths").toObject();
    QString custom = paths.value("node").toString();
    if (!custom.isEmpty() && QFile::exists(custom)) return custom;
    return findInPath("node.exe");
}

QString ConfigManager::ffmpegPath() const
{
    QJsonObject paths = m_config.value("paths").toObject();
    QString custom = paths.value("ffmpeg").toString();
    if (!custom.isEmpty() && QFile::exists(custom)) return custom;
    return findInPath("ffmpeg.exe");
}

QString ConfigManager::appiumPath() const
{
    QJsonObject paths = m_config.value("paths").toObject();
    QString custom = paths.value("appium").toString();
    if (!custom.isEmpty() && QFile::exists(custom)) return custom;
    return findAppium();
}

QString ConfigManager::defaultLlmProvider() const
{
    QJsonObject llm = m_config.value("llm").toObject();
    return llm.value("default_provider").toString("openai");
}

QJsonObject ConfigManager::llmProviderConfig(const QString &provider) const
{
    QJsonObject llm = m_config.value("llm").toObject();
    QJsonObject providers = llm.value("providers").toObject();
    return providers.value(provider).toObject();
}

QString ConfigManager::appiumServerUrl() const
{
    QJsonObject appium = m_config.value("appium").toObject();
    return appium.value("server_url").toString("http://localhost:4723");
}

bool ConfigManager::appiumAutoStart() const
{
    QJsonObject appium = m_config.value("appium").toObject();
    return appium.value("auto_start_server").toBool(true);
}

QString ConfigManager::workspacePath() const
{
    QJsonObject paths = m_config.value("paths").toObject();
    QString p = paths.value("workspace").toString("./workspace");
    return QDir(p).absolutePath();
}

QString ConfigManager::logsPath() const
{
    QJsonObject paths = m_config.value("paths").toObject();
    QString p = paths.value("logs").toString("./logs");
    return QDir(p).absolutePath();
}

QJsonObject ConfigManager::rawConfig() const
{
    return m_config;
}

QString ConfigManager::findInPath(const QString &name) const
{
    QStringList paths = QProcessEnvironment::systemEnvironment().value("PATH").split(";");
    for (const QString &dir : paths) {
        QString fullPath = QDir(dir).filePath(name);
        if (QFile::exists(fullPath)) {
            return fullPath;
        }
    }
    return QString();
}

QString ConfigManager::findScrcpy() const
{
    // Check common installation paths
    QStringList candidates = {
        findInPath("scrcpy.exe"),
        QDir::homePath() + "/scoop/apps/scrcpy/current/scrcpy.exe",
        "C:/Program Files/scrcpy/scrcpy.exe",
        "C:/Program Files (x86)/scrcpy/scrcpy.exe",
        QCoreApplication::applicationDirPath() + "/sdk/scrcpy/scrcpy.exe",
    };
    for (const QString &path : candidates) {
        if (!path.isEmpty() && QFile::exists(path)) return path;
    }
    return QString();
}

QString ConfigManager::findAdb() const
{
    QStringList candidates = {
        findInPath("adb.exe"),
        QDir::homePath() + "/AppData/Local/Android/Sdk/platform-tools/adb.exe",
        "C:/Program Files/Android/platform-tools/adb.exe",
        QCoreApplication::applicationDirPath() + "/sdk/adb/adb.exe",
    };
    for (const QString &path : candidates) {
        if (!path.isEmpty() && QFile::exists(path)) return path;
    }
    return QString();
}

QString ConfigManager::findAppium() const
{
    QString node = nodePath();
    if (node.isEmpty()) return QString();

    // Try npx appium or global appium
    QStringList candidates = {
        findInPath("appium.cmd"),
        findInPath("appium.bat"),
        QDir::homePath() + "/AppData/Roaming/npm/appium.cmd",
        "C:/Program Files/nodejs/appium.cmd",
    };
    for (const QString &path : candidates) {
        if (!path.isEmpty() && QFile::exists(path)) return path;
    }
    return QString();
}

} // namespace config
} // namespace core
