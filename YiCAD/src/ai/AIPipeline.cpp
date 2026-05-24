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

#include "AIIntentRouter.h"
#include "ContextResolver.h"
#include "DeepSeekProvider.h"
#include "DirectEntityExecutor.h"
#include "LLMCommandBridge.h"
#include "ModificationExecutor.h"
#include "RAGPipeline.h"

#include <QJsonArray>
#include <QJsonObject>

// ============================================================================
// 构造 / 析构
// ============================================================================

AIPipeline::AIPipeline(const QString& docsDir,
                       const QString& readmePath,
                       const QString& keywordsJsonPath,
                       DmDocument* doc,
                       GuiDocumentView* docView,
                       QObject* parent)
    : QObject(parent)
    , m_router(new AIIntentRouter(this))
    , m_ragPipeline(new RAGPipeline(this))
    , m_modelingProvider(new DeepSeekProvider(this))
    , m_bridge()
    , m_contextResolver(doc ? new ContextResolver(doc) : nullptr)
    , m_drawExecutor(doc ? new DirectEntityExecutor(doc) : nullptr)
    , m_modExecutor((doc && docView) ? new ModificationExecutor(doc, docView) : nullptr)
{
    // ---- 1. 初始化 Router（加载关键词 JSON） ----
    m_routerReady = m_router->loadKeywordsFromJson(keywordsJsonPath);
    if (!m_routerReady)
    {
        emit errorOccurred(tr("Error"),
                           tr("Failed to load intent keywords from: %1").arg(keywordsJsonPath));
    }

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

    // ---- Router 未就绪 ----
    if (!m_routerReady)
    {
        emit errorOccurred(tr("Error"),
                           tr("Intent router is not initialized. "
                              "Please check intent_keywords.json."));
        return;
    }

    // ---- 1. 路由分类 ----
    const RouterResult route = m_router->route(text, mode);

    // ---- 2. 分发 ----
    switch (route.intent)
    {
    case IntentType::QA:
        handleUserInput_QA(route, text);
        break;

    case IntentType::Modeling:
        handleUserInput_Modeling(route, text);
        break;

    case IntentType::Mixed:
        // 混合意图：先走 QA 回答，额外提示可执行
        emit responseReady(tr("System"),
                           tr("Detected mixed intent (%1). "
                              "Providing help answer first. "
                              "Switch to Modeling mode if you want to execute.")
                               .arg(route.reasoning));
        handleUserInput_QA(route, text);
        break;

    case IntentType::Uncertain:
    default:
        emit responseReady(tr("System"),
                           tr("Unable to determine your intent. %1\n\n"
                              "Try:\n"
                              "  • QA mode for questions like \"how to trim\"\n"
                              "  • Modeling mode for commands like \"draw a circle\"")
                               .arg(route.suggestedAction));
        break;
    }
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

    // 构建 modeling prompt 并发送给 LLM
    const QString prompt = buildModelingPrompt(text);
    m_modelingProvider->sendMessage(prompt);
}

void AIPipeline::onModelingProviderResponse(const QString& responseText)
{
    // 1. 解析 LLM 返回的 JSON
    const ParsedCommand cmd = m_bridge.parse(responseText);

    if (!cmd.ok)
    {
        // 解析失败：展示原始 LLM 回复 + 错误原因
        emit errorOccurred(tr("Parse Error"),
                           tr("Failed to parse modeling command from LLM response:\n%1\n\n"
                              "Raw response:\n%2")
                               .arg(cmd.errorMessage)
                               .arg(responseText.left(500)));
        return;
    }

    // 2. 检查是否需要确认
    if (cmd.needsConfirmation)
    {
        emit responseReady(tr("System"),
                           tr("⚠ High-risk operation detected: %1\n\n"
                              "Please review and confirm before execution.")
                               .arg(cmd.message));
        // 首版不实现确认交互，仅提示
        return;
    }

    // 3. 检查是否有缺失输入
    if (!cmd.missingInputs.isEmpty())
    {
        QString missingList = cmd.missingInputs.join(QStringLiteral(", "));
        emit responseReady(tr("System"),
                           tr("Command parsed, but some inputs are missing: %1\n\n"
                              "Parsed command: %2\n"
                              "Please provide these inputs manually or refine your command.")
                               .arg(missingList)
                               .arg(cmd.message));
        // 首版不触发 AIPickSession，仅提示
        return;
    }

    // 4. 解析 selection 中的实体引用
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

    // 5. 执行命令
    const QString execResult = executeCommand(cmd);
    emit responseReady(tr("AI"), execResult);
}

void AIPipeline::onModelingProviderError(const QString& error)
{
    emit errorOccurred(tr("Modeling Error"), error);
}

// ============================================================================
// Modeling prompt 构建
// ============================================================================

QString AIPipeline::buildModelingPrompt(const QString& userText)
{
    // 注意：prompt 内容使用 QStringLiteral 而非 tr()，
    // 因为这些是发给 LLM 的指令，不应随 UI 语言切换而变化。

    QString prompt;

    prompt += QStringLiteral(
        "[System Instruction]\n"
        "You are YiCAD's CAD modeling command parser.\n"
        "Convert natural language CAD commands into a single JSON object.\n"
        "Output ONLY the JSON object, no markdown fences, no explanations.\n\n"

        "The JSON must have these fields:\n"
        "- intent: (string) one of: draw_point, draw_line, draw_circle, "
        "draw_rectangle, draw_ellipse, move, copy, delete, offset, trim\n"
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
        "- missing_inputs: (array of strings) inputs that need user interaction\n"
        "- needs_confirmation: (boolean) true for delete or trim operations\n"
        "- message: (string) human-readable description of the operation\n\n"

        "CRITICAL RULES:\n"
        "1. NEVER invent entity IDs. Use selection modes instead.\n"
        "2. When entity selection is needed, set selection.mode to "
        "\"current_selection\" and tell user in message.\n"
        "3. For delete operations, set needs_confirmation to true.\n"
        "4. If coordinates are unspecified, list them in missing_inputs.\n"
        "5. Use only the intent values listed above.\n\n");

    prompt += QStringLiteral("[User Command]\n");
    prompt += userText;
    prompt += QStringLiteral("\n\n[Output JSON only]");

    return prompt;
}

// ============================================================================
// 命令执行分发
// ============================================================================

QString AIPipeline::executeCommand(const ParsedCommand& cmd)
{
    // 根据 intent 类型分发到不同的执行器
    // 绘制类 intent → DirectEntityExecutor
    // 修改类 intent → ModificationExecutor

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

    // ---- 修改类 ----
    case CommandIntent::Delete:
    case CommandIntent::Move:
    case CommandIntent::Copy:
    case CommandIntent::Offset:
    case CommandIntent::Trim:
    {
        if (!m_modExecutor)
        {
            return tr("Modification executor is not available (no document open).");
        }

        const ModificationResult result = m_modExecutor->execute(cmd);
        if (!result.success)
        {
            return tr("Modification failed: %1").arg(result.errorMessage);
        }

        // 构造成功消息
        QString msg = tr("Modification completed: %1").arg(cmd.message);
        msg += QStringLiteral("\n");
        msg += tr("Affected %1 entity(s).").arg(result.affectedCount);
        return msg;
    }

    // ---- 暂不支持的意图 ----
    default:
        return tr("Command intent '%1' is not yet supported by the executors.")
            .arg(intentToString(intent));
    }
}
