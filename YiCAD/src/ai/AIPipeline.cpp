/*
 * Copyright (C) 2026 YiCAD Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/// @file AIPipeline.cpp
/// @brief AIPipeline 实现 —— AI 总调度器

#include "AIPipeline.h"

#include "AILLMClassifier.h"
#include "AIIntentRouter.h"
#include "ContextResolver.h"
#include "ConversationHistory.h"
#include "DeepSeekProvider.h"
#include "DirectEntityExecutor.h"
#include "LLMCommandBridge.h"
#include "RAGPipeline.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QPointer>

// ============================================================================
// 构造 / 析构
// ============================================================================

AIPipeline::AIPipeline(const QString& docsDir,
                       const QString& readmePath,
                       DmDocument* doc,
                       GuiDocumentView* docView,
                       QObject* parent)
    : QObject(parent)
    , m_router(new AIIntentRouter(this))
    , m_llmClassifier(new AILLMClassifier(this))
    , m_ragPipeline(new RAGPipeline(this))
    , m_modelingProvider(new DeepSeekProvider(this))
    , m_bridge()
    , m_contextResolver(doc ? new ContextResolver(doc, this) : nullptr)
    , m_drawExecutor(doc ? new DirectEntityExecutor(doc, this) : nullptr)
    , m_doc(doc)
{
    // ---- 1. 初始化 LLM 分类器 ----
    m_routerReady = m_llmClassifier->isAvailable();
    if (!m_routerReady)
    {
        emit errorOccurred(tr("Warning"),
                           tr("LLM classifier is not available (API Key not configured). "
                              "Falling back to dual-pipeline mode."));
    }

    // 连接分类失败信号（用于降级到 3B）
    connect(m_llmClassifier.get(), &AILLMClassifier::classificationFailed,
            this, [this](const QString& reason) {
        Q_UNUSED(reason)
        // 降级：如果当前没有在 3B 模式且有待处理的用户输入，走双 Pipeline
        if (!m_inDualPipeline && !m_lastUserText.isEmpty())
        {
            handleUncertainInput(m_lastUserText);
        }
    });

    // ---- 2. 初始化 RAG（加载知识库） ----
    m_ragReady = m_ragPipeline->initialize(docsDir, readmePath);
    if (!m_ragReady)
    {
        emit errorOccurred(tr("Warning"),
                           tr("RAG knowledge base is empty. "
                              "QA functionality will be limited. "
                              "Check docs/ directory."));
    }

    // ---- 3. 建模链路就绪检查 ----
    m_modelingReady = (doc != nullptr) && (docView != nullptr) && (m_contextResolver != nullptr);
    if (!m_modelingReady)
    {
        emit errorOccurred(tr("Warning"),
                           tr("No document open. Modeling commands are unavailable."));
    }

    // ---- 4. 连接 QA 链路信号 ----
    connect(m_ragPipeline.get(), &RAGPipeline::answerReady,
            this, &AIPipeline::onRAGAnswer);
    connect(m_ragPipeline.get(), &RAGPipeline::errorOccurred,
            this, &AIPipeline::onRAGError);

    // ---- 5. 连接 Modeling 链路信号 ----
    connect(m_modelingProvider.get(), &DeepSeekProvider::responseReceived,
            this, &AIPipeline::onModelingProviderResponse);
    connect(m_modelingProvider.get(), &DeepSeekProvider::errorOccurred,
            this, &AIPipeline::onModelingProviderError);
}

AIPipeline::~AIPipeline() = default;

// ============================================================================
// 公开接口
// ============================================================================

bool AIPipeline::isReady() const
{
    return m_routerReady;
}

void AIPipeline::handleUserInput(const QString& text, const QString& mode)
{
    m_lastUserText = text;
    m_lastMode     = mode;

    // ---- 空输入保护 ----
    if (text.trimmed().isEmpty())
    {
        emit errorOccurred(tr("Error"), tr("Input is empty."));
        return;
    }

    // ---- 0. 记录用户消息到对话历史 ----
    m_history.pushUser(text);

    // ---- 1. 手动模式：同步路由（不变） ----
    if (mode == QStringLiteral("qa") || mode == QStringLiteral("modeling"))
    {
        const RouterResult route = m_router->route(text, mode);
        dispatchByIntent(route, text);
        return;
    }

    // ---- 2. 自动模式：异步 LLM 路由 ----
    // LLM 分类器不可用 → 直接走 3B 双 Pipeline
    if (!m_routerReady)
    {
        handleUncertainInput(text);
        return;
    }

    emit responseReady(tr("System"), tr("Analyzing intent..."));

    // 递增请求序列号
    const int seq = ++m_classifySeq;

    // 提取最近 N 条用户消息作为上下文
    const QStringList recentMsgs = m_history.recentUserMessages(2);

    QPointer<AIPipeline> guard(this);
    m_llmClassifier->classify(
        text, recentMsgs,
        [this, guard, seq, text](IntentType intent, float confidence) {
            // 生命周期保护：如果 AIPipeline 已被析构，直接返回
            if (!guard) return;
            // 序列号保护：忽略过时的回调
            if (seq != m_classifySeq) return;

            // 构造 RouterResult
            RouterResult route;
            route.intent     = intent;
            route.confidence = confidence;
            dispatchByIntent(route, text);
        });
}

// ============================================================================
// 意图分发
// ============================================================================

void AIPipeline::dispatchByIntent(const RouterResult& route,
                                   const QString& text)
{
    switch (route.intent)
    {
    case IntentType::QA:
        m_lastResolvedIntent = IntentType::QA;
        handleUserInput_QA(route, text);
        break;

    case IntentType::Modeling:
        m_lastResolvedIntent = IntentType::Modeling;
        handleUserInput_Modeling(route, text);
        break;

    case IntentType::Mixed:
        emit responseReady(tr("System"),
                           tr("Detected mixed intent. Providing help answer first."));
        m_lastResolvedIntent = IntentType::QA;
        handleUserInput_QA(route, text);
        break;

    case IntentType::Uncertain:
    default:
        // LLM 也不确定 → 走 3B 双 Pipeline 并发
        handleUncertainInput(text);
        break;
    }
}

void AIPipeline::handleUncertainInput(const QString& text)
{
    emit responseReady(tr("System"), tr("Analyzing with dual pipelines..."));

    m_inDualPipeline = true;
    m_dualPendingCount = 2;
    m_pendingQAResult.reset();
    m_pendingModelResult = ParsedCommand{};

    // 并发启动 QA
    if (m_ragReady)
    {
        m_ragPipeline->query(text);
    }
    else
    {
        // RAG 不可用，直接标记 QA 失败
        m_dualPendingCount--;
    }

    // 并发启动 Modeling
    if (m_modelingReady)
    {
        const QString sysPrompt = buildModelingSystemPrompt();
        m_modelingProvider->sendMessage(text, m_history.toMessages(sysPrompt));
    }
    else
    {
        // Modeling 不可用，直接标记失败
        m_dualPendingCount--;
    }

    // 如果两个都不行，直接输出错误
    if (m_dualPendingCount <= 0)
    {
        m_inDualPipeline = false;
        emit errorOccurred(tr("Error"),
                           tr("Unable to process your request. "
                              "Neither QA nor Modeling pipeline is available. "
                              "Please open a document or check your configuration."));
    }
}

void AIPipeline::tryResolveDualPipeline()
{
    m_inDualPipeline = false;

    const bool qaOk   = m_pendingQAResult.has_value();
    const bool modelOk = m_pendingModelResult.ok;

    if (modelOk)
    {
        // Modeling 优先：直接执行
        executeParsedCommand(m_pendingModelResult);
        return;
    }

    if (qaOk)
    {
        // QA 降级
        onRAGAnswer(m_pendingQAResult.value());
        return;
    }

    // 都失败
    emit errorOccurred(tr("Error"),
                       tr("Unable to process your request. "
                          "Please try rephrasing or switch to manual mode."));
}

// ============================================================================
// 模式映射
// ============================================================================

QString AIPipeline::modeToRouterToken(int comboIndex)
{
    switch (comboIndex)
    {
    case 0:  return QStringLiteral("qa");       // Q&A
    case 1:  return QStringLiteral("modeling"); // Modeling
    case 2:
    default: return QStringLiteral("auto");      // Auto
    }
}

// ============================================================================
// QA 链路 —— 分发与回调
// ============================================================================

void AIPipeline::handleUserInput_QA(const RouterResult& route, const QString& text)
{
    if (!m_ragReady)
    {
        emit errorOccurred(tr("Error"),
                           tr("RAG pipeline is not initialized. "
                              "QA mode is unavailable."));
        return;
    }

    emit responseReady(tr("System"),
                       tr("Intent: Q&A (confidence: %1%). Searching knowledge base...")
                           .arg(static_cast<int>(route.confidence * 100)));

    m_ragPipeline->query(text);
}

void AIPipeline::onRAGAnswer(const RAGAnswer& answer)
{
    // 3B 双 Pipeline 模式：暂存结果，等待 Modeling 也完成
    if (m_inDualPipeline)
    {
        m_pendingQAResult = answer;
        m_dualPendingCount--;
        if (m_dualPendingCount <= 0)
        {
            tryResolveDualPipeline();
        }
        return;
    }

    // 记录 assistant 回复到对话历史
    m_history.pushAssistant(answer.answer);

    // 组装展示文本：主回答 + 引用
    QString display = answer.answer;

    if (!answer.citations.isEmpty())
    {
        display += QStringLiteral("\n\n---\n");
        display += tr("References:");
        display += QStringLiteral("\n");
        for (int i = 0; i < answer.citations.size(); ++i)
        {
            const Citation& c = answer.citations[i];
            display += QStringLiteral("  [%1] %2 (%3)\n")
                           .arg(i + 1)
                           .arg(c.title.isEmpty() ? c.docPath : c.title)
                           .arg(c.chunkId);
        }
    }

    if (answer.confidence > 0.0f)
    {
        display += QStringLiteral("\n");
        display += tr("Confidence: %1%").arg(static_cast<int>(answer.confidence * 100));
    }

    emit responseReady(tr("AI"), display);
}

void AIPipeline::onRAGError(const QString& error)
{
    emit errorOccurred(tr("RAG Error"), error);
}

// ============================================================================
// Modeling 链路 —— 分发与回调
// ============================================================================

void AIPipeline::handleUserInput_Modeling(const RouterResult& route, const QString& text)
{
    if (!m_modelingReady)
    {
        emit errorOccurred(tr("Error"),
                           tr("Modeling mode requires an open document. "
                              "Please open or create a drawing first."));
        return;
    }

    emit responseReady(tr("System"),
                       tr("Intent: Modeling (%1%). Generating command...")
                           .arg(static_cast<int>(route.confidence * 100)));

    // 注入建模系统指令到对话历史
    m_history.pushSystem(buildModelingSystemPrompt());

    // 发送给 LLM：传入历史消息数组（含 system + 前文）
    const QString sysPrompt = buildModelingSystemPrompt();
    m_modelingProvider->sendMessage(text, m_history.toMessages(sysPrompt));
}

void AIPipeline::onModelingProviderResponse(const QString& responseText)
{
    // 1. 解析 LLM 返回的 JSON
    const ParsedCommand cmd = m_bridge.parse(responseText);

    // 3B 双 Pipeline 模式：暂存解析结果，不执行，等待 QA 也完成
    if (m_inDualPipeline)
    {
        m_pendingModelResult = cmd;
        m_dualPendingCount--;
        if (m_dualPendingCount <= 0)
        {
            tryResolveDualPipeline();
        }
        return;
    }

    if (!cmd.ok)
    {
        emit errorOccurred(tr("Parse Error"),
                           tr("Failed to parse modeling command from LLM response:\n%1\n\n"
                              "Raw response:\n%2")
                               .arg(cmd.errorMessage)
                               .arg(responseText.left(500)));
        return;
    }

    // 2. 高风险操作提示（首版仅警告，不阻断执行）
    if (cmd.needsConfirmation)
    {
        emit responseReady(tr("System"),
                           tr("⚠ High-risk operation: %1").arg(cmd.message));
    }

    // 3. 检查是否有缺失输入 → 用文本提示替代画布拾取
    if (!cmd.missingInputs.isEmpty())
    {
        QString missingList = cmd.missingInputs.join(QStringLiteral("、"));
        QString prompt = tr("需要以下参数：%1\n请直接在对话中提供这些参数的值。")
                             .arg(missingList);

        // 关键：将参数请求写入对话历史，使下一轮 LLM 调用能获取上下文
        m_history.pushAssistant(prompt);

        emit responseReady(tr("AI"), prompt);
        return;
    }

    // 4. 直接执行
    executeParsedCommand(cmd);

    // 5. 记录 assistant 回复到对话历史
    //    （executeParsedCommand 内部已 emit responseReady，此处仅记账）
}

void AIPipeline::onModelingProviderError(const QString& error)
{
    emit errorOccurred(tr("Modeling Error"), error);
}

void AIPipeline::executeParsedCommand(const ParsedCommand& cmd)
{
    // 1. 解析 selection 中的实体引用
    if (m_contextResolver)
    {
        ResolvedSelection sel = m_contextResolver->resolve(cmd.selection);
        if (!sel.ok && cmd.selection.mode != SelectionMode::None)
        {
            emit errorOccurred(tr("Context Error"),
                               tr("Failed to resolve selection: %1").arg(sel.errorMessage));
            return;
        }
        if (!sel.explanation.isEmpty())
        {
            emit responseReady(tr("System"), sel.explanation);
        }
    }

    // 2. 执行命令
    const QString execResult = executeCommand(cmd);
    emit responseReady(tr("AI"), execResult);
}

// ============================================================================
// Modeling prompt 构建
// ============================================================================

QString AIPipeline::buildModelingSystemPrompt()
{
    // 注意：prompt 内容使用 QStringLiteral 而非 tr()，
    // 因为这些是发给 LLM 的指令，不应随 UI 语言切换而变化。
    //
    // 此方法只返回 system 指令部分。
    // 用户输入由 handleUserInput_Modeling() 通过对话历史的 user 消息传递。

    QString prompt;

    prompt += QStringLiteral(
        "You are YiCAD's CAD modeling command parser.\n"
        "Convert natural language CAD commands into a single JSON object.\n"
        "Output ONLY the JSON object, no markdown fences, no explanations.\n\n"

        "The JSON must have these fields:\n"
        "- intent: (string) one of: draw_point, draw_line, draw_circle, "
        "draw_rectangle, draw_ellipse\n"
        "- selection: (object) with \"mode\" field. mode values:\n"
        "    \"none\" (for creating new entities),\n"
        "    \"current_selection\" (use currently selected entities),\n"
        "    \"last_created\" (use the most recently created entity),\n"
        "    \"all\" (operate on all entities)\n"
        "- params: (object) geometric parameters. Use these keys:\n"
        "    center: [x, y], radius: number\n"
        "    start: [x, y], end: [x, y]\n"
        "    corner1: [x, y], corner2: [x, y]\n"
        "    offset: [dx, dy], distance: number\n"
        "    point: [x, y]\n"
        "- missing_inputs: (array of strings) inputs that need more information\n"
        "- needs_confirmation: (boolean) reserved for future use\n"
        "- message: (string) human-readable description of the operation\n\n"

        "CRITICAL RULES:\n"
        "1. NEVER invent entity IDs. Use selection modes instead.\n"
        "2. When entity selection is needed, set selection.mode to "
        "\"current_selection\" and tell user in message.\n"
        "3. If coordinates or points are unspecified, list them in missing_inputs "
        "AND clearly state what is missing in the message field.\n"
        "4. Use only the intent values listed above.\n\n"

        "Output the JSON object only, no other text.");

    return prompt;
}

// ============================================================================
// 命令执行分发
// ============================================================================

QString AIPipeline::executeCommand(const ParsedCommand& cmd)
{
    // 记录 assistant 回复到对话历史
    m_history.pushAssistant(cmd.message);

    // 根据 intent 类型分发到不同的执行器
    // 绘制类 intent → DirectEntityExecutor
    // 修改类 intent → 暂不支持，返回提示信息

    const auto intent = cmd.intent;

    // ---- 绘制类 ----
    switch (intent)
    {
    case CommandIntent::DrawPoint:
    case CommandIntent::DrawLine:
    case CommandIntent::DrawCircle:
    case CommandIntent::DrawRectangle:
    case CommandIntent::DrawEllipse:
    {
        if (!m_drawExecutor)
        {
            return tr("Drawing executor is not available (no document open).");
        }

        const ExecutorResult result = m_drawExecutor->execute(cmd);
        if (!result.success)
        {
            return tr("Drawing failed: %1").arg(result.errorMessage);
        }

        // 构造成功消息
        QString msg = tr("Drawing completed: %1").arg(cmd.message);
        if (!result.createdEntities.isEmpty())
        {
            msg += QStringLiteral("\n");
            msg += tr("Created %1 entity(s):").arg(result.createdEntities.size());
            for (const auto& e : result.createdEntities)
            {
                msg += QStringLiteral("\n  • %1 [%2]")
                           .arg(e.entityType)
                           .arg(QString::fromStdString(e.entityId.asString()));
            }
        }
        return msg;
    }

    // ---- 暂不支持的意图 ----
    default:
        return tr("Command intent '%1' is not yet supported by the executors.")
            .arg(intentToString(intent));
    }
}
