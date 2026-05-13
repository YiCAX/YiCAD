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

/// @file ActionEditCopy.cpp
/// @brief 编辑复制/剪切 Action 类的实现

#include "ActionEditCopy.h"

#include <QAction>
#include <QMouseEvent>

#include "GuiCoordinateEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Modification.h"

/// @brief 构造函数
/// @param copy 为 true 时复制，为 false 时剪切
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionEditCopy::ActionEditCopy(bool copy, DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Edit Copy", doc, docView), copy{copy}, referencePoint{new DmVector{}}
{
}

ActionEditCopy::~ActionEditCopy() = default;

/// @brief 初始化 Action 状态
/// @param status 初始状态
void ActionEditCopy::init(int status)
{
    ActionInterface::init(status);
    // trigger();
}

/// @brief 触发复制/剪切操作
void ActionEditCopy::trigger()
{
    Modification m(docView);
    m.copy(*referencePoint, !copy);

    // docView->redraw();
    finish(false);
    docView->killSelectActions();
    // init(getStatus()-1);
    GUIDIALOGFACTORY->updateSelectionWidget(pDocument->getEntityTable()->countSelect());
}

/// @brief 鼠标移动事件处理
/// @param e 鼠标事件
void ActionEditCopy::mouseMoveEvent(QMouseEvent* e)
{
    if (getStatus() == SetReferencePoint)
    {
        (void)snapPoint(e);
    }
    else
    {
        deleteSnapper();
    }
}

/// @brief 鼠标释放事件处理
/// @param e 鼠标事件
void ActionEditCopy::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        GuiCoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    }
    else if (e->button() == Qt::RightButton)
    {
        init(getStatus() - 1);
    }
}

/// @brief 坐标事件处理
/// @param e 坐标事件
void ActionEditCopy::coordinateEvent(GuiCoordinateEvent* e)
{
    if (!e)
    {
        return;
    }

    *referencePoint = e->getCoordinate();
    trigger();
}

/// @brief 更新鼠标按钮提示
void ActionEditCopy::updateMouseButtonHints()
{
    switch (getStatus())
    {
        case SetReferencePoint:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify reference point"), tr("Cancel"));
            break;
        default:
            GUIDIALOGFACTORY->updateMouseWidget();
            break;
    }
}

/// @brief 更新鼠标光标
void ActionEditCopy::updateMouseCursor()
{
    docView->setMouseCursor(DM::CadCursor);
}
