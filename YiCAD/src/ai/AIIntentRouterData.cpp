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

/// @file AIIntentRouterData.cpp
/// @brief 意图分类关键词数据的 JSON 加载器

#include "AIIntentRouterData.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

// ---------------------------------------------------------------------------
// 辅助函数：解析 {word, weight} 对象数组
// ---------------------------------------------------------------------------

static std::vector<KeywordEntry> parseKeywordArray(const QJsonArray& arr)
{
    std::vector<KeywordEntry> result;
    result.reserve(static_cast<size_t>(arr.size()));

    for (const QJsonValue& v : arr)
    {
        const QJsonObject obj = v.toObject();
        KeywordEntry entry;
        entry.word   = obj["word"].toString().toStdString();
        entry.weight = static_cast<float>(obj["weight"].toDouble());
        result.push_back(std::move(entry));
    }
    return result;
}

// ---------------------------------------------------------------------------
// 辅助函数：解析字符串数组
// ---------------------------------------------------------------------------

static std::vector<std::string> parseStringArray(const QJsonArray& arr)
{
    std::vector<std::string> result;
    result.reserve(static_cast<size_t>(arr.size()));

    for (const QJsonValue& v : arr)
    {
        result.push_back(v.toString().toStdString());
    }
    return result;
}

// ---------------------------------------------------------------------------
// 公开加载器
// ---------------------------------------------------------------------------

bool loadIntentKeywordsFromJson(const QString& filePath, IntentKeywords& out)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    const QByteArray raw = file.readAll();
    file.close();

    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(raw, &parseErr);
    if (doc.isNull())
    {
        return false;
    }

    const QJsonObject root = doc.object();

    out.qaKeywords       = parseKeywordArray(root["qa_keywords"].toArray());
    out.modelingKeywords = parseKeywordArray(root["modeling_keywords"].toArray());
    out.entityNouns      = parseStringArray(root["entity_nouns"].toArray());
    out.topicMarkers     = parseStringArray(root["topic_markers"].toArray());

    out.questionStartMarkers    = parseStringArray(root["question_start_markers"].toArray());
    out.questionEndMarkers      = parseStringArray(root["question_end_markers"].toArray());
    out.questionContainsMarkers = parseStringArray(root["question_contains_markers"].toArray());

    out.valid = !out.qaKeywords.empty() || !out.modelingKeywords.empty();
    return out.valid;
}
