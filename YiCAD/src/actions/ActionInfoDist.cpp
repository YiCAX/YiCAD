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

/// @file ActionInfoDist.cpp
/// @brief 距离测量 Action 类的实现

#include <QAction>
#include "ActionInfoDist.h"

#include <QMouseEvent>

#include "Debug.h"
#include "DmDocument.h"
#include "DmLine.h"
#include "GuiCoordinateEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Preview.h"

struct ActionInfoDist::Points
{
    DmVector point1;
    DmVector point2;
};

/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionInfoDist::ActionInfoDist(DmDocument* doc, GuiDocumentView* docView) :
    PreviewActionInterface("Info Dist", doc, docView), pPoints(new Points{})
{
    actionType = DM::ActionInfoDist;
}

ActionInfoDist::~ActionInfoDist() = default;

/// @brief 初始化 Action 状态
/// @param status 初始状态
void ActionInfoDist::init(int status)
{
    ActionInterface::init(status);
}

/// @brief 触发距离计算和显示
void ActionInfoDist::trigger()
{
    if (pPoints->point1.valid && pPoints->point2.valid)
    {
        const DmVector dV = pPoints->point2 - pPoints->point1;
        QStringList dists;
        for (double a : {dV.magnitude(), dV.x, dV.y})
        {
            dists << DmUnits::formatLinear(a, pDocument->getUnit(), pDocument->getLinearFormat(), pDocument->getLinearPrecision());
        }

        const QString angle = DmUnits::formatAngle(dV.angle(), pDocument->getAngleFormat(), pDocument->getAnglePrecision());

        GUIDIALOGFACTORY->commandMessage(tr("Distance: %1 Cartesian: (%2 , %3), Polar: (%4<%5)").arg(dists[0]).arg(dists[1]).arg(dists[2]).arg(dists[0]).arg(angle));
    }
}

/// @brief 鼠标移动事件处理
/// @param e 鼠标事件
void ActionInfoDist::mouseMoveEvent(QMouseEvent* e)
{
    if (getStatus() == SetPoint1 || getStatus() == SetPoint2)
    {
        DmVector mouse = snapPoint(e);
        switch (getStatus())
        {
            case SetPoint1:
                break;

            case SetPoint2:
                if (pPoints->point1.valid)
                {
                    pPoints->point2 = mouse;

                    deletePreview();

                    preview->addEntity(new DmLine(nullptr, pPoints->point1, pPoints->point2));

                    drawPreview();
                }
                break;

            default:
                break;
        }
    }
}

/// @brief 鼠标释放事件处理
/// @param e 鼠标事件
void ActionInfoDist::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        GuiCoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    }
    else if (e->button() == Qt::RightButton)
    {
        deletePreview();
        init(getStatus() - 1);
    }
}

/// @brief 坐标事件处理
/// @param e 坐标事件
void ActionInfoDist::coordinateEvent(GuiCoordinateEvent* e)
{
    if (e == nullptr)
    {
        return;
    }

    DmVector mouse = e->getCoordinate();

    switch (getStatus())
    {
        case SetPoint1:
            pPoints->point1 = mouse;
            docView->moveRelativeZero(pPoints->point1);
            setStatus(SetPoint2);
            break;

        case SetPoint2:
            if (pPoints->point1.valid)
            {
                pPoints->point2 = mouse;
                deletePreview();
                docView->moveRelativeZero(pPoints->point2);
                trigger();
                setStatus(SetPoint1);
            }
            break;

        default:
            break;
    }
}

/// @brief 更新鼠标按钮提示
void ActionInfoDist::updateMouseButtonHints()
{
    switch (getStatus())
    {
        case SetPoint1:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify first point of distance"), tr("Cancel"));
            break;
        case SetPoint2:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify second point of distance"), tr("Back"));
            break;
        default:
            GUIDIALOGFACTORY->updateMouseWidget();
            break;
    }
}

/// @brief 更新鼠标光标
void ActionInfoDist::updateMouseCursor()
{
    docView->setMouseCursor(DM::CadCursor);
}
