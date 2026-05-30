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

/// @file ConversationBrowser.h
/// @brief 会话历史浏览对话框 —— 列出、加载、删除历史会话
///
/// 使用方式：
///   @code
///     ConversationStore store;
///     ConversationBrowser browser(&store, parent);
///     connect(&browser, &ConversationBrowser::sessionSelected,
///             this, &MyClass::onSessionSelected);
///     browser.exec();
///   @endcode

#ifndef CONVERSATIONBROWSER_H
#define CONVERSATIONBROWSER_H

#include <QDialog>

class QListWidget;
class QPushButton;
class ConversationStore;
struct SessionMeta;

class ConversationBrowser : public QDialog
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param store 会话存储指针（非 owning）
    /// @param parent 父窗口
    explicit ConversationBrowser(ConversationStore* store,
                                 QWidget* parent = nullptr);

signals:
    /// @brief 用户选择加载某个会话
    /// @param sessionId 会话 ID
    void sessionSelected(const QString& sessionId);

    /// @brief 用户删除某个会话后发射
    /// @param sessionId 被删除的会话 ID
    void sessionDeleted(const QString& sessionId);

private slots:
    void onLoadClicked();
    void onDeleteClicked();

private:
    void refreshList();

    QListWidget*        m_sessionList = nullptr;
    QPushButton*        m_loadButton  = nullptr;
    QPushButton*        m_deleteButton = nullptr;
    ConversationStore*  m_store       = nullptr;  ///< 非 owning
};

#endif // CONVERSATIONBROWSER_H
