
# ROADMAP.md

> AI Mobile Test Studio (AMTS) Development Roadmap

Version: 1.0

Last Updated: 2026-07-05

---

# 项目愿景（Vision）

AI Mobile Test Studio（AMTS）旨在打造一个面向移动端测试工程师的 **AI Native 自动化测试 IDE**。

它不仅仅是 Appium 的图形化工具，而是一个集：

- AI 对话
- 自动生成测试脚本
- 自动执行测试
- 自动分析日志
- 自动修复脚本
- 自动生成测试报告
- 插件扩展

于一体的下一代智能测试平台。

最终目标：

> **让测试工程师使用自然语言即可完成移动端自动化测试。**

---

# 长期目标

最终实现：

- AI 自动编写 Appium 脚本
- AI 自动执行脚本
- AI 自动分析错误
- AI 自动修复脚本
- AI 自动生成测试报告
- AI 自动生成 Bug
- AI 自动调用 ADB
- AI 自动调用 Scrcpy
- AI 自动调用 Python
- AI 自动完成整个测试流程

最终形成：

AI → Tool → Device → Report

完整闭环。

---

# Version 0.1.0（项目初始化）

状态：

✅ 已完成

目标：

建立项目基础框架。

完成内容：

- 项目目录设计
- Qt + CMake 初始化
- UI 原型设计
- 项目文档体系
- Provider 架构设计
- Plugin 架构设计
- Development Guide
- Coding Style
- API Specification

---

# Version 0.2.0（设备管理）

目标：

完成设备管理模块。

预计功能：

Device

- DeviceManager
- DeviceScanner
- DeviceMonitor
- DeviceInfo

ADB

- adb devices
- adb shell
- adb install
- adb uninstall
- adb push
- adb pull

Logcat

- 实时日志
- 日志过滤
- 日志搜索
- 日志导出

UI

- Device Page
- Device Card
- Device Status

完成后：

软件能够识别 Android 设备。

---

# Version 0.3.0（手机画面）

目标：

完成手机实时画面。

包括：

Scrcpy

- 启动
- 停止
- 全屏
- 截图

Phone Widget

- 实时画面
- 鼠标操作
- 键盘输入

支持：

- 多设备切换

---

# Version 0.4.0（AI 对话）

目标：

完成聊天系统。

包括：

Chat

- Markdown
- Code Highlight
- Streaming
- Conversation

Provider

- OpenAI
- Claude
- Gemini
- DeepSeek
- Ollama

支持：

- 多模型切换
- 自定义 API

---

# Version 0.5.0（Python Runtime）

目标：

完成 Python 自动化运行环境。

包括：

- Python Runtime
- 虚拟环境管理
- Appium Runtime
- Script Runner

支持：

- 自动执行 Python
- 自动安装依赖
- 自动检测环境

---

# Version 0.6.0（AI Script）

目标：

AI 自动生成测试脚本。

包括：

Prompt

↓

AI

↓

Python Script

↓

Appium

↓

Device

支持：

- 蓝牙测试
- WiFi 测试
- UI 测试

---

# Version 0.7.0（AI Agent）

目标：

AI Agent 工作流。

实现：

需求

↓

规划

↓

生成脚本

↓

执行

↓

分析

↓

修复

↓

再次执行

↓

完成

实现真正闭环。

---

# Version 0.8.0（插件系统）

目标：

插件化。

官方插件：

- Bluetooth
- WiFi
- Camera
- OCR
- Vision
- Monkey
- Performance
- Report

支持：

第三方插件。

---

# Version 0.9.0（Workspace）

目标：

Workspace。

包括：

项目

脚本

截图

日志

报告

AI 对话

配置

全部保存。

---

# Version 1.0.0（第一个正式版）

目标：

发布稳定版本。

支持：

设备管理

ADB

Appium

Scrcpy

AI Chat

AI Script

AI Agent

Plugin

Workspace

Report

支持企业使用。

---

# Version 2.0（AI Native）

目标：

真正 AI Native。

AI：

自动：

分析需求

↓

拆解任务

↓

生成脚本

↓

执行测试

↓

分析失败

↓

修复脚本

↓

重新执行

↓

输出报告

↓

生成 Bug

无需人工参与。

---

# Version 3.0（Cloud）

目标：

云端测试平台。

支持：

多设备

远程测试

Web Dashboard

多人协作

任务调度

企业部署

CI/CD

---

# 插件发展路线

阶段一：

官方插件。

阶段二：

第三方插件。

阶段三：

插件市场。

支持：

下载安装

自动更新

评分

评论

版本管理

---

# AI 能力发展路线

第一阶段：

聊天。

第二阶段：

生成代码。

第三阶段：

调用工具。

第四阶段：

Agent。

第五阶段：

自动测试。

第六阶段：

自动修复。

第七阶段：

完全自主测试。

---

# 企业功能规划

未来增加：

- 权限管理
- 用户管理
- 团队协作
- 测试任务
- Jenkins
- GitLab CI
- GitHub Actions
- 企业登录
- LDAP

---

# 性能目标

启动时间：

< 3 秒

设备识别：

< 1 秒

AI 回复：

< 5 秒（流式）

Scrcpy 延迟：

< 100 ms

插件加载：

< 500 ms

---

# 开发原则

所有新增功能必须满足：

- 模块化
- 插件化
- 可维护
- 可扩展
- AI 可理解
- 用户无需额外安装开发环境

---

# 当前优先级（Priority）

★★★★★

- DeviceManager
- ADB Runtime
- Scrcpy 集成

★★★★☆

- Chat UI
- Provider
- AIService

★★★☆☆

- Python Runtime
- Appium Runtime

★★☆☆☆

- AI Agent
- OCR
- Vision

★☆☆☆☆

- 插件市场
- 云平台
- 企业协作

---

# Milestone

M1

✅ 项目初始化

M2

□ 设备管理

M3

□ 手机画面

M4

□ AI 对话

M5

□ Python Runtime

M6

□ AI 自动生成脚本

M7

□ AI Agent

M8

□ Plugin

M9

□ Workspace

M10

□ Version 1.0 Release

---

# 最终愿景

AMTS 将成为一个：

- AI Native
- Plugin First
- Cross Platform
- Mobile Testing IDE

帮助测试工程师从"编写脚本"转变为"描述需求"，让 AI 完成自动化测试的大部分工作。
