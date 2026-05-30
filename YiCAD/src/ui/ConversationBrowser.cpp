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

/// @file ConversationBrowser.cpp
/// @brief ConversationBrowser 实现

#include "ConversationBrowser.h"
#include "ConversationStore.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

ConversationBrowser::ConversationBrowser(ConversationStore* store,
                                         QWidget* parent)
    : QDialog(parent)
    , m_store(store)
{
    setWindowTitle(tr("Conversation History"));
    setMinimumSize(420, 350);
    resize(480, 450);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);

    // ---- 标题 ----
    auto* titleLabel = new QLabel(tr("Saved Conversations"), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    // ---- 列表 ----
    m_sessionList = new QListWidget(this);
    m_sessionList->setAlternatingRowColors(true);
    mainLayout->addWidget(m_sessionList, 1);

    // ---- 按钮行 ----
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_deleteButton = new QPushButton(tr("Delete"), this);
    m_loadButton   = new QPushButton(tr("Load"), this);
    m_loadButton->setDefault(true);

    btnLayout->addWidget(m_deleteButton);
    btnLayout->addWidget(m_loadButton);
    mainLayout->addLayout(btnLayout);

    // ---- 信号连接 ----
    connect(m_loadButton, &QPushButton::clicked,
            this, &ConversationBrowser::onLoadClicked);
    connect(m_deleteButton, &QPushButton::clicked,
            this, &ConversationBrowser::onDeleteClicked);

    // 双击列表项 = 加载
    connect(m_sessionList, &QListWidget::itemDoubleClicked,
            this, &ConversationBrowser::onLoadClicked);

    // ---- 初始填充 ----
    refreshList();
}

// ============================================================================
// 槽
// ============================================================================

void ConversationBrowser::refreshList()
{
    m_sessionList->clear();

    if (!m_store)
        return;

    const auto sessions = m_store->list();
    if (sessions.isEmpty())
    {
        m_sessionList->addItem(tr("(No saved conversations)"));
        m_loadButton->setEnabled(false);
        m_deleteButton->setEnabled(false);
        return;
    }

    m_loadButton->setEnabled(true);
    m_deleteButton->setEnabled(true);

    for (const auto& meta : sessions)
    {
        // 显示格式：日期 标题 (N 条消息)
        const QString dateStr = meta.updatedAt.toString(QStringLiteral("yyyy-MM-dd hh:mm"));
        const QString label = QStringLiteral("%1  %2  (%3)")
                                  .arg(dateStr)
                                  .arg(meta.title.isEmpty()
                                       ? tr("(Untitled)")
                                       : meta.title)
                                  .arg(tr("%1 msgs").arg(meta.messageCount));
        auto* item = new QListWidgetItem(label, m_sessionList);
        item->setData(Qt::UserRole, meta.id);  // 存储 sessionId
    }
}

void ConversationBrowser::onLoadClicked()
{
    auto* currentItem = m_sessionList->currentItem();
    if (!currentItem)
        return;

    const QString sessionId = currentItem->data(Qt::UserRole).toString();
    if (sessionId.isEmpty())
        return;

    emit sessionSelected(sessionId);
    accept();  // 关闭对话框
}

void ConversationBrowser::onDeleteClicked()
{
    auto* currentItem = m_sessionList->currentItem();
    if (!currentItem)
        return;

    const QString sessionId = currentItem->data(Qt::UserRole).toString();
    if (sessionId.isEmpty())
        return;

    // 确认对话框
    const QString title = currentItem->text().section(QStringLiteral("  "), 1, 1);
    const int ret = QMessageBox::question(
        this,
        tr("Delete Conversation"),
        tr("Are you sure you want to delete this conversation?\n\n%1").arg(title),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (ret != QMessageBox::Yes)
        return;

    if (m_store && m_store->remove(sessionId))
    {
        emit sessionDeleted(sessionId);
        refreshList();
    }
}
