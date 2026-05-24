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

/// @file ModificationExecutor.cpp
/// @brief ModificationExecutor 实现 —— 删除/移动/复制/偏移/修剪

#include "ModificationExecutor.h"

#include "DmDocument.h"
#include "DmEntity.h"
#include "DmId.h"
#include "DmVector.h"
#include "EntityTable.h"
#include "GuiDocumentView.h"
#include "Modification.h"
#include "Transaction.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

// ============================================================================
// ModificationExecutor 公开接口
// ============================================================================

ModificationExecutor::ModificationExecutor(DmDocument* doc, GuiDocumentView* docView)
    : m_doc(doc)
    , m_docView(docView)
{
}

ModificationResult ModificationExecutor::execute(const ParsedCommand& cmd)
{
    switch (cmd.intent) {
    case CommandIntent::Delete:
        return executeDelete(cmd);
    case CommandIntent::Move:
        return executeMove(cmd);
    case CommandIntent::Copy:
        return executeCopy(cmd);
    case CommandIntent::Offset:
        return executeOffset(cmd);
    case CommandIntent::Trim:
        return executeTrim(cmd);

    default: {
        ModificationResult fail;
        fail.success      = false;
        fail.errorMessage = QStringLiteral(
            "ModificationExecutor: unsupported intent '%1'. "
            "Only delete / move / copy / offset / trim are supported.")
            .arg(intentToString(cmd.intent));
        return fail;
    }
    }
}

bool ModificationExecutor::isHighRiskIntent(CommandIntent intent)
{
    switch (intent) {
    case CommandIntent::Delete:
    case CommandIntent::Trim:
        return true;
    default:
        return false;
    }
}

// ============================================================================
// executeDelete —— 删除选中或全部实体
// ============================================================================

ModificationResult ModificationExecutor::executeDelete(const ParsedCommand& cmd)
{
    ModificationResult result;

    // ---- 高删除操作 — 全选删除必须确认 ----
    if (!cmd.needsConfirmation && cmd.selection.mode == SelectionMode::All) {
        result.success           = false;
        result.needsConfirmation = true;
        result.errorMessage      = QStringLiteral("Delete all entities requires confirmation.");
        return result;
    }

    // ---- 解析目标实体 ----
    std::vector<DmEntity*> targets;
    if (!resolveEntities(cmd.selection, targets)) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::delete — failed to resolve entities.");
        return result;
    }

    if (targets.empty()) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::delete — no entities selected.");
        return result;
    }

    // ---- 在删除前收集 ID（避免 use-after-free） ----
    const int targetCount = static_cast<int>(targets.size());
    QVector<DmId> targetIds;
    targetIds.reserve(targetCount);
    for (auto e : targets) {
        if (e) {
            targetIds.append(e->getId());
        }
    }

    // ---- 选中目标实体 ----
    EntityTable* table = m_doc->getEntityTable();
    for (auto e : *table) {
        if (e && e->isSelected()) {
            e->setSelected(false);
        }
    }
    for (auto e : targets) {
        e->setSelected(true);
    }

    // ---- 执行删除（Modification::remove 内部有 Transaction） ----
    Modification m(m_docView);
    m.remove();
    // 此后 targets 中指针已全部失效，仅使用 targetIds

    // ---- 填充结果（使用预收集的 ID） ----
    result.affectedCount = targetCount;
    for (const auto& id : targetIds) {
        ModificationAffectedEntity ae;
        ae.entityId   = id;
        ae.entityType = QStringLiteral("DmEntity");
        ae.action     = QStringLiteral("removed");
        result.affectedEntities.append(ae);
    }

    result.success = true;
    notifyDocumentModified();
    return result;
}

// ============================================================================
// executeMove —— 按偏移量移动选中实体
// ============================================================================

