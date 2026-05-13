/*
 * Copyright (C) 2026 YiCAD Team
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

/// @file ActionDrawCircle2p.cpp
/// @brief 通过两点绘制圆的交互动作类实现

#include <QAction>
#include "ActionDrawCircle2p.h"

#include <QMouseEvent>

#include "DmCircle.h"
#include "GuiCommandEvent.h"
#include "GuiCoordinateEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Preview.h"
#include "Transaction.h"

struct ActionDrawCircle2P::Points
{
    DmVector point1; ///< 第一个点
    DmVector point2; ///< 第二个点
    DmVector mouse;  ///< 当前鼠标所在点
};

ActionDrawCircle2P::ActionDrawCircle2P(DmDocument* doc, GuiDocumentView* docView)
    : PreviewActionInterface("Draw circles", doc, docView)
    , data(new CircleData()), pPoints(new Points{})
{
    actionType = DM::ActionDrawCircle2P;
    reset();
}

ActionDrawCircle2P::~ActionDrawCircle2P() = default;

void ActionDrawCircle2P::reset()
{
    data.reset(new CircleData{});
    pPoints->point1 = {};
    pPoints->point2 = {};
}

void ActionDrawCircle2P::init(int status)
{
    PreviewActionInterface::init(status);

    reset();
}

void ActionDrawCircle2P::trigger()
{
    PreviewActionInterface::trigger();

    preparePreview();
    if (data->isValid())
    {
        Transaction t(tr("Create Circle2p").toStdString(), pDocument);
        t.start();
        DmCircle* circle = new DmCircle(nullptr, *data);
        circle->setDocument(pDocument);
        pDocument->getEntityTable()->add(circle);
        t.commit();

        DmVector rz = docView->getRelativeZero();
        docView->moveRelativeZero(rz);

        if (getSnapMode()->restriction == DM::RestrictOrthogonal)
        {
            setStatus(-1);
        }
        else
        {
            setStatus(SetPoint1);
        }
        reset();
    }
    else
    {
        GUIDIALOGFACTORY->requestWarningDialog(tr("Invalid Circle data."));
    }
}

void ActionDrawCircle2P::preparePreview()
{
    data.reset(new CircleData{});
    if (pPoints->point1.valid && pPoints->point2.valid)
    {
        DmCircle circle(nullptr, *data);
        bool success = circle.createFrom2P(pPoints->point1, pPoints->point2);
        if (success)
        {
            data.reset(new CircleData(circle.getData()));
        }
    }
}

void ActionDrawCircle2P::mouseMoveEvent(QMouseEvent* e)
{
    DmVector mouse = snapPoint(e);
    pPoints->mouse = mouse;
    switch (getStatus())
    {
        case SetPoint1:
            pPoints->point1 = mouse;
            break;

        case SetPoint2:
            pPoints->point2 = mouse;
            preparePreview();
            if (data->isValid())
            {
                DmCircle* circle = new DmCircle(preview->getEntityContainer(), *data);
                circle->setDocument(PreviewActionInterface::pDocument);
                circle->setDocument(pDocument);
                deletePreview();
                preview->addEntity(circle);
                drawPreview();
            }
            break;

        default:
            break;
    }
}

void ActionDrawCircle2P::mouseReleaseEvent(QMouseEvent* e)
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

void ActionDrawCircle2P::coordinateEvent(GuiCoordinateEvent* e)
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
            docView->moveRelativeZero(mouse);
            setStatus(SetPoint2);
            break;

        case SetPoint2:
            pPoints->point2 = mouse;
            docView->moveRelativeZero(mouse);
            trigger();
            finishOrthogonal();
            break;

        default:
            break;
    }
}

void ActionDrawCircle2P::commandEvent(GuiCommandEvent* e)
{
    QString c = e->getCommand().toLower();
    bool isLength = false;
    double length = c.toDouble(&isLength);
    DmVector endPt, startPt;
    DmVector vec(1.0, 0.0);

    switch (getStatus())
    {
        case SetPoint1:
            if (checkCommand("help", c))
            {
                GUIDIALOGFACTORY->commandMessage(msgAvailableCommands() + getAvailableCommands().join(", "));
                return;
            }
            break;
        case SetPoint2:
        {
            if (isLength)
            {
                startPt = pPoints->point1;
                if (pPoints->mouse.valid)
                {
                    vec = (pPoints->mouse - startPt).normalize();
                }
                endPt = startPt + vec * length;
                pPoints->point2 = endPt;
                docView->moveRelativeZero(endPt);
                trigger();
                finishOrthogonal();
            }
        }
            break;
        default:
            return;
    }
}

QStringList ActionDrawCircle2P::getAvailableCommands()
{
    QStringList cmd;
    return cmd;
}

void ActionDrawCircle2P::updateMouseButtonHints()
{
    switch (getStatus())
    {
        case SetPoint1:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify first point"), tr("Cancel"));
            break;
        case SetPoint2:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify second point"), tr("Back"));
            break;
        default:
            GUIDIALOGFACTORY->updateMouseWidget();
            break;
    }
}

void ActionDrawCircle2P::updateMouseCursor()
{
    docView->setMouseCursor(DM::CadCursor);
}
