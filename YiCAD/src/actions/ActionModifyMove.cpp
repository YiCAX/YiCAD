/*
 * Copyright (C) 2026 YiCAD Community
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

/// @file ActionModifyMove.cpp
/// @brief 移动修改操作——处理用户鼠标事件以实现实体移动功能

#include "ActionModifyMove.h"

#include <QAction>
#include <QMouseEvent>
#include <cmath>

#include "Debug.h"
#include "DmLine.h"
#include "GuiCoordinateEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Modification.h"
#include "Preview.h"
#include "Transaction.h"

/// @brief 移动操作步进角度（用于 Shift 约束）
constexpr double MOVE_SNAP_ANGLE = 15.0;

struct ActionModifyMove::Points
{
    DmVector referencePoint;
    DmVector targetPoint;
};

ActionModifyMove::ActionModifyMove(DmDocument* doc, GuiDocumentView* docView) :
    PreviewActionInterface("Move Entities", doc, docView), pPoints(new Points{})
{
    actionType = DM::ActionModifyMove;
}

ActionModifyMove::~ActionModifyMove() = default;

/// @brief 执行移动操作，将所有选中实体沿参考点到目标点的偏移量进行移动
void ActionModifyMove::trigger()
{
    Modification m(docView);
    DmVector offset = pPoints->targetPoint - pPoints->referencePoint;
    m.move(offset);

    GUIDIALOGFACTORY->updateSelectionWidget(pDocument->getEntityTable()->countSelect());
    finish(false);
}

/// @brief 处理鼠标移动事件，更新目标点位置并实时预览移动效果
/// @param [in] e 鼠标事件指针
void ActionModifyMove::mouseMoveEvent(QMouseEvent* e)
{
    if (getStatus() == SetReferencePoint || getStatus() == SetTargetPoint)
    {
        DmVector mouse = snapPoint(e);
        switch (getStatus())
        {
            case SetReferencePoint:
                pPoints->referencePoint = mouse;
                break;

            case SetTargetPoint:
                if (pPoints->referencePoint.valid)
                {
                    if (e->modifiers() & Qt::ShiftModifier)
                    {
                        mouse = snapToAngle(mouse, pPoints->referencePoint, MOVE_SNAP_ANGLE);
                    }

                    pPoints->targetPoint = mouse;

                    deletePreview();
                    preview->addSelectionFromDocument();
                    preview->move(pPoints->targetPoint - pPoints->referencePoint);

                    if (e->modifiers() & Qt::ShiftModifier)
                    {
                        DmLine* line = new DmLine(nullptr, pPoints->referencePoint, mouse);
                        preview->addEntity(line);
                        line->setSelected(true);
                        line->setLayerToActive();
                        line->setPenToActive();
                    }
                    drawPreview();
                }
                break;

            default:
                break;
        }
    }
}

/// @brief 处理鼠标释放事件，确认移动或回退操作
/// @param [in] e 鼠标事件指针
void ActionModifyMove::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        DmVector snapped = snapPoint(e);
        if ((e->modifiers() & Qt::ShiftModifier) && getStatus() == SetTargetPoint)
        {
            snapped = snapToAngle(snapped, pPoints->referencePoint, MOVE_SNAP_ANGLE);
        }

        GuiCoordinateEvent ce(snapped);
        coordinateEvent(&ce);
    }
    else if (e->button() == Qt::RightButton)
    {
        deletePreview();
        init(getStatus() - 1);
    }
}

/// @brief 处理坐标事件，根据状态设置参考点/目标点并执行移动
/// @param [in] e 坐标事件指针
void ActionModifyMove::coordinateEvent(GuiCoordinateEvent* e)
{
    if (e == nullptr)
    {
        return;
    }

    DmVector pos = e->getCoordinate();

    switch (getStatus())
    {
        case SetReferencePoint:
            pPoints->referencePoint = pos;
            docView->moveRelativeZero(pPoints->referencePoint);
            setStatus(SetTargetPoint);
            break;

        case SetTargetPoint:
            pPoints->targetPoint = pos;
            docView->moveRelativeZero(pPoints->targetPoint);
            trigger();
            break;

        default:
            break;
    }
}

/// @brief 更新鼠标按钮提示信息
void ActionModifyMove::updateMouseButtonHints()
{
    switch (getStatus())
    {
        case SetReferencePoint:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify reference point"), tr("Cancel"));
            break;
        case SetTargetPoint:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify target point"), tr("Back"));
            break;
        default:
            GUIDIALOGFACTORY->updateMouseWidget();
            break;
    }
}

/// @brief 更新鼠标光标为十字光标
void ActionModifyMove::updateMouseCursor()
{
    if (docView != nullptr)
    {
        docView->setMouseCursor(DM::CadCursor);
    }
}
