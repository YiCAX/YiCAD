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

/// @file ModificationExecutor.h
/// @brief 修改命令执行器 —— 解析 ParsedCommand，通过 Transaction/Modification 执行修改操作
///
/// 职责：
///   - 接收 LLMCommandBridge 产出的 ParsedCommand
///   - 针对 Delete / Move / Copy / Offset / Trim 五种修改意图执行操作
///   - 所有操作通过 Transaction 包裹，保证 Undo/Redo
///   - 复用现有 Modification 类（move、offset、trim、delete）
///   - Copy 操作通过 Entity::clone() + 偏移 + EntityTable::add 实现
///   - 高删除操作返回 needsConfirmation 强制用户确认
///   - 不做绘制、不做标注
///
/// 支持的操作（按优先级）：
///   1. Delete  — 删除选中或全部实体
///   2. Move    — 按偏移量移动选中实体
///   3. Copy    — 偏移复制选中实体
///   4. Offset  — 等距偏移实体
///   5. Trim    — 修剪实体（部分支持，见已知限制）
///
/// 已知限制（当前版本）：
///   - Trim 仅支持"当前选中既作剪切边又被剪"的简化模式（selection=current_selection）
///   - Copy 不支持多次阵列复制（仅单次偏移复制）
///   - Offset 的单实体模式（数量为 1）
///   - 不支持 Extend / Bevel / Round / Explode / Cut / Rotate / Scale / Mirror
///   - 不支持 selection 的 PickRequired / LastCreated 模式（需 ContextResolver 先补全实体 ID）
///   - 仅支持 selection mode: CurrentSelection, All

#ifndef MODIFICATIONEXECUTOR_H
#define MODIFICATIONEXECUTOR_H

#include "LLMCommandBridge.h"

#include <QString>
#include <QVector>
#include "DmVector.h"
#include "DmId.h"

class DmDocument;
class DmEntity;
class GuiDocumentView;

// ============================================================================
// 修改执行结果
// ============================================================================

/// @brief 单个受影响的实体信息
struct ModificationAffectedEntity
{
    DmId    entityId;       ///< 受影响的实体 ID
    QString entityType;     ///< 实体类型名（调试用）
    QString action;         ///< 执行的动作：removed / moved / copied / offset / trimmed
};

/// @brief ModificationExecutor::execute() 返回值
struct ModificationResult
{
    bool    success = false;                ///< 整体执行是否成功
    QString errorMessage;                   ///< 失败时的错误描述（人类可读）
    int     affectedCount = 0;              ///< 受影响的实体数量
    bool    needsConfirmation = false;      ///< 是否需要用户确认后才能执行

    /// 本次调用影响的实体详情（调试用，可能不完整）
    QVector<ModificationAffectedEntity> affectedEntities;
};

// ============================================================================
// ModificationExecutor
// ============================================================================

/// @brief 修改命令执行器
///
/// 无状态；构造时传入 DmDocument* 和 GuiDocumentView*，后续反复调用 execute()。
class ModificationExecutor
{
public:
    /// @brief 构造函数
    /// @param doc     目标文档（非空）
    /// @param docView 文档视图（非空，用于 UI 刷新）
    explicit ModificationExecutor(DmDocument* doc, GuiDocumentView* docView);

    ~ModificationExecutor() = default;

    /// @brief 执行一条已解析的修改命令
    /// @param cmd  LLMCommandBridge::parse() 产出的 ParsedCommand
    /// @return ModificationResult  调用方应首先检查 success 字段
    ///
    /// 对于未知 intent 或非修改类 intent，返回 success=false 并附带错误信息。
    /// 对于高删除操作，返回 needsConfirmation=true，调用方应展示确认对话框。
    ModificationResult execute(const ParsedCommand& cmd);

    /// @brief 检查该意图是否需要用户确认
    /// @return 对于 Delete / Trim（高删除操作）返回 true
    static bool isHighRiskIntent(CommandIntent intent);

private:
    // ---- 各 intent 的实现 ----
    ModificationResult executeDelete(const ParsedCommand& cmd);
    ModificationResult executeMove(const ParsedCommand& cmd);
    ModificationResult executeCopy(const ParsedCommand& cmd);
    ModificationResult executeOffset(const ParsedCommand& cmd);
    ModificationResult executeTrim(const ParsedCommand& cmd);

    // ---- 工具函数：从 SelectionSpec 解析目标实体 ----

    /// @brief 解析选择说明，收集目标实体
    /// @param spec  选择说明
    /// @param out   输出：目标实体列表
    /// @return 成功返回 true（即使列表为空）
    bool resolveEntities(const SelectionSpec& spec, std::vector<DmEntity*>& out);

    // ---- 工具函数：从 JSON params 提取参数 ----

    /// @brief 从 JSON 对象中提取 DmVector（期望 [x, y] 数组）
    static bool extractPoint(const QJsonObject& params,
                             const QString&     key,
                             DmVector&          out,
                             QString&           errorDetail);

    /// @brief 从 JSON 对象中提取 double 值
    static bool extractDouble(const QJsonObject& params,
                              const QString&     key,
                              double&            out,
                              QString&           errorDetail);

    // ---- 工具函数：复制实体 ----

    /// @brief 克隆实体、偏移并添加到文档（用于 Copy 操作）
    /// @param entity 源实体（不会被修改）
    /// @param offset 偏移量
    /// @param result 输出：填充 affectedEntities
    /// @return 成功返回新实体指针（失败返回 nullptr）
    DmEntity* cloneAndOffsetEntity(DmEntity*          entity,
                                   const DmVector&    offset,
                                   ModificationResult& result);

    // ---- 工具函数：通知 UI 刷新 ----

    void notifyDocumentModified();

    DmDocument*       m_doc;     ///< 目标文档（非空）
    GuiDocumentView*  m_docView; ///< 文档视图（非空）
};

#endif // MODIFICATIONEXECUTOR_H
