# AI Mobile Test Studio (AMTS)

> AI Native Mobile Automation Testing Platform

Version: v0.1

Author: Ze dex

---

# 一、项目简介

## 项目名称

AI Mobile Test Studio（AMTS）

## 项目定位

AMTS 是一款基于 **Qt + Python + AI Agent** 的桌面端移动自动化测试平台。

目标不是做一个简单的 Appium GUI，而是打造一个类似于：

- Cursor
- VSCode
- Android Studio

这样的 **AI Native Testing IDE**。

未来希望用户只需要自然语言即可完成整个测试流程，而无需手写 Appium 脚本。

例如：

```
帮我测试蓝牙连接。

要求：

1. 打开蓝牙
2. 搜索设备
3. 配对
4. 断开连接
5. 重连
```

AI 自动：

```
理解需求

↓

分析手机UI

↓

生成测试脚本

↓

执行脚本

↓

发现错误

↓

自动修复脚本

↓

继续执行

↓

生成测试报告
```

最终形成完整的 AI Test Agent。

---

# 二、项目目标

核心目标：

打造一个真正的 AI 自动化测试 IDE。

最终用户：

- Android测试工程师
- 手机厂商
- IoT设备厂商
- 智能眼镜
- 智能手表
- QA团队
- 自动化测试开发

支持：

✓ Android

未来支持：

- HarmonyOS
- Linux
- Windows
- iOS

---

# 三、设计原则

整个项目遵循：

## 1. 高可维护性

每个模块职责单一。

禁止：

UI 写业务。

业务写 UI。

所有模块低耦合。

---

## 2. 高扩展性

未来能够增加：

新的AI模型

新的插件

新的测试方式

无需修改核心代码。

例如：

```
OpenAI

Claude

DeepSeek

Gemini

Qwen

Ollama
```

全部实现统一接口。

---

## 3. 高可读性

所有模块：

目录清晰

命名统一

尽量避免巨大类。

推荐：

一个类只负责一件事。

---

## 4. 插件化

所有能力：

最好插件化。

例如：

Bluetooth

WiFi

OCR

Vision

Performance

Monkey

以后直接增加插件。

---

# 四、核心功能

## 第一阶段

### AI聊天

类似：

Cursor

ChatGPT Desktop

Claude Desktop

支持：

- Markdown
- 代码高亮
- 文件上传
- 图片上传

---

### 手机实时画面

左侧：

嵌入：

Scrcpy

要求：

实时

低延迟

支持：

点击

拖动

输入

录屏

截图

---

### Appium

支持：

Python

自动运行

自动调试

---

### ADB

支持：

devices

shell

logcat

install

pull

push

uiautomator dump

dumpsys

---

### AI脚本生成

根据：

自然语言

↓

生成：

Python Appium脚本。

例如：

```
测试登录。
```

↓

生成：

```
pytest

Appium

```

---

### 自动执行

AI：

生成

↓

执行

↓

分析

↓

继续

---

### 自动修复

例如：

NoSuchElementException

AI：

重新分析页面

↓

修改Locator

↓

重新执行

---

### 自动报告

生成：

HTML

Markdown

PDF

包含：

截图

日志

执行时间

失败原因

建议

---

# 五、UI设计

整体参考：

Cursor

+

Claude Desktop

+

Android Studio

布局：

```
------------------------------------------------------------

菜单栏

------------------------------------------------------------

Toolbar

------------------------------------------------------------

Navigation

Conversation

AI Chat

Phone

------------------------------------------------------------

Log

Terminal

Result

------------------------------------------------------------
```

左侧：

导航栏

第二列：

会话列表

中间：

AI聊天

右侧：

手机实时画面

底部：

日志

终端

运行结果

状态栏：

ADB状态

设备名称

连接状态

AI状态

---

# 六、AI Agent流程

整体采用：

Agent Loop

流程：

```
用户需求

↓

LLM理解

↓

拆任务

↓

生成脚本

↓

执行

↓

失败

↓

分析日志

↓

分析UI

↓

修改脚本

↓

重新执行

↓

完成
```

这是整个软件最重要的部分。

---

# 七、Vision能力

AI不仅依赖：

XPath

未来支持：

OCR

OpenCV

YOLO

Vision Model

例如：

AI看到：

```
连接蓝牙
```

直接点击：

按钮。

而不是：

resource-id。

这样：

UI变化以后：

脚本仍然可以运行。

---

# 八、LLM Provider

采用统一接口。

```
BaseProvider

↓

OpenAIProvider

ClaudeProvider

GeminiProvider

DeepSeekProvider

QwenProvider

OllamaProvider
```

用户：

填写：

API URL

API KEY

MODEL

即可。

---

# 九、项目目录

```
AI-Mobile-Test-Studio
│
├── app/
│
├── core/
│
├── services/
│
├── modules/
│
├── plugins/
│
├── scripts/
│
├── sdk/
│
├── resources/
│
├── config/
│
├── workspace/
│
├── logs/
│
├── docs/
│
├── tests/
│
├── README.md
│
└── CMakeLists.txt
```

---

## app

Qt界面。

禁止：

业务逻辑。

包括：

```
pages

widgets

dialogs

styles

icons
```

---

## core

整个软件的大脑。

例如：

```
agent

adb

appium

llm

workflow

logger

utils

scrcpy
```

---

## services

负责：

连接：

UI

Core

Python

例如：

```
ChatService

DeviceService

AIService

ScriptService

PluginService
```

---

## modules

功能模块。

例如：

```
chat

terminal

report

vision

ocr

explorer
```

---

## plugins

插件。

例如：

```
Bluetooth

WiFi

Monkey

Performance

Camera

Custom
```

---

## scripts

Python。

包括：

```
agent

appium

vision

report

utils
```

---

## sdk

内置：

```
adb

scrcpy

python

node

ffmpeg

appium
```

用户无需安装环境。

---

# 十、技术栈

## 前端

Qt6

C++

Qt Designer

QSS

---

## 自动化

Python

Appium

ADB

uiautomator2

---

## AI

OpenAI API

Claude API

DeepSeek API

Gemini API

Ollama

---

## Vision

OpenCV

OCR

YOLO

---

## 打包

Qt

PyInstaller（Python）

WinDeployQt

最终：

绿色免安装版本。

---

# 十一、开发路线

## V0.1

完成：

Qt UI

基础框架

目录

主题

---

## V0.2

完成：

ADB

设备管理

日志

Scrcpy

---

## V0.3

完成：

聊天

API

Markdown

文件上传

---

## V0.4

完成：

Python

Appium

脚本执行

---

## V0.5

完成：

AI生成脚本

自动执行

---

## V0.6

完成：

Agent Loop

自动修复

自动报告

---

## V1.0

发布：

第一个正式版本。

---

# 十二、未来规划

未来增加：

✓ MCP

✓ 多设备

✓ Device Farm

✓ Docker

✓ Plugin Marketplace

✓ 云同步

✓ AI Memory

✓ 企业版

✓ 协同测试

✓ Git集成

✓ Test Case管理

✓ Jira集成

✓ Jenkins集成

✓ GitHub Actions

✓ CI/CD

✓ 本地模型

✓ RAG知识库

---

# 十三、最终目标

打造一个真正意义上的：

> Cursor for Mobile QA

不是：

一个 Appium GUI。

而是：

一个 AI Native 的移动端测试开发平台。

AI 可以：

理解需求。

编写测试。

控制手机。

分析日志。

定位错误。

修复脚本。

生成报告。

整个测试流程尽可能实现自动化，仅在必要时需要人工介入。