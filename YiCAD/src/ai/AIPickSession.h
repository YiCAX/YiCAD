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

/// @file AIPickSession.h
/// @brief AI 补输入会话 —— 当 ParsedCommand 存在 missing_inputs 时，
///        管理多次画布拾取的顺序执行，将结果回填到 params 中。
///
/// 职责：
///   - 接收 ParsedCommand（其中 missingInputs 非空）
///   - 根据 missingInputs 生成 PickTask 队列
///   - 依次创建 AIPickAction 推入事件栈
///   - 收集每次拾取的结果并累加到 params
///   - 全部完成后通过回调通知上层
///
/// 使用方式：
///   @code
///     // 假设 cmd 是从 LLMCommandBridge::parse() 得到的，且 missingInputs 非空
///     auto* session = new AIPickSession(doc, docView, this);
///     session->start(cmd, [](bool ok, const QJsonObject& completedParams) {
///         if (ok) {
///             // 将 completedParams 交给执行器
///             DirectEntityExecutor exec(doc);
///             ParsedCommand filledCmd = cmd;
///             filledCmd.params = completedParams;
///             filledCmd.missingInputs.clear();
///             exec.execute(filledCmd);
///         } else {
///             // 用户取消了补输入
///         }
///     });
///   @endcode
///
/// 已知限制（当前版本）：
///   - 不支持并行拾取（每次只推一个 AIPickAction）
///   - 不支持跳过某个拾取项（用户必须依次完成或取消全部）
///   - 会话内不处理上下文解析（仅收集原始坐标/实体 ID）

#ifndef AIPICKSESSION_H
#define AIPICKSESSION_H

#include <QObject>
#include <QJsonObject>
#include <QVector>

#include <functional>

#include "AIPickTypes.h"
#include "LLMCommandBridge.h"  // for ParsedCommand

class DmDocument;
class GuiDocumentView;

/// @brief AI 补输入会话
///
/// 无模态：通过 AIPickAction 在现有事件栈中工作。
/// 一个实例对应一次完整的 missing_inputs 补全流程。
class AIPickSession : public QObject
{
    Q_OBJECT

public:
    /// @brief 会话完成回调类型
    /// @param success         全部拾取是否成功（用户取消则为 false）
    /// @param completedParams 补全后的 params（含原有参数 + 拾取结果）
    using CompletionCallback = std::function<void(bool success, const QJsonObject& completedParams)>;

    /// @brief 构造函数
    /// @param doc     文档指针
    /// @param docView 文档视图指针
    /// @param parent  父 QObject（可选）
    explicit AIPickSession(DmDocument*      doc,
                           GuiDocumentView* docView,
                           QObject*         parent = nullptr);

    ~AIPickSession() override = default;

    /// @brief 启动补输入流程
    /// @param cmd      包含 missingInputs 的已解析命令
    /// @param callback 流程完成回调（成功或取消时各调用一次）
    ///
    /// 调用后即开始：推入第一个 AIPickAction。
    /// 会话期间 isActive() 返回 true。
    void start(const ParsedCommand& cmd, CompletionCallback callback);

    /// @brief 是否正在进行补输入
    bool isActive() const;

    /// @brief 取消当前补输入流程（用户中途取消）
    ///
    /// 取消后回调会被触发（success=false）。
    void cancel();

private slots:
    /// @brief 处理下一个拾取任务（通过 QueuedConnection 延迟调用）
    void doProcessNext();

private:
    /// @brief 单次拾取完成后的回调（由 AIPickAction 调用）
    void onPickComplete(const PickResult& result);

    /// @brief \xe6\xa0\xb9\xe6\x8d\xae PickType \xe7\x94\x9f\xe6\x88\x90\xe9\xbb\x98\xe8\xae\xa4\xe6\x8f\x90\xe7\xa4\xba\xe6\x96\x87\xe6\x9c\xac\xef\xbc\x88\xe4\xbd\xbf\xe7\x94\xa8 tr()\xef\xbc\x89
    QString defaultPickHint(PickType pickType, const QString& key) const;

    DmDocument*       m_doc   = nullptr;
    GuiDocumentView*  m_view  = nullptr;

    QVector<PickTask> m_queue;               ///< 待处理的拾取任务队列
    int               m_currentIndex = 0;    ///< 当前处理到的索引
    QJsonObject       m_accumulatedParams;   ///< 累积的 params（含原有参数 + 已拾取结果）
    CompletionCallback m_completionCallback; ///< 流程完成回调
    bool              m_active = false;      ///< 是否正在活动
};

#endif // AIPICKSESSION_H