ModificationResult ModificationExecutor::executeMove(const ParsedCommand& cmd)
{
    ModificationResult result;

    // ---- 提取偏移量 ----
    DmVector offset;
    QString  err;
    if (!extractPoint(cmd.params, QStringLiteral("offset"), offset, err)) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::move — %1").arg(err);
        return result;
    }
    if (!offset.valid) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::move — offset is invalid.");
        return result;
    }

    // ---- 解析目标实体 ----
    std::vector<DmEntity*> targets;
    if (!resolveEntities(cmd.selection, targets)) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::move — failed to resolve entities.");
        return result;
    }
    if (targets.empty()) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::move — no entities selected.");
        return result;
    }

    // ---- 选中目标实体 ----
    EntityTable* table = m_doc->getEntityTable();
    for (auto e : *table) {
        if (e && e->isSelected()) {
            e->setSelected(false);
        }
    }
    for (auto e : targets) {
        e->setSelected(true);
    }

    // ---- 执行移动（Modification::move 内部有 Transaction） ----
    Modification m(m_docView);
    m.move(offset);

    // ---- 填充结果 ----
    result.affectedCount = static_cast<int>(targets.size());
    for (auto e : targets) {
        ModificationAffectedEntity ae;
        ae.entityId   = e->getId();
        ae.entityType = QStringLiteral("DmEntity");
        ae.action     = QStringLiteral("moved");
        result.affectedEntities.append(ae);
    }

    result.success = true;
    notifyDocumentModified();
    return result;
}

// ============================================================================
// executeCopy —— 偏移复制选中实体
// ============================================================================

ModificationResult ModificationExecutor::executeCopy(const ParsedCommand& cmd)
{
    ModificationResult result;

    // ---- 提取偏移量 ----
    DmVector offset;
    QString  err;
    if (!extractPoint(cmd.params, QStringLiteral("offset"), offset, err)) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::copy — %1").arg(err);
        return result;
    }
    if (!offset.valid) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::copy — offset is invalid.");
        return result;
    }

    // ---- 解析目标实体 ----
    std::vector<DmEntity*> targets;
    if (!resolveEntities(cmd.selection, targets)) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::copy — failed to resolve entities.");
        return result;
    }
    if (targets.empty()) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::copy — no entities selected.");
        return result;
    }

    // ---- 克隆 + 偏移 + 添加（通过 Transaction） ----
    Transaction t(QObject::tr("Copy Entities").toStdString(), m_doc);
    t.start();

    EntityTable* table = m_doc->getEntityTable();
    int copiedCount = 0;

    for (auto src : targets) {
        DmEntity* cloned = cloneAndOffsetEntity(src, offset, result);
        if (cloned) {
            cloned->setDocument(m_doc);
            table->add(cloned);
            ++copiedCount;
        }
    }

    t.commit();

    // ---- 填充结果 ----
    result.affectedCount = copiedCount;
    if (copiedCount == 0) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::copy — failed to clone any entity.");
    } else {
        result.success = true;
    }

    notifyDocumentModified();
    return result;
}

// ============================================================================
// executeOffset —— 等距偏移实体
// ============================================================================

