# DEVELOPMENT_GUIDE.md

> AI Mobile Test Studio（AMTS）开发指南

Version: 1.0

---

# 1. 文档目的

本文档用于指导所有开发者（包括 AI Agent）参与 AMTS 项目的开发。

本项目采用模块化、分层架构设计。

开发过程中必须遵守本文档中的所有规范，以保证：

- 高可维护性
- 高可扩展性
- 高可读性
- 低耦合
- 易于 AI 理解

---

# 2. 开发原则

整个项目遵循以下原则：

## 单一职责原则（Single Responsibility）

每一个类只负责一件事情。

例如：

✓ DeviceManager

负责设备管理。

不要：

DeviceManager

↓

连接设备

↓

执行脚本

↓

聊天

↓

AI

全部写进去。

禁止。

---

## 高内聚

一个模块只管理自己的功能。

例如：

Chat Module

不能：

管理ADB。

ADB Module

不能：

操作聊天记录。

---

## 低耦合

模块之间通过：

Service

Interface

Signal / Slot

通信。

禁止：

直接互相调用内部实现。

---

## 不重复造轮子

优先：

Qt

STL

官方库

成熟第三方库。

禁止：

重复实现已有能力。

---

# 3. 项目架构

项目采用：

MVC + Service + Core

整体结构：

```

UI

↓

ViewModel

↓

Service

↓

Core

↓

Python

↓

ADB / Appium

```

说明：

UI：

只负责显示。

Service：

负责业务协调。

Core：

负责真正业务逻辑。

Python：

负责自动化。

---

# 4. UI 开发规范

所有页面：

放入：

```

app/pages/

```

例如：

```

ChatPage

DevicePage

SettingPage

PluginPage

```

每一个页面：

一个目录。

例如：

```

ChatPage/

├── ChatPage.h
├── ChatPage.cpp
└── ChatPage.ui

```

禁止：

所有页面：

放一起。

---

## Widget

所有可复用组件：

放：

```

app/widgets/

```

例如：

```

MessageWidget

DeviceCard

PhoneView

MarkdownView

LogWidget

```

Widget：

必须：

可以独立使用。

禁止：

依赖页面。

---

## Dialog

所有弹窗：

统一：

```

app/dialogs/

```

例如：

```

ApiDialog

AboutDialog

ConnectDialog

```

禁止：

弹窗代码：

写在 MainWindow。

---

# 5. Service 开发规范

Service：

负责：

连接：

UI

Core

Python。

例如：

```

ChatPage

↓

ChatService

↓

LLMProvider

```

UI：

不能：

直接：

调用：

OpenAI。

必须：

经过：

Service。

---

Service：

统一命名：

```

XXXService

```

例如：

```

ChatService

DeviceService

ScriptService

AIService

```

---

# 6. Core 开发规范

Core：

负责：

真正逻辑。

例如：

```

Agent

ADB

Appium

Workflow

Logger

Config

```

禁止：

Core：

依赖：

Qt Widget。

Core：

必须：

独立。

未来：

可用于：

CLI。

---

# 7. Python 开发规范

Python：

负责：

自动化。

禁止：

Qt。

例如：

```

scripts/

appium/

vision/

agent/

report/

```

Python：

与 Qt：

通过：

JSON

标准输出

Socket

HTTP

通信。

禁止：

共享变量。

---

# 8. 新增页面规范

新增页面：

必须：

包含：

```

Page.ui

Page.h

Page.cpp

```

例如：

```

AutomationPage/

├── AutomationPage.ui
├── AutomationPage.h
└── AutomationPage.cpp

```

页面：

不得：

超过：

1000 行。

超过：

必须：

拆 Widget。

---

# 9. 新增 Widget

Widget：

目录：

```

widgets/

PhoneWidget/

├── PhoneWidget.cpp
├── PhoneWidget.h
└── PhoneWidget.ui

```

Widget：

必须：

职责单一。

例如：

PhoneWidget

只负责：

显示手机。

不要：

执行ADB。

---

# 10. 新增 Provider

所有AI：

统一接口。

例如：

```

BaseProvider

↓

OpenAIProvider

ClaudeProvider

DeepSeekProvider

GeminiProvider

OllamaProvider

```

新增模型：

只需要：

新增：

Provider。

禁止：

修改：

ChatService。

---

# 11. 新增插件

插件：

放：

```

plugins/

Bluetooth

WiFi

Camera

Performance

```

插件：

必须：

继承：

```

PluginInterface

```

禁止：

直接：

修改：

核心代码。

---

# 12. 日志规范

统一：

Logger。

禁止：

printf()

禁止：

std::cout

Qt：

使用：

Logger。

日志：

分类：

```

INFO

WARNING

ERROR

DEBUG

```

输出：

```

logs/

```

每天：

一个文件。

---

# 13. 配置规范

所有配置：

JSON。

例如：

```

config/

settings.json

theme.json

models.json

```

禁止：

写死：

API。

路径。

主题。

颜色。

---

# 14. AI 调用规范

所有AI：

统一：

AIService。

流程：

```

UI

↓

ChatService

↓

AIService

↓

Provider

↓

HTTP

```

禁止：

UI：

直接：

HTTP。

---

# 15. Python 通信规范

Qt：

永远：

不要：

解析：

Python。

统一：

JSON。

例如：

```

{
"type":"progress",
"value":60
}

```

```

{
"type":"finish"
}

```

统一协议。

---

# 16. Git 规范

main

始终：

稳定。

开发：

feature/

例如：

```

feature/chat

feature/device

feature/report

```

修复：

```

fix/logcat

fix/provider

```

禁止：

直接：

Commit：

main。

---

# 17. Commit 规范

统一：

```

feat:

fix:

docs:

style:

refactor:

test:

chore:

```

例如：

```

feat: add bluetooth plugin

fix: repair adb detection

docs: update architecture

```

---

# 18. 开发顺序

推荐：

第一阶段：

✓ MainWindow

↓

第二阶段：

✓ Device

↓

第三阶段：

✓ Scrcpy

↓

第四阶段：

✓ Chat

↓

第五阶段：

✓ API

↓

第六阶段：

✓ Python

↓

第七阶段：

✓ Appium

↓

第八阶段：

✓ AI Agent

↓

第九阶段：

✓ Report

---

# 19. AI 开发规则

所有 AI Agent：

必须：

先阅读：

```

PROJECT_DESIGN.md

ARCHITECTURE.md

DEVELOPMENT_GUIDE.md

CODING_STYLE.md

```

之后：

才能：

生成代码。

AI：

不得：

随意：

修改：

目录。

不得：

新增：

重复模块。

不得：

破坏：

分层结构。

---

# 20. 开发目标

整个项目最终目标不是：

一个：

Appium GUI。

而是：

一个真正的：

AI Native Mobile Testing IDE。

任何新增功能都必须符合以下原则：

- 模块化
- 可插拔
- 易维护
- 易扩展
- AI 可理解
- 用户无需额外安装运行环境

如果新增功能破坏了上述原则，应重新设计后再进行开发。