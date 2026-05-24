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

/// @file AIPickTypes.cpp
/// @brief AIPickTypes 实现

#include "AIPickTypes.h"

#include <QJsonArray>
#include <QSet>

// ============================================================================
// PickType 映射
// ============================================================================

/// 实体拾取关键词集合 —— 如果 missing_inputs 的键名包含这些子串，归类为 PickEntity
static const QSet<QString> kEntityPickKeys = {
    QStringLiteral("entity"),
    QStringLiteral("target_entity"),
    QStringLiteral("boundary"),
    QStringLiteral("object"),
    QStringLiteral("selection"),
    QStringLiteral("trim_side"),
    QStringLiteral("extend_to"),
};

PickType pickTypeFromKey(const QString& key)
{
    // 精确匹配实体拾取
    if (kEntityPickKeys.contains(key.toLower())) {
        return PickType::PickEntity;
    }

    // 模糊匹配：键名中包含 "entity" 或 "ent" 子串
    const QString lower = key.toLower();
    if (lower.contains(QStringLiteral("entity"))
        || lower.contains(QStringLiteral("_ent"))) {
        return PickType::PickEntity;
    }

    // 其他一律默认为点拾取
    return PickType::PickPoint;
}

// ============================================================================
// PickResult → JSON
// ============================================================================

QJsonValue PickResult::toJsonValue() const
{
    if (!success) {
        return QJsonValue::Null;
    }

    switch (pickType) {
    case PickType::PickPoint: {
        QJsonArray arr;
        arr.append(point.x);
        arr.append(point.y);
        return arr;
    }
    case PickType::PickEntity: {
        return QJsonValue(QString::fromStdString(entityId.asString()));
    }
    }

    return QJsonValue::Null;
}
