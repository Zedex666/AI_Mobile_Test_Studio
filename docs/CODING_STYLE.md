# CODING_STYLE.md

> AI Mobile Test Studio (AMTS) Coding Style Guide

Version: 1.0

---

# 1. 文档目的

本文档规定了 AMTS 项目的编码规范。

所有开发者（包括 AI Agent）必须遵守本规范。

目标：

- 高可读性
- 高一致性
- 易维护
- 易扩展
- AI 易理解

当本文档与个人编码习惯冲突时，以本文档为准。

---

# 2. 通用原则

代码首先是写给人看的，其次才是写给计算机。

任何代码都应做到：

✓ 一眼知道作用

✓ 一眼知道属于哪个模块

✓ 一眼知道生命周期

禁止：

为了"炫技"而写复杂代码。

---

# 3. 命名规范

统一使用：

英文。

禁止：

拼音。

禁止：

中文。

例如：

正确：

```
DeviceManager
```

错误：

```
SheBeiManager

设备管理器
```

---

# 4. 文件命名

统一：

PascalCase

例如：

```
MainWindow.cpp

ChatPage.cpp

PhoneWidget.cpp

BluetoothService.cpp
```

不要：

```
mainwindow.cpp

chat_page.cpp

phonewidget.cpp
```

---

# 5. 类命名

统一：

PascalCase

例如：

```
DeviceManager

ChatService

ADBManager

WorkflowEngine
```

接口：

统一：

I 开头。

例如：

```
IProvider

IPlugin

IAgent
```

抽象类：

Base 开头。

例如：

```
BaseProvider

BasePlugin
```

---

# 6. 函数命名

统一：

camelCase

例如：

```
connectDevice()

disconnectDevice()

startAppium()

generateScript()

sendMessage()
```

禁止：

```
Connect_Device()

connect_device()

CONNECTDEVICE()
```

---

# 7. 变量命名

成员变量：

统一：

m_

例如：

```
m_device

m_phoneWidget

m_logger
```

局部变量：

camelCase

例如：

```
deviceName

currentPage

scriptPath
```

布尔：

统一：

is

has

can

例如：

```
isConnected

hasDevice

canExecute
```

---

# 8. 常量

统一：

constexpr

例如：

```
constexpr int MaxRetryCount = 3;
```

不要：

```
#define MAX_RETRY 3
```

除非：

Qt 宏。

---

# 9. 枚举

统一：

Enum Class

例如：

```
enum class DeviceState
{
    Offline,
    Connecting,
    Connected
};
```

禁止：

传统 enum。

---

# 10. 一个类只做一件事

例如：

PhoneWidget

负责：

显示手机。

不要：

执行ADB。

ChatService

负责：

聊天。

不要：

生成报告。

---

# 11. 一个文件只放一个类

例如：

```
ChatService.h

↓

只允许：

ChatService
```

不要：

一个文件：

三个类。

---

# 12. 类长度

建议：

≤300 行

超过：

考虑拆分。

页面：

≤800 行。

Widget：

≤500 行。

Service：

≤500 行。

---

# 13. 函数长度

建议：

≤40 行。

超过：

拆函数。

不要：

一个函数：

300 行。

---

# 14. 注释规范

不要：

解释代码。

而是：

解释原因。

错误：

```cpp
// i++
i++;
```

正确：

```cpp
// 重试连接设备，最多三次
retryConnect();
```

---

# 15. Qt规范

禁止：

直接：

new Widget

然后：

不释放。

统一：

QObject

父子关系。

例如：

```cpp
auto *dialog = new SettingDialog(this);
```

不要：

```cpp
new SettingDialog();
```

---

Signal：

统一：

过去式。

例如：

```
connected()

disconnected()

finished()
```

Slot：

统一：

onXXX()

例如：

```
onConnectClicked()

onSendMessage()

onDeviceSelected()
```

---

# 16. 日志

禁止：

```
printf()

cout
```

统一：

Logger

例如：

```
LOG_INFO()

LOG_WARNING()

LOG_ERROR()

LOG_DEBUG()
```

所有异常：

必须：

记录日志。

---

# 17. 异常处理

禁止：

忽略异常。

错误：

```cpp
catch (...)
{
}
```

正确：

```cpp
catch (...)
{
    LOG_ERROR(...);
}
```

---

# 18. UI规范

UI：

只负责：

显示。

禁止：

UI：

执行：

ADB。

执行：

HTTP。

执行：

Python。

所有业务：

必须：

交给：

Service。

---

# 19. Service规范

Service：

不允许：

直接操作Widget。

例如：

错误：

```
ChatService

↓

ui->textEdit
```

正确：

```
ChatPage

↓

ChatService

↓

emit messageGenerated()
```

---

# 20. Python规范

Python：

PEP8。

函数：

snake_case

例如：

```
generate_script()

run_appium()

analyze_logcat()
```

类：

PascalCase

例如：

```
AppiumRunner

AgentExecutor
```

---

# 21. JSON

统一：

camelCase。

例如：

```
{
    "deviceName":"",
    "deviceId":"",
    "batteryLevel":90
}
```

不要：

```
device_name

DEVICE_NAME
```

---

# 22. AI Provider规范

所有Provider：

继承：

```
BaseProvider
```

必须实现：

```
initialize()

sendMessage()

streamMessage()

cancel()

models()

name()
```

不要：

自己定义接口。

---

# 23. Plugin规范

插件：

必须：

实现：

```
PluginInterface
```

不要：

直接：

依赖：

MainWindow。

---

# 24. CMake规范

一个模块：

一个：

CMakeLists.txt

不要：

整个项目：

只有一个：

5000行：

CMakeLists。

---

# 25. Include顺序

统一：

```
当前头文件

Qt

STL

第三方

项目内部
```

例如：

```cpp
#include "ChatService.h"

#include <QObject>
#include <QString>

#include <memory>

#include "Logger.h"
```

---

# 26. 智能指针

优先：

```
std::unique_ptr
```

其次：

```
std::shared_ptr
```

避免：

裸指针。

Qt对象：

遵循：

QObject

父子关系。

---

# 27. 魔法数字

禁止：

```
sleep(5);
```

正确：

```
constexpr int RetryDelay = 5;
```

---

# 28. AI Agent 开发规范

AI 不得：

- 修改项目目录结构
- 修改公开接口
- 重复创建已有模块
- 新建重复 Widget
- 修改公共 API

AI 必须：

优先：

复用已有类。

新增功能：

必须：

符合：

Service + Core 架构。

---

# 29. TODO规范

统一：

```
TODO:

FIXME:

NOTE:
```

例如：

```cpp
// TODO: 支持多设备
```

---

# 30. 禁止事项

禁止：

超长函数。

禁止：

超长类。

禁止：

God Class（上帝类）。

禁止：

复制粘贴代码。

禁止：

硬编码路径。

禁止：

硬编码API。

禁止：

UI写业务。

禁止：

Service操作UI。

禁止：

Core依赖Widget。

---

# 31. 最终目标

整个项目应该做到：

任何一个模块都可以独立阅读。

任何一个 AI Agent 都能够快速理解代码。

新增任何功能都不会破坏现有架构。

代码风格保持统一，整个项目看起来像是由一位开发者完成，而不是多人拼接而成。