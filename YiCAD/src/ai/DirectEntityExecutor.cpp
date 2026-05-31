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

/// @file DirectEntityExecutor.cpp
/// @brief DirectEntityExecutor 实现 —— 直接创建点/线/圆/矩形/椭圆

#include "DirectEntityExecutor.h"

#include "CircleData.h"
#include "DmCircle.h"
#include "DmDocument.h"
#include "DmEllipse.h"
#include "DmEntity.h"
#include "DmId.h"
#include "DmLine.h"
#include "DmPoint.h"
#include "DmPolyline.h"
#include "DmVector.h"
#include "EllipseData.h"
#include "EntityTable.h"
#include "LineData.h"
#include "PointData.h"
#include "PolylineData.h"
#include "Transaction.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <cmath>  // M_PI

// ============================================================================
// DirectEntityExecutor 公开接口
// ============================================================================

DirectEntityExecutor::DirectEntityExecutor(DmDocument* doc, QObject* parent)
    : QObject(parent)
    , m_doc(doc)
{
}

ExecutorResult DirectEntityExecutor::execute(const ParsedCommand& cmd)
{
    // 仅处理直接创建绘制命令
    switch (cmd.intent) {
    case CommandIntent::DrawPoint:
        return executeDrawPoint(cmd);
    case CommandIntent::DrawLine:
        return executeDrawLine(cmd);
    case CommandIntent::DrawCircle:
        return executeDrawCircle(cmd);
    case CommandIntent::DrawRectangle:
        return executeDrawRectangle(cmd);
    case CommandIntent::DrawEllipse:
        return executeDrawEllipse(cmd);

    default: {
        ExecutorResult fail;
        fail.success      = false;
        fail.errorMessage = tr(
            "DirectEntityExecutor: unsupported intent '%1'. "
            "Only draw_point / draw_line / draw_circle / draw_rectangle / draw_ellipse "
            "are supported.").arg(intentToString(cmd.intent));
        return fail;
    }
    }
}

// ============================================================================
// 各 intent 实现
// ============================================================================

ExecutorResult DirectEntityExecutor::executeDrawPoint(const ParsedCommand& cmd)
{
    ExecutorResult result;

    DmVector pos;
    QString err;
    if (!extractPoint(cmd.params, QStringLiteral("position"), pos, err)) {
        result.success      = false;
        result.errorMessage = tr("DirectEntityExecutor::draw_point -- %1").arg(err);
        return result;
    }

    PointData data(pos);
    DmPoint* entity = new DmPoint(nullptr, data);
    finalizeEntity(entity, tr("Create Point").toStdString(), QStringLiteral("DmPoint"), result);
    return result;
}

ExecutorResult DirectEntityExecutor::executeDrawLine(const ParsedCommand& cmd)
{
    ExecutorResult result;

    DmVector start, end;
    QString err;
    if (!extractPoint(cmd.params, QStringLiteral("start"), start, err)) {
        result.success      = false;
        result.errorMessage = tr("DirectEntityExecutor::draw_line -- %1").arg(err);
        return result;
    }
    if (!extractPoint(cmd.params, QStringLiteral("end"), end, err)) {
        result.success      = false;
        result.errorMessage = tr("DirectEntityExecutor::draw_line -- %1").arg(err);
        return result;
    }

    LineData data(start, end);
    DmLine* entity = new DmLine(nullptr, data);
    finalizeEntity(entity, tr("Create Line").toStdString(), QStringLiteral("DmLine"), result);
    return result;
}

ExecutorResult DirectEntityExecutor::executeDrawCircle(const ParsedCommand& cmd)
{
    ExecutorResult result;

    DmVector center;
    double   radius = 0.0;
    QString  err;

    if (!extractPoint(cmd.params, QStringLiteral("center"), center, err)) {
        result.success      = false;
        result.errorMessage = tr("DirectEntityExecutor::draw_circle -- %1").arg(err);
        return result;
    }
    if (!extractDouble(cmd.params, QStringLiteral("radius"), radius, err)) {
        result.success      = false;
        result.errorMessage = tr("DirectEntityExecutor::draw_circle -- %1").arg(err);
        return result;
    }

    CircleData data(center, radius);
    DmCircle* entity = new DmCircle(nullptr, data);
    finalizeEntity(entity, tr("Create Circle").toStdString(), QStringLiteral("DmCircle"), result);
    return result;
}

ExecutorResult DirectEntityExecutor::executeDrawRectangle(const ParsedCommand& cmd)
{
    ExecutorResult result;

    DmVector corner1, corner2;
    QString  err;
    if (!extractPoint(cmd.params, QStringLiteral("corner1"), corner1, err)) {
        result.success      = false;
        result.errorMessage = tr("DirectEntityExecutor::draw_rectangle -- %1").arg(err);
        return result;
    }
    if (!extractPoint(cmd.params, QStringLiteral("corner2"), corner2, err)) {
        result.success      = false;
        result.errorMessage = tr("DirectEntityExecutor::draw_rectangle -- %1").arg(err);
        return result;
    }

    // 用 corner1/corner2 推导四个角点（轴对齐矩形）
    // p1 = corner1
    // p2 = (corner2.x, corner1.y)
    // p3 = corner2
    // p4 = (corner1.x, corner2.y)
    DmVector p1(corner1);
    DmVector p2(corner2.x, corner1.y);
    DmVector p3(corner2);
    DmVector p4(corner1.x, corner2.y);

    std::vector<DmVector> pts{ p1, p2, p3, p4 };
    std::vector<double>   bulges(4, 0.0);
    std::vector<double>   weights(8, 0.0);  // 每个顶点 2 个权重

    PolylineData polyData(pts, bulges, weights, /*isClosed=*/true);
    DmPolyline* entity = new DmPolyline(nullptr, polyData);
    finalizeEntity(entity, tr("Create Rectangle").toStdString(), QStringLiteral("DmPolyline"), result);
    return result;
}

