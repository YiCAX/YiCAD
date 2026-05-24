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

/// @file AIPipeline.h
/// @brief AI 总调度器 —— 串联路由、RAG、建模桥接、执行器，实现设计文档 §4.1 的完整流程
///
/// 职责：
///   - 持有并管理 AIIntentRouter / RAGPipeline / LLMCommandBridge / Executors
///   - 接收用户输入，调用 Router 分类，分发到 QA 或 Modeling 链路
///   - QA 链路：RAGPipeline 检索 + 模型回答 + 引用
///   - Modeling 链路：LLM 建模 prompt → JSON 解析 → ContextResolver → Executor
///   - 统一通过 responseReady / errorOccurred 信号向 UI 层回报
///
/// 使用方式：
///   @code
///     auto* pipeline = new AIPipeline(docsDir, readmePath, keywordsPath,
///                                     doc, docView, this);
///     connect(pipeline, &AIPipeline::responseReady,
///             dialog, &AIDialog::appendMessage);
///     connect(pipeline, &AIPipeline::errorOccurred,
///             dialog, &AIDialog::appendMessage);
///     pipeline->handleUserInput("怎么画圆", "auto");
///   @endcode
///
/// 已知限制（当前版本）：
///   - DmDocument* / GuiDocumentView* 在构造时传入，不支持运行时切换文档
///   - missing_inputs 仅做提示展示，不触发 AIPickSession 补输入交互
///   - 混合意图 (Mixed) 先走 QA 回答，暂不跟进"是否执行"确认
///   - 不支持多轮对话状态（每次 handleUserInput 独立处理）

#ifndef AIPIPELINE_H
#define AIPIPELINE_H

#include <QObject>
#include <QString>
#include <memory>

class AIIntentRouter;
class RAGPipeline;
class DeepSeekProvider;
#include "LLMCommandBridge.h"
class ContextResolver;
class DirectEntityExecutor;
class ModificationExecutor;
class DmDocument;
class GuiDocumentView;

struct RAGAnswer;
struct RouterResult;
struct ParsedCommand;

class AIPipeline : public QObject
{
    Q_OBJECT

public:
    /// @brief 构造函数 —— 初始化所有子模块
    /// @param docsDir          docs/ 目录路径（用于 RAG 知识库）
    /// @param readmePath       README.md 路径（用于 RAG 知识库）
    /// @param keywordsJsonPath intent_keywords.json 路径（用于路由关键词加载）
    /// @param doc              当前文档指针（用于建模执行器，可为 nullptr）
    /// @param docView           当前文档视图指针（用于建模执行器 UI 刷新，可为 nullptr）
    /// @param parent            父 QObject
    explicit AIPipeline(const QString& docsDir,
                        const QString& readmePath,
                        const QString& keywordsJsonPath,
                        DmDocument* doc,
                        GuiDocumentView* docView,
                        QObject* parent = nullptr);

    /// @brief 析构函数
    ~AIPipeline() override;

    /// @brief 整体是否就绪（路由 + RAG 均已初始化）
    bool isReady() const;

    /// @brief 处理一条用户输入（从 AIDialog 的 sendRequested 信号驱动）
    /// @param text 用户原始输入文本
    /// @param mode 路由模式 token："qa" / "modeling" / "auto"
    ///
    /// 流程：
    ///   1. AIIntentRouter::route() 分类
    ///   2. QA  → RAGPipeline::query()
    ///   3. Modeling → buildModelingPrompt → DeepSeekProvider → LLMCommandBridge → execute
    ///   4. Mixed → 先走 QA，额外提示可执行
    ///   5. Uncertain → 提示用户澄清意图
    void handleUserInput(const QString& text, const QString& mode);

signals:
    /// @brief 管线产出回复（QA 回答 / 建模执行结果 / 路由提示）
    /// @param sender 发送者标识（"AI" / "System"）
    /// @param text   回复文本
    void responseReady(const QString& sender, const QString& text);

    /// @brief 管线执行中发生错误
    /// @param sender 发送者标识（"Error"）
    /// @param text   错误描述
    void errorOccurred(const QString& sender, const QString& text);

private slots:
    // ---- QA 链路回调 ----
    void onRAGAnswer(const RAGAnswer& answer);
    void onRAGError(const QString& error);

    // ---- Modeling 链路回调 ----
    void onModelingProviderResponse(const QString& responseText);
    void onModelingProviderError(const QString& error);

private:
    // ---- 链路内部分发 ----
    /// @brief QA 链路：展示路由信息并调用 RAGPipeline
    void handleUserInput_QA(const RouterResult& route, const QString& text);

    /// @brief Modeling 链路：构建 prompt 并调用 LLM
    void handleUserInput_Modeling(const RouterResult& route, const QString& text);

    // ---- 模式映射 ----
    /// @brief 将 AIDialog 的模式索引转为 Router token
    static QString modeToRouterToken(int comboIndex);

    // ---- Modeling prompt 构建 ----
    /// @brief 构建发送给 LLM 的建模系统指令 + 用户输入
    static QString buildModelingPrompt(const QString& userText);

    // ---- 执行分发 ----
    /// @brief 根据 ParsedCommand::intent 分发到 DirectEntityExecutor 或 ModificationExecutor
    /// @return 人类可读的执行结果描述
    QString executeCommand(const ParsedCommand& cmd);

    // ---- 子模块 ----
    std::unique_ptr<AIIntentRouter>       m_router;          ///< 意图路由器
    std::unique_ptr<RAGPipeline>          m_ragPipeline;     ///< RAG 问答管线（内含自己的 DeepSeekProvider）
    std::unique_ptr<DeepSeekProvider>     m_modelingProvider;///< 建模链路专用 LLM Provider
    LLMCommandBridge                      m_bridge;          ///< JSON 命令桥（无状态）
    std::unique_ptr<ContextResolver>      m_contextResolver; ///< 上下文解析器
    std::unique_ptr<DirectEntityExecutor> m_drawExecutor;    ///< 直接绘图执行器
    std::unique_ptr<ModificationExecutor> m_modExecutor;     ///< 修改命令执行器

    // ---- 状态 ----
    bool m_routerReady  = false;  ///< Router 关键词数据是否已加载
    bool m_ragReady     = false;  ///< RAG 知识库是否已索引
    bool m_modelingReady = false; ///< 建模链路是否可用（doc + docView 均非空）
    QString m_lastUserText;       ///< 最近一次用户输入（调试用）
    QString m_lastMode;           ///< 最近一次模式（调试用）
};

#endif // AIPIPELINE_H
