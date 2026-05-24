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

/// @file AIPickAction.cpp
/// @brief AIPickAction 实现

#include "AIPickAction.h"

#include <QKeyEvent>
#include <QMouseEvent>

#include "Datamodel.h"
#include "DmEntity.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"

// ============================================================================
// 工具函数：实体类型枚举 → 可读字符串
// ============================================================================

QString AIPickAction::entityTypeDisplayName(DM::EntityType t)
{
    switch (t) {
    case DM::EntityPoint:       return tr("Point");
    case DM::EntityLine:        return tr("Line");
    case DM::EntityCircle:      return tr("Circle");
    case DM::EntityArc:         return tr("Arc");
    case DM::EntityEllipse:     return tr("Ellipse");
    case DM::EntityPolyline:    return tr("Polyline");
    case DM::EntityBlockReference: return tr("BlockRef");
    case DM::EntityText:        return tr("Text");
    case DM::EntityMText:       return tr("MText");
    case DM::EntitySpline:      return tr("Spline");
    case DM::EntityHatch:       return tr("Hatch");
    default:                    return tr("Entity(%1)").arg(static_cast<int>(t));
    }
}

// ============================================================================
// 构造 / 初始化
// ============================================================================

AIPickAction::AIPickAction(DmDocument*      doc,
                           GuiDocumentView* docView,
                           const PickTask&  task,
                           PickCallback     callback)
    : ActionInterface("AIPickAction", doc, docView)
    , m_task(task)
    , m_callback(std::move(callback))
{
    // 使用一个不冲突的 ActionType（避免被 killSelectActions 误杀）
    actionType = DM::ActionNone;
}

void AIPickAction::init(int status)
{
    ActionInterface::init(status);

    // 默认开启所有常用捕捉模式，给用户良好体验
    SnapMode sm;
    sm.snapEndpoint     = true;
    sm.snapCenter       = true;
    sm.snapMiddle       = true;
    sm.snapOnEntity     = true;
    sm.snapIntersection = true;
    sm.snapFree         = true;
    sm.snapGrid         = true;
    setSnapMode(sm);

    updateMouseButtonHints();
    updateMouseCursor();

    // 命令栏显示提示
    GUIDIALOGFACTORY->commandMessage(m_task.hint);
}

// ============================================================================
// 事件处理
// ============================================================================

void AIPickAction::mouseReleaseEvent(QMouseEvent* e)
{
    if (m_called) return;

    if (e->button() == Qt::RightButton) {
        doCancel();
        return;
    }

    if (e->button() == Qt::LeftButton) {
        switch (m_task.pickType) {
        case PickType::PickPoint:
            doPickPoint(e);
            break;
        case PickType::PickEntity:
            doPickEntity(e);
            break;
        }
    }
}

void AIPickAction::mouseMoveEvent(QMouseEvent* e)
{
    // 驱动捕捉预览：调用 snapPoint 以更新捕捉指示器
    if (m_task.pickType == PickType::PickPoint) {
        snapPoint(e);
    }
    // PickEntity 模式下不驱动捕捉预览，保持选择光标
}

void AIPickAction::keyPressEvent(QKeyEvent* e)
{
    if (m_called) return;

    if (e->key() == Qt::Key_Escape) {
        doCancel();
        return;
    }

    // 其他按键委托基类
    ActionInterface::keyPressEvent(e);
}

// ============================================================================
// 鼠标光标 / 按钮提示
// ============================================================================

void AIPickAction::updateMouseCursor()
{
    switch (m_task.pickType) {
    case PickType::PickPoint:
        docView->setMouseCursor(DM::CadCursor);
        break;
    case PickType::PickEntity:
        docView->setMouseCursor(DM::SelectCursor);
        break;
    }
}

void AIPickAction::updateMouseButtonHints()
{
    QString leftHint;
    switch (m_task.pickType) {
    case PickType::PickPoint:
        leftHint = tr("Specify %1").arg(m_task.key);
        break;
    case PickType::PickEntity:
        leftHint = tr("Select %1").arg(m_task.key);
        break;
    }

    GUIDIALOGFACTORY->updateMouseWidget(leftHint, tr("Cancel"));
}

// ============================================================================
// 内部实现
// ============================================================================

void AIPickAction::doPickPoint(QMouseEvent* e)
{
    PickResult result;
    result.pickType = PickType::PickPoint;

    // 使用 Snapper 获取捕捉后的坐标
    DmVector snapped = snapPoint(e);
    if (!snapped.valid) {
        // 捕捉失败，回退到自由点
        snapped = snapFree(e);
    }

    // snapFree 已包含屏幕→世界坐标转换，且 snapFree 在 free 模式下
    // 几乎总能返回有效坐标。如果仍然无效（极端情况），则结果为无效点。

    result.point   = snapped;
    result.success = true;

    if (m_callback && !m_called) {
        m_called = true;
        m_callback(result);
    }

    // 自身结束，由 GuiEventHandler::cleanUp 回收
    finish(false);
}

void AIPickAction::doPickEntity(QMouseEvent* e)
{
    PickResult result;
    result.pickType = PickType::PickEntity;

    DmEntity* entity = catchEntity(e);
    if (entity) {
        result.entityId       = entity->getId();
        result.entityTypeName = entityTypeDisplayName(entity->getEntityType());
        result.point          = snapPoint(e);  // 同时记录命中点
        result.success        = true;
    }

    if (m_callback && !m_called) {
        m_called = true;
        m_callback(result);
    }

    finish(false);
}

void AIPickAction::doCancel()
{
    PickResult result;
    result.pickType = m_task.pickType;
    result.success  = false;

    if (m_callback && !m_called) {
        m_called = true;
        m_callback(result);
    }

    finish(false);
}
