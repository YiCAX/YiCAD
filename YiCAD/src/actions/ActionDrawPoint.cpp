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

/// @file ActionDrawPoint.cpp
/// @brief 绘制点动作类的实现

#include "ActionDrawPoint.h"

#include <QMouseEvent>

#include "DmPoint.h"
#include "GuiCommandEvent.h"
#include "GuiCoordinateEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Transaction.h"

ActionDrawPoint::ActionDrawPoint(DmDocument* doc, GuiDocumentView* docView) :
    PreviewActionInterface("Draw Points", doc, docView), m_pt(new DmVector{})
{
    actionType = DM::ActionDrawPoint;
}

ActionDrawPoint::~ActionDrawPoint() = default;

void ActionDrawPoint::trigger()
{
    if (m_pt->valid)
    {
        Transaction t(tr("Create Point").toStdString(), pDocument);
        t.start();
        DmPoint* point = new DmPoint(nullptr, PointData(*m_pt));
        point->setDocument(pDocument);
        pDocument->getEntityTable()->add(point);
        t.commit();
        docView->moveRelativeZero(*m_pt);
    }
}

void ActionDrawPoint::mouseMoveEvent(QMouseEvent* e)
{
    snapPoint(e);
}

void ActionDrawPoint::mouseReleaseEvent(QMouseEvent* e)
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
    else
    {
        // TODO: 处理其他鼠标按钮事件
    }
}

void ActionDrawPoint::coordinateEvent(GuiCoordinateEvent* e)
{
    if (e == nullptr)
    {
        return;
    }

    DmVector mouse = e->getCoordinate();

    *m_pt = mouse;
    trigger();
}

void ActionDrawPoint::commandEvent(GuiCommandEvent* e)
{
    QString c = e->getCommand().toLower();

    if (checkCommand("help", c))
    {
        GUIDIALOGFACTORY->commandMessage(msgAvailableCommands() + getAvailableCommands().join(", "));
        return;
    }
    else
    {
        // TODO: 处理其他命令
    }
}

QStringList ActionDrawPoint::getAvailableCommands()
{
    return {};
}

void ActionDrawPoint::updateMouseButtonHints()
{
    switch (getStatus())
    {
        case 0:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify location"), tr("Cancel"));
            break;
        default:
            GUIDIALOGFACTORY->updateMouseWidget();
            break;
    }
}

void ActionDrawPoint::updateMouseCursor()
{
    docView->setMouseCursor(DM::CadCursor);
}
