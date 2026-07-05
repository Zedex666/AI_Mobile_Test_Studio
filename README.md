# AI Mobile Test Studio (AMTS)

> AI Native Mobile Automation Testing Platform

## 简介

AMTS 是一款基于 **Qt + Python + AI Agent** 的桌面端移动自动化测试平台。

目标不是做一个简单的 Appium GUI，而是打造一个类似于 **Cursor / VSCode / Android Studio** 这样的 **AI Native Testing IDE**。

未来希望用户只需要自然语言即可完成整个测试流程，而无需手写 Appium 脚本。

## 技术栈

- **前端**: Qt6, C++, QSS
- **自动化**: Python, Appium, ADB, uiautomator2
- **AI**: OpenAI API, Claude API, DeepSeek API, Gemini API, Ollama
- **Vision**: OpenCV, OCR, YOLO
- **打包**: Qt, PyInstaller, WinDeployQt

## 项目结构

```
AI-Mobile-Test-Studio
│
├── app/              # Qt界面 (pages, widgets, dialogs, styles, icons)
├── core/             # 核心引擎 (agent, adb, appium, llm, workflow, logger, utils, scrcpy)
├── services/         # 服务层 (ChatService, DeviceService, AIService, ScriptService, PluginService)
├── modules/          # 功能模块 (chat, terminal, report, vision, ocr, explorer)
├── plugins/          # 插件系统 (Bluetooth, WiFi, Monkey, Performance, Camera)
├── scripts/          # Python脚本 (agent, appium, vision, report, utils)
├── sdk/              # 内置SDK (adb, scrcpy, python, node, ffmpeg, appium)
├── resources/        # 资源文件 (images, fonts, qss)
├── config/           # 配置文件
├── workspace/        # 工作空间
├── logs/             # 日志目录
├── docs/             # 文档
├── tests/            # 测试用例
├── README.md
└── CMakeLists.txt
```

## 核心功能

### 第一阶段 (V0.1)
- [x] Qt UI 基础框架
- [x] 项目目录结构
- [x] 主题与样式

### 后续阶段
- [ ] ADB 设备管理
- [ ] Scrcpy 实时画面
- [ ] AI 聊天界面 (Markdown, 代码高亮)
- [ ] Python Appium 脚本执行
- [ ] AI 脚本生成
- [ ] Agent Loop (自动执行、自动修复、自动报告)

## 快速开始

### 构建要求
- Qt 6.5+
- CMake 3.19+
- MinGW / MSVC
- Python 3.10+

### 构建步骤

```bash
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
ninja
```

### 运行

```bash
./AI_Mobile_Test_Studio
```

## 开发路线

| 版本 | 目标 |
|------|------|
| V0.1 | Qt UI, 基础框架, 目录, 主题 |
| V0.2 | ADB, 设备管理, 日志, Scrcpy |
| V0.3 | 聊天, API, Markdown, 文件上传 |
| V0.4 | Python, Appium, 脚本执行 |
| V0.5 | AI生成脚本, 自动执行 |
| V0.6 | Agent Loop, 自动修复, 自动报告 |
| V1.0 | 第一个正式版本 |

## 未来规划

- MCP 支持
- 多设备 / Device Farm
- Docker 部署
- Plugin Marketplace
- 云同步 / AI Memory
- Git / Jira / Jenkins 集成
- CI/CD / GitHub Actions
- 本地模型 / RAG 知识库

## 许可证

MIT License

## 作者

Ze dex
