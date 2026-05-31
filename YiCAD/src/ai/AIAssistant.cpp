/*
 * Copyright (C) 2026 YiCAD
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

/// @file AIAssistant.cpp
/// @brief AI 助手控制器实现

#include "AIAssistant.h"

#include "AIDialog.h"
#include "AIPipeline.h"
#include "ConversationHistory.h"
#include "ConversationStore.h"
#include "ConversationBrowser.h"
#include "LLMSettingsPage.h"
#include "DmSystem.h"

#include <QDateTime>

AIAssistant::AIAssistant(QWidget* parentWindow, QObject* parent)
    : QObject(parent)
    , m_parentWindow(parentWindow)
{
}

AIAssistant::~AIAssistant()
{
    saveCurrentSession();
    delete m_store;
}

void AIAssistant::show(DmDocument* doc, GuiDocumentView* docView)
{
    ensureCreated(doc, docView);
    m_dialog->show();
    m_dialog->raise();
    m_dialog->activateWindow();
}

void AIAssistant::ensureCreated(DmDocument* doc, GuiDocumentView* docView)
{
    if (m_dialog)
        return;

    // ---- 1. 初始化存储 ----------------
    m_store = new ConversationStore(QString(), this);

    m_dialog = new AIDialog(m_parentWindow);

    const QString appDir = DMSYSTEM->getAppDir();
    const QString docsDir = appDir + "/ai";
    const QString readmePath = appDir + "/ai/README.md";

    m_pipeline = new AIPipeline(docsDir, readmePath,
                                doc, docView, this);

    // ---- 2. 加载最近会话 ----
    loadLatestSession();

    // ---- 3. 信号连接 ----
    connect(m_dialog, &AIDialog::sendRequested,
            this, &AIAssistant::onSendRequested);

    connect(m_pipeline, &AIPipeline::responseReady,
            m_dialog, &AIDialog::appendMessage);
    connect(m_pipeline, &AIPipeline::responseReady,
            this, &AIAssistant::onPipelineResponse);

    connect(m_pipeline, &AIPipeline::errorOccurred,
            m_dialog, &AIDialog::appendMessage);
    connect(m_pipeline, &AIPipeline::errorOccurred,
            this, &AIAssistant::onPipelineResponse);

    connect(m_dialog, &AIDialog::configRequested,
            this, &AIAssistant::onConfigRequested);

    // 对话框关闭时自动保存
    connect(m_dialog, &AIDialog::finished,
            this, &AIAssistant::onDialogClosing);

    // 历史浏览
    connect(m_dialog, &AIDialog::historyRequested,
            this, &AIAssistant::onHistoryRequested);
    connect(m_dialog, &AIDialog::loadSessionRequested,
            this, &AIAssistant::onLoadSessionRequested);
}

void AIAssistant::onSendRequested(const QString& text, const QString& /*mode*/)
{
    // AI 处理期间禁用发送按钮
    m_dialog->setSendEnabled(false);

    const int idx = m_dialog->modeIndex();
    const QString token = (idx == 0) ? QStringLiteral("qa")
                        : (idx == 1) ? QStringLiteral("modeling")
                        : QStringLiteral("auto");
    m_pipeline->handleUserInput(text, token);
}

void AIAssistant::onConfigRequested()
{
    LLMSettingsPage dlg(m_dialog);
    dlg.exec();
}

void AIAssistant::onDialogClosing()
{
    saveCurrentSession();
}

void AIAssistant::onPipelineResponse(const QString& sender, const QString& /*text*/)
{
    // AI 最终回复或错误时恢复发送按钮（System 消息是中间状态）
    if (sender == QStringLiteral("AI") || sender != QStringLiteral("System"))
    {
        m_dialog->setSendEnabled(true);
    }

    // 每 5 条消息后自动保存一次
    ++m_unsavedMsgCount;
    if (m_unsavedMsgCount >= 5)
    {
        saveCurrentSession();
        m_unsavedMsgCount = 0;
    }
}

