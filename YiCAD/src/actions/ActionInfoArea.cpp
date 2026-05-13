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

/// @file ActionInfoArea.cpp
/// @brief 面积测量 Action 类的实现

#include <QAction>
#include "ActionInfoArea.h"

#include <QMouseEvent>

#include "Debug.h"
#include "DmDocument.h"
#include "DmLine.h"
#include "GuiCoordinateEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "InfoArea.h"
#include "Preview.h"

/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionInfoArea::ActionInfoArea(DmDocument* doc, GuiDocumentView* docView) :
    PreviewActionInterface("Info Area", doc, docView), ia(new InfoArea{})
{
    actionType = DM::ActionInfoArea;
}

ActionInfoArea::~ActionInfoArea() = default;

/// @brief 初始化 Action 状态
/// @param status 初始状态
void ActionInfoArea::init(int status)
{
    ActionInterface::init(status);

    if (status == SetFirstPoint)
    {
        deletePreview();
        ia->reset();
    }
}

/// @brief 触发面积显示并重置
void ActionInfoArea::trigger()
{
    display();

    init(SetFirstPoint);
}

/// @brief 显示面积周长和多边形预览
// TODO: 重新生成整个预览，可以优化为只生成修改的行
void ActionInfoArea::display()
{
    deletePreview();
    if (ia->size() < 1)
    {
        return;
    }
    switch (ia->size())
    {
        case 2:
            preview->addEntity(new DmLine(nullptr, ia->at(0), ia->at(1)));
            break;
        default:
            for (int i = 0; i < ia->size(); i++)
            {
                preview->addEntity(new DmLine(nullptr, ia->at(i), ia->at((i + 1) % ia->size())));
            }
            const QString linear = DmUnits::formatLinear(ia->getCircumference(), pDocument->getUnit(), pDocument->getLinearFormat(), pDocument->getLinearPrecision());
            GUIDIALOGFACTORY->commandMessage(tr("Circumference: %1").arg(linear));
            GUIDIALOGFACTORY->commandMessage(tr("Area: %1 %2^2").arg(ia->getArea()).arg(DmUnits::unitToString(pDocument->getUnit())));
            break;
    }
    drawPreview();
}

/// @brief 鼠标移动事件处理
/// @param e 鼠标事件
void ActionInfoArea::mouseMoveEvent(QMouseEvent* e)
{
    DmVector mouse = snapPoint(e);
    if (getStatus() == SetNextPoint)
    {
        ia->push_back(mouse);
        display();
        ia->pop_back();
    }
}

/// @brief 鼠标释放事件处理
/// @param e 鼠标事件
void ActionInfoArea::mouseReleaseEvent(QMouseEvent* e)
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
void ActionInfoArea::coordinateEvent(GuiCoordinateEvent* e)
{
    if (e == nullptr)
    {
        return;
    }

    DmVector mouse = e->getCoordinate();
    if (ia->duplicated(mouse))
    {
        ia->push_back(mouse);
        GUIDIALOGFACTORY->commandMessage(tr("Closing Point: %1/%2").arg(mouse.x).arg(mouse.y));
        trigger();
        return;
    }
    docView->moveRelativeZero(mouse);

    ia->push_back(mouse);
    GUIDIALOGFACTORY->commandMessage(tr("Point: %1/%2").arg(mouse.x).arg(mouse.y));
    switch (getStatus())
    {
        case SetFirstPoint:
            setStatus(SetNextPoint);
            break;
        case SetNextPoint:
            display();
            break;

        default:
            break;
    }
}

/// @brief 更新鼠标按钮提示
void ActionInfoArea::updateMouseButtonHints()
{
    switch (getStatus())
    {
        case SetFirstPoint:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify first point of polygon"), tr("Cancel"));
            break;
        case SetNextPoint:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify next point of polygon"), tr("Cancel"));
            break;
        default:
            GUIDIALOGFACTORY->updateMouseWidget();
            break;
    }
}

/// @brief 更新鼠标光标
void ActionInfoArea::updateMouseCursor()
{
    docView->setMouseCursor(DM::CadCursor);
}
