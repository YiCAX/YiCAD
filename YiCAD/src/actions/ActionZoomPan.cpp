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

/// @file ActionZoomPan.cpp
/// @brief 平移缩放Action实现

#include "ActionZoomPan.h"

#include <QAction>
#include <QMouseEvent>

#include "Commands.h"
#include "Debug.h"
#include "GuiCommandEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"

/// @brief 构造函数，初始化平移缩放Action
/// @param [in] doc 文档指针
/// @param [in] docView 文档视图指针
ActionZoomPan::ActionZoomPan(DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Zoom Panning", doc, docView)
{
}

/// @brief 初始化操作状态，重置坐标和捕捉设置
/// @param [in] status 初始状态值
void ActionZoomPan::init(int status)
{
    ActionInterface::init(status);
    snapMode.clear();
    snapMode.restriction = DM::RestrictNothing;
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;
    setStatus(SetPanStart);
    updateMouseButtonHints();
}

/// @brief 执行平移操作：根据拖拽距离计算并执行视图平移
void ActionZoomPan::trigger()
{
    if (getStatus() == SetPanning
        && (abs(x2 - x1) > MIN_PAN_DISTANCE
            || abs(y2 - y1) > MIN_PAN_DISTANCE))
    {
        docView->zoomPan(x2 - x1, y2 - y1);
        x1 = x2;
        y1 = y2;
    }
    if (getStatus() == SetPanEnd)
    {
        finish(false);
        docView->redraw();
    }
}

/// @brief 处理鼠标移动事件，更新当前拖拽坐标
/// @param [in] e 鼠标事件指针
void ActionZoomPan::mouseMoveEvent(QMouseEvent* e)
{
    x2 = e->x();
    y2 = e->y();
    if (getStatus() == SetPanning)
    {
        if (abs(x2 - x1) > MIN_PAN_DISTANCE
            || abs(y2 - y1) > MIN_PAN_DISTANCE)
        {
            trigger();
        }
    }
}

/// @brief 处理鼠标按下事件，记录拖拽起始坐标
/// @param [in] e 鼠标事件指针
void ActionZoomPan::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::MiddleButton || e->button() == Qt::LeftButton)
    {
        x1 = e->x();
        y1 = e->y();
        setStatus(SetPanning);
    }
}

/// @brief 处理鼠标释放事件，设置平移结束状态
/// @param [in] e 鼠标事件指针
void ActionZoomPan::mouseReleaseEvent(QMouseEvent* e)
{
    switch (e->button())
    {
        case Qt::MiddleButton:
        case Qt::RightButton:
            setStatus(SetPanEnd);
            break;

        default:
            setStatus(SetPanStart);
            break;
    }
    trigger();
}

/// @brief 更新鼠标按钮提示文本
void ActionZoomPan::updateMouseButtonHints()
{
//     switch (getStatus())
//     {
//         case SetPanStart:
//             GUIDIALOGFACTORY->updateMouseWidget(
//                 tr("Click and drag to pan zoom"), tr("Cancel"));
//             break;
//         case SetPanning:
//             GUIDIALOGFACTORY->updateMouseWidget(
//                 tr("Zoom panning"), tr("Cancel"));
//             break;
//         default:
//             GUIDIALOGFACTORY->updateMouseWidget();
//     }
}

/// @brief 更新鼠标光标样式：起始为张开手，拖拽中为握紧手
void ActionZoomPan::updateMouseCursor()
{
    switch (getStatus())
    {
        case SetPanStart:
            docView->setMouseCursor(DM::OpenHandCursor);
            break;

        case SetPanning:
            docView->setMouseCursor(DM::ClosedHandCursor);
            break;

        default:
            break;
    }
}