ModificationResult ModificationExecutor::executeOffset(const ParsedCommand& cmd)
{
    ModificationResult result;

    // ---- 提取偏移距离 ----
    double  distance = 0.0;
    QString err;
    if (!extractDouble(cmd.params, QStringLiteral("distance"), distance, err)) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::offset — %1").arg(err);
        return result;
    }
    if (distance <= 0.0) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::offset — distance must be positive.");
        return result;
    }

    // ---- 可选参考坐标，默认 (0, 0) ----
    DmVector coord(0.0, 0.0);
    extractPoint(cmd.params, QStringLiteral("coord"), coord, err);

    // ---- 解析目标实体 ----
    std::vector<DmEntity*> targets;
    if (!resolveEntities(cmd.selection, targets)) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::offset — failed to resolve entities.");
        return result;
    }
    if (targets.empty()) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::offset — no entities selected.");
        return result;
    }

    // ---- 自行实现：Transaction + startModify + e->offset() ----
    // 不使用 Modification::offset()，因为它内部缺少 startModify() 导致 Undo 失效
    EntityTable* table = m_doc->getEntityTable();
    Transaction t(QObject::tr("Offset Entities").toStdString(), m_doc);
    t.start();

    int offsetCount = 0;
    for (auto e : targets) {
        if (!e) continue;

        // 记录修改前状态（与 Modification::move 一致的 Undo 模式）
        table->startModify(e);

        // 如果未指定 coord，使用实体包围盒中心作为参考点
        DmVector refCoord = coord.valid ? coord : e->getCenter();
        if (e->offset(refCoord, distance)) {
            ++offsetCount;

            ModificationAffectedEntity ae;
            ae.entityId   = e->getId();
            ae.entityType = QStringLiteral("DmEntity");
            ae.action     = QStringLiteral("offset");
            result.affectedEntities.append(ae);
        }
        // 个别实体 offset 失败不阻断整个事务
    }

    t.commit();

    // ---- 填充结果 ----
    result.affectedCount = offsetCount;
    if (offsetCount == 0) {
        result.success      = false;
        result.errorMessage = QStringLiteral(
            "ModificationExecutor::offset — no entity could be offset. "
            "Entity type may not support offset.");
    } else {
        result.success = true;
    }

    notifyDocumentModified();
    return result;
}

// ============================================================================
// executeTrim —— 修剪实体（部分支持）
// ============================================================================

ModificationResult ModificationExecutor::executeTrim(const ParsedCommand& cmd)
{
    ModificationResult result;

    // ---- 解析目标实体（既做剪切边又被剪） ----
    std::vector<DmEntity*> targets;
    if (!resolveEntities(cmd.selection, targets)) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::trim — failed to resolve entities.");
        return result;
    }
    if (targets.size() < 2) {
        result.success      = false;
        result.errorMessage = QStringLiteral(
            "ModificationExecutor::trim — at least 2 entities required "
            "(cutting edge + entity to trim).");
        return result;
    }

    // ---- 提取鼠标点（决定保留哪一侧） ----
    DmVector mousePt(0.0, 0.0);
    QString  err;
    if (!extractPoint(cmd.params, QStringLiteral("mouse_point"), mousePt, err)) {
        result.success      = false;
        result.errorMessage = QStringLiteral("ModificationExecutor::trim — %1").arg(err);
        return result;
    }

    // ---- 简化模式：targets 互为剪切边，逐个尝试剪切 ----
    Modification m(m_docView);
    int trimmedCount = 0;

    for (auto entBeenCut : targets) {
        if (!entBeenCut) continue;

        std::vector<DmEntity*> cuttingEdges;
        for (auto e : targets) {
            if (e != entBeenCut) {
                cuttingEdges.push_back(e);
            }
        }
        if (cuttingEdges.empty()) continue;

        if (m.trim(cuttingEdges, entBeenCut, mousePt)) {
            ++trimmedCount;
            ModificationAffectedEntity ae;
            ae.entityId   = entBeenCut->getId();
            ae.entityType = QStringLiteral("DmEntity");
            ae.action     = QStringLiteral("trimmed");
            result.affectedEntities.append(ae);
        }
    }

    // ---- 填充结果 ----
    result.affectedCount = trimmedCount;
    if (trimmedCount == 0) {
        result.success      = false;
        result.errorMessage = QStringLiteral(
            "ModificationExecutor::trim — no entities could be trimmed. "
            "Ensure mouse_point is on the side to keep and entities intersect.");
    } else {
        result.success = true;
    }

    notifyDocumentModified();
    return result;
}

// ============================================================================
// 工具函数：实体解析
// ============================================================================

