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

/// @file ConversationStore.cpp
/// @brief ConversationStore 实现 —— JSON 文件读写

#include "ConversationStore.h"
#include "ConversationHistory.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <algorithm>

// ============================================================================
// 构造
// ============================================================================

ConversationStore::ConversationStore(const QString& storageDir, QObject* parent)
    : QObject(parent)
{
    if (storageDir.isEmpty())
    {
        // 默认路径：~/.yicad/conversations/
        const QString home = QDir::homePath();
        m_storageDir = home + QStringLiteral("/.yicad/conversations");
    }
    else
    {
        m_storageDir = storageDir;
    }
}

// ============================================================================
// 公开接口
// ============================================================================

QString ConversationStore::save(const Session& session)
{
    if (!ensureStorageDir())
        return QString();

    // ---- 1. 序列化 messages 为 JSON 数组 ----
    QJsonArray messagesJson;
    for (const auto& msg : session.messages)
    {
        QJsonObject msgObj;
        msgObj[QStringLiteral("role")]      = msg.role;
        msgObj[QStringLiteral("content")]   = msg.content;
        msgObj[QStringLiteral("timestamp")] = msg.timestamp.toString(Qt::ISODate);
        messagesJson.append(msgObj);
    }

    // ---- 2. 构建根 JSON ----
    QJsonObject root;
    root[QStringLiteral("id")]          = session.meta.id;
    root[QStringLiteral("title")]       = session.meta.title;
    root[QStringLiteral("createdAt")]   = session.meta.createdAt.toString(Qt::ISODate);
    root[QStringLiteral("updatedAt")]   = session.meta.updatedAt.toString(Qt::ISODate);
    root[QStringLiteral("messages")]    = messagesJson;

    QJsonDocument doc(root);

    // ---- 3. 写入磁盘 ----
    const QString path = sessionFilePath(session.meta.id);
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        // 写入失败，返回空
        return QString();
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    // ---- 4. 上限清理 ----
    enforceSessionLimit();

    return path;
}

Session ConversationStore::load(const QString& sessionId) const
{
    Session session;

    const QString path = sessionFilePath(sessionId);
    QFile file(path);
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        return session;  // 返回空的 Session
    }

    const QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        return session;
    }

    const QJsonObject root = doc.object();

    // ---- 解析 meta ----
    session.meta.id          = root[QStringLiteral("id")].toString();
    session.meta.title       = root[QStringLiteral("title")].toString();
    session.meta.createdAt   = QDateTime::fromString(
        root[QStringLiteral("createdAt")].toString(), Qt::ISODate);
    session.meta.updatedAt   = QDateTime::fromString(
        root[QStringLiteral("updatedAt")].toString(), Qt::ISODate);

    // ---- 解析 messages ----
    const QJsonArray messagesJson = root[QStringLiteral("messages")].toArray();
    session.messages.reserve(messagesJson.size());
    for (const auto& val : messagesJson)
    {
        const QJsonObject msgObj = val.toObject();
        MessageEntry msg;
        msg.role      = msgObj[QStringLiteral("role")].toString();
        msg.content   = msgObj[QStringLiteral("content")].toString();
        msg.timestamp = QDateTime::fromString(
            msgObj[QStringLiteral("timestamp")].toString(), Qt::ISODate);
        session.messages.append(msg);
    }

    session.meta.messageCount = session.messages.size();
    return session;
}

QVector<SessionMeta> ConversationStore::list() const
{
    QVector<SessionMeta> result;

    if (!ensureStorageDir())
        return result;

    QDir dir(m_storageDir);
    const QFileInfoList entries = dir.entryInfoList(
        { QStringLiteral("*.json") }, QDir::Files, QDir::Time);  // 按修改时间倒序

    for (const auto& fi : entries)
    {
        // 从文件名提取 sessionId（去掉 .json 后缀）
        const QString sessionId = fi.completeBaseName();

        // 快速读取 meta 信息（不加载全部消息，性能优先）
        QFile file(fi.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly))
            continue;

        const QByteArray data = file.readAll();
        file.close();

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error != QJsonParseError::NoError)
            continue;

        const QJsonObject root = doc.object();

        SessionMeta meta;
        meta.id          = root[QStringLiteral("id")].toString(sessionId);
        meta.title       = root[QStringLiteral("title")].toString();
        meta.createdAt   = QDateTime::fromString(
            root[QStringLiteral("createdAt")].toString(), Qt::ISODate);
        meta.updatedAt   = QDateTime::fromString(
            root[QStringLiteral("updatedAt")].toString(), Qt::ISODate);
        meta.messageCount = root[QStringLiteral("messages")].toArray().size();

        result.append(meta);
    }

    return result;
}

bool ConversationStore::remove(const QString& sessionId)
{
    const QString path = sessionFilePath(sessionId);
    QFile file(path);
    if (!file.exists())
        return true;  // 不存在也算成功

    return file.remove();
}

QString ConversationStore::storageDir() const
{
    return m_storageDir;
}

void ConversationStore::setMaxSessions(int max)
{
    if (max < 1)
        max = 1;
    m_maxSessions = max;
}

// ============================================================================
// 内部方法
// ============================================================================

QString ConversationStore::sessionFilePath(const QString& sessionId) const
{
    return m_storageDir + QStringLiteral("/") + sessionId + QStringLiteral(".json");
}

bool ConversationStore::ensureStorageDir() const
{
    QDir dir(m_storageDir);
    if (dir.exists())
        return true;

    if (dir.mkpath(QStringLiteral(".")))
        return true;

    // 降级到临时目录
    const QString tempDir = QStandardPaths::writableLocation(
        QStandardPaths::TempLocation) + QStringLiteral("/yicad_conversations");

    // 注意：m_storageDir 是 const 方法中无法修改的非 const 成员。
    // 降级路径由上层控制：存储目录创建失败时返回 false，
    // 调用方应检查返回值并决定是否降级。
    QDir tempPath(tempDir);
    if (tempPath.mkpath(QStringLiteral(".")))
        return true;

    return false;
}

void ConversationStore::enforceSessionLimit()
{
    QDir dir(m_storageDir);
    if (!dir.exists())
        return;

    QFileInfoList entries = dir.entryInfoList(
        { QStringLiteral("*.json") }, QDir::Files, QDir::Time);  // 按修改时间倒序

    // 如果未超出上限，无需清理
    if (entries.size() <= m_maxSessions)
        return;

    // 按 lastModified 升序排序（最旧的在前）
    std::sort(entries.begin(), entries.end(),
              [](const QFileInfo& a, const QFileInfo& b) {
                  return a.lastModified() < b.lastModified();
              });

    // 删除最旧的文件直到满足上限
    const int toDelete = entries.size() - m_maxSessions;
    for (int i = 0; i < toDelete && i < entries.size(); ++i)
    {
        QFile::remove(entries[i].absoluteFilePath());
    }
}
