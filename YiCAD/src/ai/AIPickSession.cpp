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

/// @file AIPickSession.cpp
/// @brief AIPickSession 实现

#include "AIPickSession.h"

#include "AIPickAction.h"
#include "GuiDocumentView.h"
#include "GuiDialogFactory.h"

#include <QMetaObject>
#include <QPointer>

// ============================================================================
// 构造 / 析构
// ============================================================================

AIPickSession::AIPickSession(DmDocument*      doc,
                             GuiDocumentView* docView,
                             QObject*         parent)
    : QObject(parent)
    , m_doc(doc)
    , m_view(docView)
{
}

// ============================================================================
// 公开接口
// ============================================================================

void AIPickSession::start(const ParsedCommand& cmd, CompletionCallback callback)
{
    if (m_active) {
        // 已有活动会话，先取消旧的
        cancel();
    }

    m_completionCallback = std::move(callback);
    m_accumulatedParams  = cmd.params;   // 保留原有参数
    m_currentIndex       = 0;
    m_queue.clear();

    // 根据 missingInputs 生成 PickTask 队列
    for (const QString& key : cmd.missingInputs) {
        PickTask task;
        task.key      = key;
        task.pickType = pickTypeFromKey(key);
        task.hint     = this->defaultPickHint(task.pickType, key);
        m_queue.append(task);
    }

    if (m_queue.isEmpty()) {
        // 没有待拾取项，直接完成
        m_active = true;
        if (m_completionCallback) {
            m_completionCallback(true, m_accumulatedParams);
        }
        m_active = false;
        return;
    }

    m_active = true;
    GUIDIALOGFACTORY->commandMessage(
        tr("AI needs additional input (%1 items)").arg(m_queue.size()));

    // 延迟到下一事件循环开始，避免与当前事件栈冲突
    QMetaObject::invokeMethod(this, "doProcessNext", Qt::QueuedConnection);
}

bool AIPickSession::isActive() const
{
    return m_active;
}

void AIPickSession::cancel()
{
    if (!m_active) return;

    m_active = false;
    m_queue.clear();

    if (m_completionCallback) {
        auto cb = m_completionCallback;
        m_completionCallback = nullptr;
        cb(false, QJsonObject());
    }

    GUIDIALOGFACTORY->commandMessage(tr("AI input supplement cancelled"));
}

// ============================================================================
// 内部流程（doProcessNext 作为槽，通过 QueuedConnection 调用）
// ============================================================================

void AIPickSession::doProcessNext()
{
    if (!m_active) return;

    if (m_currentIndex >= m_queue.size()) {
        // 全部完成
        m_active = false;
        if (m_completionCallback) {
            auto cb = m_completionCallback;
            m_completionCallback = nullptr;
            cb(true, m_accumulatedParams);
        }
        GUIDIALOGFACTORY->commandMessage(
            tr("AI input supplement complete"));
        return;
    }

    const PickTask& task = m_queue.at(m_currentIndex);

    // 使用 QPointer 防止 session 先于 action 被销毁导致的悬空指针
    QPointer<AIPickSession> guard(this);

    auto* action = new AIPickAction(
        m_doc, m_view, task,
        [guard, this](const PickResult& result) {
            if (!guard) return;  // session 已被销毁，静默忽略
            // onPickComplete 内部会通过 QueuedConnection 延迟调用 doProcessNext，
            // 确保 AIPickAction 的 finish/cleanUp 完全结束后再推入下一个 action。
            onPickComplete(result);
        });

    m_view->setCurrentAction(action);

    // 显示进度信息
    GUIDIALOGFACTORY->commandMessage(
        tr("[%1/%2] %3")
            .arg(m_currentIndex + 1)
            .arg(m_queue.size())
            .arg(task.hint));
}

void AIPickSession::onPickComplete(const PickResult& result)
{
    if (!m_active) return;

    if (!result.success) {
        // 用户取消本次拾取 → 取消整个流程
        cancel();
        return;
    }

    // 将拾取结果回填到累积的 params 中
    const QString& key = m_queue.at(m_currentIndex).key;
    m_accumulatedParams.insert(key, result.toJsonValue());

    // 前进到下一个（延迟执行，避免在 AIPickAction 的 mouseReleaseEvent 栈中
    // 创建新 action 导致 use-after-free）
    m_currentIndex++;
    QMetaObject::invokeMethod(this, "doProcessNext", Qt::QueuedConnection);
}

// ============================================================================
// defaultPickHint -- QObject member, can use tr()
// ============================================================================

QString AIPickSession::defaultPickHint(PickType pickType, const QString& key) const
{
    switch (pickType) {
    case PickType::PickPoint:
        return tr("Specify %1 on canvas").arg(key);
    case PickType::PickEntity:
        return tr("Select %1 entity").arg(key);
    }
    return tr("Operate on canvas");
}
