# API_SPEC.md

> AI Mobile Test Studio (AMTS) API Specification

Version: 1.0

---

# 1. 文档目的

本文档定义了 AMTS 所有 AI Provider 的统一接口规范。

所有 AI 模型（包括云端模型、本地模型、自定义模型）必须遵循本规范。

目标：

- Provider 可插拔
- 最小耦合
- 统一调用方式
- 易于扩展
- 支持多模型切换

---

# 2. 整体架构

所有 Provider 必须继承 BaseProvider。

```

Chat UI

↓

ChatService

↓

AIService

↓

BaseProvider

↓

OpenAI

Claude

Gemini

DeepSeek

Qwen

Ollama

Custom Provider

```

UI 永远不知道底层是哪一种模型。

---

# 3. Provider 生命周期

Provider 生命周期：

```

创建

↓

初始化

↓

读取模型

↓

发送消息

↓

流式返回

↓

取消

↓

释放资源

```

---

# 4. BaseProvider

所有 Provider 必须继承：

```cpp
class BaseProvider
{
public:

    virtual bool initialize() = 0;

    virtual QString name() const = 0;

    virtual QList<ModelInfo> models() = 0;

    virtual bool setModel(const QString& model) = 0;

    virtual void sendMessage(const ChatRequest&) = 0;

    virtual void streamMessage(const ChatRequest&) = 0;

    virtual void cancel() = 0;

    virtual bool isConnected() const = 0;

    virtual void reset() = 0;

    virtual ~BaseProvider() = default;
};
```

禁止：

Provider 自定义接口。

---

# 5. Provider 注册

所有 Provider：

统一：

ProviderFactory

例如：

```
ProviderFactory

↓

OpenAIProvider

ClaudeProvider

GeminiProvider

DeepSeekProvider

OllamaProvider

CustomProvider
```

新增 Provider：

只需要：

注册。

无需：

修改：

ChatService。

---

# 6. Provider 信息

Provider：

至少包含：

```
名称

图标

版本

官网

支持模型

是否支持流式

是否支持Vision

是否支持Tool Calling

是否支持MCP
```

例如：

```cpp
struct ProviderInfo
{
    QString name;

    QString version;

    QString website;

    bool supportStream;

    bool supportVision;

    bool supportTool;

    bool supportMCP;
};
```

---

# 7. ModelInfo

模型：

统一：

```cpp
struct ModelInfo
{
    QString id;

    QString name;

    QString description;

    int contextLength;

    bool vision;

    bool functionCalling;

    bool reasoning;
};
```

例如：

```
gpt-5

claude-opus

deepseek-chat

qwen-max

gemini-2.5-pro
```

---

# 8. ChatRequest

统一请求格式：

```cpp
struct ChatRequest
{
    QString conversationId;

    QString model;

    QString prompt;

    QList<Message> history;

    QList<FileAttachment> attachments;

    double temperature;

    int maxTokens;

    bool stream;
};
```

---

# 9. ChatResponse

统一返回：

```cpp
struct ChatResponse
{
    bool success;

    QString content;

    QString finishReason;

    QString error;

    int promptTokens;

    int completionTokens;

    int totalTokens;
};
```

---

# 10. Message

聊天记录：

统一：

```cpp
struct Message
{
    QString role;

    QString content;
};
```

role：

只能：

```
system

user

assistant

tool
```

---

# 11. 附件

统一：

```cpp
struct FileAttachment
{
    QString fileName;

    QString fileType;

    QString path;

    QString mimeType;
};
```

支持：

```
txt

pdf

docx

xlsx

csv

png

jpg

json

xml

zip
```

未来：

支持：

APK。

---

# 12. Streaming

所有支持流式输出的 Provider：

必须：

实现：

```
streamMessage()
```

返回：

```
Token

↓

Token

↓

Token

↓

Finish
```

禁止：

一次：

全部返回。

---

# 13. Error

统一错误：

```cpp
enum class ProviderError
{
    None,

    Network,

    Timeout,

    Unauthorized,

    InvalidApiKey,

    InvalidModel,

    QuotaExceeded,

    ServerError,

    Unknown
};
```

禁止：

字符串判断。

---

# 14. Retry

网络错误：

自动：

Retry。

默认：

```
3次
```

Retry：

采用：

指数退避。

例如：

```
1秒

↓

2秒

↓

4秒
```

---

# 15. Vision

Vision Provider：

必须：

支持：

```
图片

截图

手机截图

OCR

UI分析
```

例如：

```
手机截图

↓

Vision

↓

按钮位置

↓

生成脚本
```

---

# 16. Tool Calling

Provider：

如果支持：

Tool Calling。

统一：

```
callTool()

↓

返回

ToolResult
```

例如：

```
ADB

Scrcpy

Python

Report

OCR
```

---

# 17. MCP

未来：

Provider：

支持：

Model Context Protocol。

统一：

```
connectMCP()

disconnectMCP()

callMCP()
```

---

# 18. Provider 配置

配置：

JSON。

例如：

```json
{
    "provider": "OpenAI",
    "baseUrl": "https://api.openai.com/v1",
    "apiKey": "xxxxxxxx",
    "model": "gpt-5",
    "temperature": 0.7
}
```

禁止：

硬编码。

---

# 19. Custom Provider

允许：

用户：

新增：

兼容 OpenAI API 的模型。

用户填写：

```
API URL

API KEY

MODEL
```

即可。

无需：

修改代码。

---

# 20. 多 Provider

允许：

多个 Provider：

同时存在。

例如：

```
Chat

↓

OpenAI

↓

Claude

↓

DeepSeek
```

用户：

可以：

自由切换。

---

# 21. Provider Factory

统一：

```cpp
auto provider = ProviderFactory::create(providerType);
```

禁止：

```
new OpenAIProvider()
```

写在：

UI。

---

# 22. Provider Manager

统一管理：

```
加载

切换

销毁

缓存

状态

连接
```

所有 Provider。

---

# 23. API 限流

Provider：

必须：

支持：

```
Rate Limit

Retry

Queue
```

避免：

同时：

大量请求。

---

# 24. AI Agent

AI Agent：

不得：

直接：

HTTP。

统一：

```
Agent

↓

AIService

↓

Provider
```

---

# 25. OpenAI Compatible API

支持：

所有兼容：

OpenAI API 的模型。

例如：

```
DeepSeek

Qwen

智谱

月之暗面

SiliconFlow

OpenRouter

LM Studio

vLLM
```

统一：

OpenAIProvider。

无需：

重新开发。

---

# 26. 本地模型

支持：

```
Ollama

LM Studio

vLLM
```

统一：

LocalProvider。

支持：

离线。

---

# 27. API Key

禁止：

硬编码。

统一：

```
config/models.json
```

保存。

未来：

支持：

系统加密。

---

# 28. 日志

所有请求：

记录：

```
Provider

Model

耗时

Token

错误

状态
```

禁止：

记录：

API Key。

---

# 29. AI 开发规范

新增 Provider：

必须：

1.

继承：

BaseProvider

2.

实现：

所有虚函数

3.

注册：

ProviderFactory

4.

增加：

图标

5.

增加：

配置页面

6.

增加：

文档

---

# 30. 最终目标

整个 AI 模块应该做到：

增加一个新的 AI 模型：

仅需：

新增：

```
XXXProvider.cpp

XXXProvider.h
```

然后：

注册。

无需：

修改：

UI

ChatService

Agent

Workflow

其它模块。

整个系统始终保持：

高扩展性

高一致性

低耦合

易维护。