bool ModificationExecutor::resolveEntities(const SelectionSpec& spec,
                                           std::vector<DmEntity*>& out)
{
    out.clear();

    if (!m_doc) {
        return false;
    }

    EntityTable* table = m_doc->getEntityTable();
    if (!table) {
        return false;
    }

    switch (spec.mode) {

    case SelectionMode::CurrentSelection:
        for (auto e : *table) {
            if (e && e->isSelected()) {
                out.push_back(e);
            }
        }
        return true;

    case SelectionMode::All:
        for (auto e : *table) {
            if (e && e->isVisible()) {
                out.push_back(e);
            }
        }
        return true;

    case SelectionMode::None:
        return true;

    case SelectionMode::LastCreated:
    {
        // 回退策略：遍历文档取最后一个可见实体
        DmEntity* lastEntity = nullptr;
        for (auto e : *table)
        {
            if (e && e->isVisible())
            {
                lastEntity = e;
            }
        }
        if (lastEntity)
        {
            out.push_back(lastEntity);
        }
        return true;
    }

    case SelectionMode::PickRequired:
    {
        // 从 selection.raw 中读取 AIPickSession 回填的实体 ID
        for (auto it = spec.raw.begin(); it != spec.raw.end(); ++it)
        {
            if (it.value().isString())
            {
                DmId id(it.value().toString().toStdString());
                if (!id.isValid()) continue;
                DmEntity* e = table->find(id);
                if (e) out.push_back(e);
            }
        }
        return true;
    }

    default:
        return false;
    }
}

// ============================================================================
// 工具函数：参数提取
// ============================================================================

bool ModificationExecutor::extractPoint(const QJsonObject& params,
                                        const QString&     key,
                                        DmVector&          out,
                                        QString&           errorDetail)
{
    if (!params.contains(key)) {
        errorDetail = QStringLiteral("missing required param '%1'").arg(key);
        return false;
    }

    const QJsonValue val = params.value(key);
    if (!val.isArray()) {
        errorDetail = QStringLiteral("param '%1' must be a JSON array [x, y]").arg(key);
        return false;
    }

    const QJsonArray arr = val.toArray();
    if (arr.size() < 2) {
        errorDetail = QStringLiteral("param '%1' array must have at least 2 elements (x, y)").arg(key);
        return false;
    }

    if (!arr.at(0).isDouble()) {
        errorDetail = QStringLiteral("param '%1'[0] (x) is not a number").arg(key);
        return false;
    }
    if (!arr.at(1).isDouble()) {
        errorDetail = QStringLiteral("param '%1'[1] (y) is not a number").arg(key);
        return false;
    }

    const double x = arr.at(0).toDouble();
    const double y = arr.at(1).toDouble();

    out = DmVector(x, y);
    return true;
}

bool ModificationExecutor::extractDouble(const QJsonObject& params,
                                         const QString&     key,
                                         double&            out,
                                         QString&           errorDetail)
{
    if (!params.contains(key)) {
        errorDetail = QStringLiteral("missing required param '%1'").arg(key);
        return false;
    }

    const QJsonValue val = params.value(key);
    if (!val.isDouble()) {
        errorDetail = QStringLiteral("param '%1' must be a number").arg(key);
        return false;
    }

    out = val.toDouble();
    return true;
}

// ============================================================================
// 工具函数：克隆实体并偏移
// ============================================================================

DmEntity* ModificationExecutor::cloneAndOffsetEntity(DmEntity*          entity,
                                                     const DmVector&    offset,
                                                     ModificationResult& result)
{
    if (!entity) {
        return nullptr;
    }

    DmEntity* cloned = entity->clone();
    if (!cloned) {
        return nullptr;
    }

    cloned->move(offset);

    ModificationAffectedEntity ae;
    ae.entityId   = cloned->getId();
    ae.entityType = QStringLiteral("DmEntity");
    ae.action     = QStringLiteral("copied");
    result.affectedEntities.append(ae);

    return cloned;
}

// ============================================================================
// 工具函数：通知 UI 刷新
// ============================================================================

void ModificationExecutor::notifyDocumentModified()
{
    if (m_docView) {
        m_docView->specifyDocumentModified();
        m_docView->redraw();
    }
}
