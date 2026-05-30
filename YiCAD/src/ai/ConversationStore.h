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

/// @file ConversationStore.h
/// @brief 会话持久化存储 —— 每个会话一个 JSON 文件
///
/// 职责：
///   - 将 Session 序列化为 JSON 写入磁盘
///   - 从磁盘反序列化 JSON 为 Session
///   - 列出所有可用会话（按更新时间降序）
///   - 删除指定会话
///   - 自动清理超出上限的旧会话
///
/// 存储路径：~/.yicad/conversations/<sessionId>.json
/// 降级路径：QStandardPaths::TempLocation/conversations/
///
/// 使用方式：
///   @code
///     ConversationStore store;
///     Session session;
///     session.meta.id = "conv_20260325_143022";
///     session.meta.title = "画一个圆";
///     session.messages = {...};
///     store.save(session);
///     auto list = store.list();   // 列出所有会话
///     Session loaded = store.load("conv_20260325_143022");
///   @endcode

#ifndef CONVERSATIONSTORE_H
#define CONVERSATIONSTORE_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QVector>

struct MessageEntry;

/// @brief 会话元信息（不包含消息内容）
struct SessionMeta
{
    QString   id;           ///< 唯一标识，如 "conv_20260325_143022"
    QString   title;        ///< 自动标题：取第一条 user 消息截断
    QDateTime createdAt;    ///< 创建时间
    QDateTime updatedAt;    ///< 最后更新时间
    int       messageCount = 0; ///< 消息总数
};

/// @brief 完整会话（含消息内容）
struct Session
{
    SessionMeta            meta;     ///< 元信息
    QVector<MessageEntry>  messages; ///< 消息列表（按时间升序）

    Session() = default;
};

/// @brief 会话持久化存储
///
/// 每个会话一个 JSON 文件，位于 ~/.yicad/conversations/ 下。
/// 支持保存、加载、列出、删除，以及自动清理超出上限的旧会话。
class ConversationStore : public QObject
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param storageDir 存储目录路径。空字符串 = 使用默认路径 ~/.yicad/conversations/
    /// @param parent     父 QObject
    explicit ConversationStore(const QString& storageDir = QString(),
                               QObject* parent = nullptr);

    /// @brief 保存一个会话到磁盘
    /// @param session 完整会话数据
    /// @return 保存的文件路径（含 .json）；空字符串表示失败
    QString save(const Session& session);

    /// @brief 加载一个会话
    /// @param sessionId 会话 ID（不含 .json 后缀）
    /// @return 加载结果；如果 sessionId 不存在则返回空的 Session（meta.id 为空）
    Session load(const QString& sessionId) const;

    /// @brief 列出所有可用会话（按 updatedAt 降序）
    /// @return 会话元信息列表（不含消息内容）
    QVector<SessionMeta> list() const;

    /// @brief 删除一个会话文件
    /// @param sessionId 会话 ID（不含 .json 后缀）
    /// @return true 删除成功或文件本就不存在
    bool remove(const QString& sessionId);

    /// @brief 获取当前存储目录路径
    QString storageDir() const;

    /// @brief 设置会话保留上限（默认 50）
    /// @param max 最大会话数。超出时按 updatedAt 升序删除最旧的文件
    void setMaxSessions(int max);

private:
    /// @brief 获取会话文件的完整路径
    /// @param sessionId 会话 ID
    /// @return 完整文件路径
    QString sessionFilePath(const QString& sessionId) const;

    /// @brief 确保存储目录存在
    /// @return true 目录已存在或成功创建
    bool ensureStorageDir() const;

    /// @brief 执行会话上限清理
    ///
    /// 在每次 save() 后调用。如果会话总数超过 m_maxSessions，
    /// 按 updatedAt 升序删除最旧的文件。
    void enforceSessionLimit();

    QString m_storageDir;    ///< 存储目录路径
    int     m_maxSessions = 50; ///< 保留上限
};

#endif // CONVERSATIONSTORE_H