ExecutorResult DirectEntityExecutor::executeDrawEllipse(const ParsedCommand& cmd)
{
    ExecutorResult result;

    DmVector center;
    double   majorRadius = 0.0;
    double   minorRadius = 0.0;
    double   angle       = 0.0;  // 长轴与 X 轴夹角（度）
    QString  err;

    if (!extractPoint(cmd.params, QStringLiteral("center"), center, err)) {
        result.success      = false;
        result.errorMessage = tr("DirectEntityExecutor::draw_ellipse -- %1").arg(err);
        return result;
    }
    if (!extractDouble(cmd.params, QStringLiteral("major_radius"), majorRadius, err)) {
        result.success      = false;
        result.errorMessage = tr("DirectEntityExecutor::draw_ellipse -- %1").arg(err);
        return result;
    }
    if (!extractDouble(cmd.params, QStringLiteral("minor_radius"), minorRadius, err)) {
        result.success      = false;
        result.errorMessage = tr("DirectEntityExecutor::draw_ellipse -- %1").arg(err);
        return result;
    }
    // angle 可选，默认为 0
    extractDouble(cmd.params, QStringLiteral("angle"), angle, err);
    // angle 提取失败不算致命错误，使用默认 0

    const double angleRad = angle * M_PI / 180.0;

    // 长轴端点向量（相对于中心，长度 = majorRadius）
    DmVector majorP(majorRadius * std::cos(angleRad),
                    majorRadius * std::sin(angleRad));
    // 法向量（2D 绘图，Z 轴向上）
    DmVector normal(0.0, 0.0, 1.0);
    // 短轴与长轴的比率
    const double ratio = (majorRadius > 0.0)
                         ? (minorRadius / majorRadius)
                         : 1.0;

    EllipseData data(center, majorP, normal, ratio,
                     /*isClosed=*/true,
                     /*startParam=*/0.0,
                     /*endParam=*/2.0 * M_PI);

    DmEllipse* entity = new DmEllipse(nullptr, data);
    finalizeEntity(entity, tr("Create Ellipse").toStdString(), QStringLiteral("DmEllipse"), result);
    return result;
}

// ============================================================================
// 工具函数：参数提取
// ============================================================================

bool DirectEntityExecutor::extractPoint(const QJsonObject& params,
                                        const QString&     key,
                                        DmVector&          out,
                                        QString&           errorDetail)
{
    if (!params.contains(key)) {
        errorDetail = tr("missing required param '%1'").arg(key);
        return false;
    }

    const QJsonValue val = params.value(key);
    if (!val.isArray()) {
        errorDetail = tr("param '%1' must be a JSON array [x, y]").arg(key);
        return false;
    }

    const QJsonArray arr = val.toArray();
    if (arr.size() < 2) {
        errorDetail = tr("param '%1' array must have at least 2 elements (x, y)")
                          .arg(key);
        return false;
    }

    // 元素应为数值类型（QJsonValue::isDouble 对整数也返回 true）
    if (!arr.at(0).isDouble()) {
        errorDetail = tr("param '%1'[0] (x) is not a number").arg(key);
        return false;
    }
    if (!arr.at(1).isDouble()) {
        errorDetail = tr("param '%1'[1] (y) is not a number").arg(key);
        return false;
    }

    const double x = arr.at(0).toDouble();
    const double y = arr.at(1).toDouble();

    out = DmVector(x, y);
    return true;
}

bool DirectEntityExecutor::extractDouble(const QJsonObject& params,
                                         const QString&     key,
                                         double&            out,
                                         QString&           errorDetail)
{
    if (!params.contains(key)) {
        errorDetail = tr("missing required param '%1'").arg(key);
        return false;
    }

    const QJsonValue val = params.value(key);
    if (!val.isDouble()) {
        errorDetail = tr("param '%1' must be a number").arg(key);
        return false;
    }

    out = val.toDouble();
    return true;
}

// ============================================================================
// 工具函数：标准事务收尾
// ============================================================================

void DirectEntityExecutor::finalizeEntity(DmEntity*          entity,
                                          const std::string& txName,
                                          const QString&     typeName,
                                          ExecutorResult&    result)
{
    if (!m_doc || !entity) {
        // 防御：构造失败或文档无效
        delete entity;
        result.success      = false;
        result.errorMessage = tr(
            "DirectEntityExecutor: internal error — null document or entity.");
        return;
    }

    // ---- 标准创建路径（与 ActionDrawCircle::trigger 一致） ----
    Transaction t(txName, m_doc);
    t.start();

    entity->setDocument(m_doc);       // 设置文档、活动图层、活动画笔
    m_doc->getEntityTable()->add(entity);  // add 自动创建 EntityTableAddCmd，纳入 Undo/Redo

    t.commit();
    // -----------------------------------------------------------

    // 填充结果
    EntityCreateResult ecr;
    ecr.entityId   = entity->getId();
    ecr.entityType = typeName;

    result.success = true;
    result.createdEntities.append(ecr);
}
