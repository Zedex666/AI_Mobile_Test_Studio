
# PLUGIN_SPEC.md

> AI Mobile Test Studio (AMTS) Plugin Development Specification

Version: 1.0

---

# 1. 文档目的

本文档定义了 AMTS 插件系统的开发规范。

所有插件（官方插件、第三方插件、企业插件）必须遵循本规范。

目标：

- 插件化
- 热插拔
- 低耦合
- 易维护
- 易扩展
- AI 易理解

---

# 2. 插件架构

整个插件系统采用：

```
MainWindow

↓

PluginManager

↓

PluginLoader

↓

PluginInterface

↓

BluetoothPlugin

WiFiPlugin

OCRPlugin

VisionPlugin

PerformancePlugin

...
```

MainWindow 永远不知道插件具体实现。

所有插件统一通过 PluginManager 管理。

---

# 3. 插件目录

所有插件统一放入：

```
plugins/

```

例如：

```
plugins/

Bluetooth/

WiFi/

OCR/

Vision/

Performance/

Monkey/

Camera/

Custom/
```

每个插件：

一个目录。

---

# 4. 插件目录结构

例如：

```
Bluetooth/

├── plugin.json
├── BluetoothPlugin.h
├── BluetoothPlugin.cpp
├── BluetoothWidget.ui
├── BluetoothWidget.cpp
├── BluetoothWidget.h
├── icons/
└── resources/
```

禁止：

多个插件：

混放。

---

# 5. plugin.json

每个插件：

必须：

包含：

plugin.json

例如：

```json
{
    "id":"bluetooth",

    "name":"Bluetooth Plugin",

    "version":"1.0.0",

    "author":"AMTS",

    "description":"Bluetooth Automation",

    "entry":"BluetoothPlugin",

    "category":"Device",

    "icon":"icon.png"
}
```

PluginManager：

启动时：

自动读取。

---

# 6. PluginInterface

所有插件：

必须：

继承：

```cpp
class PluginInterface
{
public:

    virtual QString id() const = 0;

    virtual QString name() const = 0;

    virtual QString version() const = 0;

    virtual QString description() const = 0;

    virtual QWidget* widget() = 0;

    virtual bool initialize() = 0;

    virtual void shutdown() = 0;

    virtual void onProjectOpened() {}

    virtual void onProjectClosed() {}

    virtual ~PluginInterface() = default;
};
```

禁止：

自定义：

插件接口。

---

# 7. 生命周期

插件生命周期：

```
加载

↓

创建

↓

初始化

↓

运行

↓

关闭

↓

释放
```

PluginManager：

统一：

管理。

---

# 8. PluginManager

PluginManager：

负责：

```
扫描插件

加载插件

卸载插件

初始化

热更新

状态

依赖管理
```

禁止：

MainWindow：

直接：

加载插件。

---

# 9. PluginLoader

PluginLoader：

负责：

```
读取plugin.json

↓

加载DLL

↓

创建实例

↓

返回PluginInterface
```

UI：

不参与。

---

# 10. 插件分类

插件分类：

```
Device

Automation

Vision

OCR

Performance

AI

Utility

Report

Enterprise
```

未来：

支持：

Marketplace。

---

# 11. Widget

如果插件：

需要：

界面。

统一：

返回：

```
QWidget*
```

例如：

```
BluetoothWidget

WiFiWidget

OCRWidget
```

不要：

操作：

MainWindow。

---

# 12. Service

插件：

如果：

需要：

ADB。

统一：

调用：

DeviceService。

不要：

自己：

执行：

adb.exe。

---

# 13. 日志

插件：

统一：

Logger。

例如：

```
LOG_INFO()

LOG_WARNING()

LOG_ERROR()
```

禁止：

printf()

---

# 14. 配置

插件：

自己的配置：

放：

```
workspace/

config/

plugin/

```

例如：

```
bluetooth.json

wifi.json
```

不要：

写死。

---

# 15. 资源

插件：

资源：

统一：

```
resources/

icons/

images/

fonts/
```

不要：

放：

项目根目录。

---

# 16. 数据通信

插件：

与：

主程序：

通过：

Signal / Slot。

或者：

Service。

禁止：

直接：

调用：

MainWindow。

---

# 17. 插件权限

插件：

允许：

```
读取设备

调用ADB

调用AI

调用Python

调用OCR
```

禁止：

直接：

访问：

数据库。

系统配置。

未来：

增加：

权限系统。

---

# 18. AI 插件

AI：

也是：

插件。

例如：

```
OpenAI Plugin

Claude Plugin

DeepSeek Plugin
```

统一：

实现：

PluginInterface。

---

# 19. Device 插件

例如：

```
Bluetooth

WiFi

USB

Sensor

GPS

Camera
```

全部：

插件。

不要：

写：

Core。

---

# 20. Report 插件

例如：

```
HTML

Markdown

PDF

Excel
```

全部：

插件。

方便：

以后：

增加：

Word。

---

# 21. OCR 插件

例如：

```
PaddleOCR

Tesseract

EasyOCR
```

统一：

OCRPlugin。

用户：

自由切换。

---

# 22. Vision 插件

例如：

```
YOLO

OpenCV

SAM

GroundingDINO
```

全部：

插件。

---

# 23. Monkey 插件

例如：

```
Monkey

MonkeyRunner

Random Click
```

未来：

增加：

AI Monkey。

---

# 24. 热插拔

PluginManager：

支持：

```
加载

↓

卸载

↓

重新加载
```

无需：

重启软件。

---

# 25. 插件依赖

支持：

```
Bluetooth

↓

依赖：

ADB
```

PluginManager：

自动：

检查。

---

# 26. 插件版本

PluginManager：

检查：

```
最低版本

当前版本

兼容版本
```

避免：

接口变化。

---

# 27. 插件市场（未来）

未来：

支持：

Plugin Marketplace。

例如：

```
下载

安装

更新

评分

卸载
```

全部：

在线完成。

---

# 28. AI 开发规范

新增插件：

必须：

独立目录。

实现：

PluginInterface。

提供：

plugin.json。

不得：

修改：

MainWindow。

不得：

直接：

调用：

UI。

通过：

Service。

同步：

文档。

---

# 29. 示例

BluetoothPlugin：

```
BluetoothPlugin

↓

BluetoothWidget

↓

BluetoothService

↓

ADB

↓

Phone
```

职责：

非常清晰。

---

# 30. 最终目标

整个插件系统应做到：

新增一个插件：

只需要：

```
plugins/

NewPlugin/
```

放进去。

无需：

修改：

MainWindow

Chat

Core

Service

其它代码。

软件启动后：

自动识别。

自动加载。

自动显示。

真正实现：

高扩展性。
