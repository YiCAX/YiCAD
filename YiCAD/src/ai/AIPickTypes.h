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

/// @file AIPickTypes.h
/// @brief AIPickSession / AIPickAction 共享类型定义
///
/// 定义拾取类型、拾取结果、拾取任务等数据结构，
/// 供 AIPickSession（编排层）和 AIPickAction（交互层）共同使用。

#ifndef AIPICKTYPES_H
#define AIPICKTYPES_H

#include <QString>
#include "DmId.h"
#include "DmVector.h"
#include <QJsonValue>

// ============================================================================
// 拾取类型
// ============================================================================

/// @brief 画布拾取类型 —— 决定用户在画布上需要做什么
enum class PickType
{
    PickPoint,   ///< 在画布上点取一个坐标
    PickEntity,  ///< 在画布上点选一个实体
};

/// @brief 将 missing_inputs 中的键名映射为 PickType
///
/// 映射规则（当前版本）：
///   - 几何点参数键名（center, start, end, position, corner1, corner2,
///     origin, target, base, reference 等） → PickPoint
///   - 实体选择参数键名（entity, target_entity, boundary, object 等）
///     → PickEntity
///   - 无法识别 → PickPoint（默认回退）
///
/// @param key  missing_inputs 中的字段名（如 "center", "entity"）
/// @return 对应的 PickType
PickType pickTypeFromKey(const QString& key);

// ============================================================================
// 拾取结果
// ============================================================================

/// @brief 单次拾取的结果
///
/// 根据 PickType 不同，结果字段的语义不同：
///   - PickPoint  → point 有效，entityId 无效
///   - PickEntity → entityId 有效，point 为实体上的命中点
struct PickResult
{
    bool    success  = false;   ///< 拾取是否成功（用户取消则为 false）
    PickType pickType = PickType::PickPoint;  ///< 本次拾取的类型

    // ---- 点结果 ----
    DmVector point;              ///< 拾取到的文档坐标（PickPoint 时有效）

    // ---- 实体结果 ----
    DmId     entityId;           ///< 拾取到的实体 ID（PickEntity 时有效）
    QString  entityTypeName;     ///< 实体类型名（如 "DmLine"），调试用

    /// @brief 将拾取结果转换为 JSON 值，供回填到命令 params
    /// @return  QJsonValue：PickPoint → [x,y] 数组；PickEntity → id 字符串
    QJsonValue toJsonValue() const;
};

// ============================================================================
// 拾取任务
// ============================================================================

/// @brief 单个待拾取的任务项
///
/// 由 AIPickSession 根据 ParsedCommand::missingInputs 生成。
struct PickTask
{
    QString  key;          ///< 缺失字段名（如 "center"、"entity"）
    PickType pickType;     ///< 拾取类型
    QString  hint;         ///< 显示给用户的提示文本（已通过 tr() 生成）
};

#endif // AIPICKTYPES_H
