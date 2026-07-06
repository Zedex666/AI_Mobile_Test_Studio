
# ARCHITECTURE.md

> AI Mobile Test Studio (AMTS) Architecture Document
> Version: 1.0

## 1. 文档目的

本文档描述 AMTS（AI Mobile Test Studio）的整体软件架构，为开发者和 AI
Agent 提供统一的设计依据。

### 目标

- 高可维护性
- 高可扩展性
- 插件化
- AI Native
- 跨平台（未来支持）

---

## 2. 项目定位

AMTS 是一个 **AI Native Mobile Testing IDE**。

核心能力包括：

- Qt 桌面客户端
- AI 对话与测试脚本生成
- Python + Appium 自动化
- ADB / Scrcpy 设备控制
- 插件系统
- 多模型 Provider
- Workspace 管理

---

## 3. 总体架构

```text
User
 │
 ▼
Qt UI
 │
 ▼
Service Layer
 │
 ▼
Core Layer
 │
 ├── AI Runtime
 ├── Tool Runtime
 ├── Plugin Runtime
 └── Workspace
 │
 ▼
Python Runtime
 │
 ├── Appium
 ├── ADB
 ├── Scrcpy
 └── OCR / Vision
 │
 ▼
Android Device
```

---

## 4. 分层架构

### UI Layer

负责：

- 页面展示
- 用户交互
- 日志显示
- 手机画面

禁止直接调用：

- Appium
- ADB
- HTTP
- AI API

---

### Service Layer

负责：

- 页面协调
- 工作流调度
- 调用 Core

例如：

- ChatService
- DeviceService
- ScriptService
- AIService

---

### Core Layer

负责：

- 真正业务逻辑
- Runtime
- Plugin
- Workflow

Core 不依赖 Qt Widgets。

---

## 5. 模块关系

```text
MainWindow
 ├── DevicePage
 ├── ChatPage
 ├── ReportPage
 └── SettingsPage

ChatPage
    │
ChatService
    │
AIService
    │
BaseProvider
    ├── OpenAI
    ├── Claude
    ├── Gemini
    ├── DeepSeek
    └── Ollama
```

---

## 6. Runtime 架构

### AI Runtime

统一管理：

- Provider
- Agent
- Prompt
- Tool Calling

### Tool Runtime

统一管理：

- ADB
- Appium
- Python
- Scrcpy
- OCR

### Python Runtime

负责：

- 执行测试脚本
- 调用 Appium
- 分析结果
- 生成报告

---

## 7. 插件架构

```text
PluginManager
 │
 ├── UI Plugins
 ├── Tool Plugins
 ├── AI Plugins
 └── Report Plugins
```

所有插件通过 PluginInterface 接入。

---

## 8. 数据流

用户输入需求

↓

AI 生成脚本

↓

Python 执行

↓

Appium 控制设备

↓

采集结果

↓

AI 分析

↓

输出报告

---

## 9. 生命周期

应用启动

↓

加载配置

↓

初始化 Runtime

↓

加载插件

↓

连接设备

↓

进入工作区

↓

关闭资源

↓

退出程序

---

## 10. 多线程

建议：

- UI 主线程
- AI 请求线程
- Python 执行线程
- 日志线程
- Scrcpy 视频线程

线程之间统一 Signal / Slot。

---

## 11. Workspace

Workspace 保存：

- 配置
- 对话
- 测试脚本
- 日志
- 报告
- 截图

保证项目隔离。

---

## 12. 日志系统

统一 Logger：

- INFO
- DEBUG
- WARNING
- ERROR

日志输出到：

    workspace/logs/

---

## 13. 配置系统

统一 JSON：

    config/
        settings.json
        models.json
        theme.json

禁止硬编码。

---

## 14. 扩展原则

新增：

- Provider
- Plugin
- Tool
- Widget

不得修改核心架构。

遵循：

开放封闭原则（OCP）。

---

## 15. 长期规划

未来支持：

- MCP
- 多设备协同
- OCR
- Vision
- AI Agent Loop
- 云端测试
- 插件市场

最终目标：

打造一个 AI 驱动、插件化、可扩展的移动端自动化测试 IDE。