void AIAssistant::onHistoryRequested()
{
    if (!m_store || !m_dialog)
        return;

    ConversationBrowser browser(m_store, m_dialog);
    connect(&browser, &ConversationBrowser::sessionSelected,
            this, &AIAssistant::onLoadSessionRequested);

    // 监听删除：如果删的是当前会话，重置 session ID 防止后续 save 复活
    connect(&browser, &ConversationBrowser::sessionDeleted,
            this, [this](const QString& sessionId) {
        if (sessionId == m_currentSessionId)
        {
            m_currentSessionId = QStringLiteral("conv_")
                + QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss"));
            m_pipeline->history().clear();
            m_dialog->clearChatView();
            m_unsavedMsgCount = 0;
        }
    });

    browser.exec();

    // 在浏览器关闭后保存（如果当前会话还在，会被持久化；若已被删则用新 ID）
    saveCurrentSession();
}

void AIAssistant::onLoadSessionRequested(const QString& sessionId)
{
    if (!m_store || !m_pipeline || !m_dialog)
        return;

    // 先保存当前会话
    saveCurrentSession();

    // 加载目标会话
    const Session session = m_store->load(sessionId);
    if (session.meta.id.isEmpty())
        return;

    // 清空 UI 和历史
    m_dialog->clearChatView();
    m_pipeline->history().clear();

    // 加载消息到历史
    m_pipeline->history().loadFrom(session.messages);
    m_currentSessionId = session.meta.id;
    m_unsavedMsgCount = 0;

    // 恢复 UI 显示
    for (const auto& msg : session.messages)
    {
        if (msg.role == QStringLiteral("user"))
        {
            m_dialog->appendMessage(tr("User"), msg.content);
        }
        else if (msg.role == QStringLiteral("assistant"))
        {
            m_dialog->appendMessage(tr("AI"), msg.content);
        }
        else if (msg.role == QStringLiteral("system"))
        {
            m_dialog->appendMessage(tr("System"), msg.content);
        }
    }
}

// ============================================================================
// 持久化
// ============================================================================

void AIAssistant::saveCurrentSession()
{
    if (!m_store || !m_pipeline || m_currentSessionId.isEmpty())
        return;

    Session session;
    session.meta.id        = m_currentSessionId;
    session.meta.createdAt = QDateTime::currentDateTime();  // 会在 load 时覆盖
    session.meta.updatedAt = QDateTime::currentDateTime();

    // 从 ConversationHistory 获取消息并提取标题
    const auto& allMsgs = m_pipeline->history().allMessages();
    session.messages = allMsgs;
    session.meta.messageCount = allMsgs.size();

    // 自动标题：取第一条 user 消息，截断到 40 字符
    for (const auto& msg : allMsgs)
    {
        if (msg.role == QStringLiteral("user"))
        {
            QString title = msg.content.left(40).trimmed();
            // 移除换行符
            title.replace(QStringLiteral("\n"), QStringLiteral(" "));
            title.replace(QStringLiteral("\r"), QString());
            if (msg.content.length() > 40)
                title += QStringLiteral("…");
            session.meta.title = title;
            session.meta.createdAt = msg.timestamp;
            break;
        }
    }

    m_store->save(session);
}

void AIAssistant::loadLatestSession()
{
    if (!m_store || !m_pipeline)
        return;

    const auto sessions = m_store->list();
    if (sessions.isEmpty())
    {
        // 新建会话 ID
        m_currentSessionId = QStringLiteral("conv_")
            + QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss"));
        return;
    }

    // 加载最近一个会话
    const Session session = m_store->load(sessions.first().id);
    if (session.meta.id.isEmpty())
    {
        m_currentSessionId = QStringLiteral("conv_")
            + QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss"));
        return;
    }

    m_currentSessionId = session.meta.id;
    m_pipeline->history().loadFrom(session.messages);

    // 将历史消息恢复到 UI
    for (const auto& msg : session.messages)
    {
        if (msg.role == QStringLiteral("user"))
        {
            m_dialog->appendMessage(tr("User"), msg.content);
        }
        else if (msg.role == QStringLiteral("assistant"))
        {
            m_dialog->appendMessage(tr("AI"), msg.content);
        }
        else if (msg.role == QStringLiteral("system"))
        {
            m_dialog->appendMessage(tr("System"), msg.content);
        }
    }
}
