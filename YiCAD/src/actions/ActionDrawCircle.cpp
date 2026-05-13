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

/// @file ActionDrawCircle.cpp
/// @brief 通过圆心和半径绘制圆的交互动作类实现

#include <QAction>
#include "ActionDrawCircle.h"

#include <QMouseEvent>

#include "Debug.h"
#include "DmCircle.h"
#include "GuiCommandEvent.h"
#include "GuiCoordinateEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Math2d.h"
#include "Preview.h"
#include "Transaction.h"

ActionDrawCircle::ActionDrawCircle(DmDocument* doc, GuiDocumentView* docView)
    : PreviewActionInterface("Draw circles", doc, docView)
    , data(new CircleData())
{
    actionType = DM::ActionDrawCircle;
    reset();
}

ActionDrawCircle::~ActionDrawCircle() = default;

void ActionDrawCircle::reset()
{
    data.reset(new CircleData{});
}

void ActionDrawCircle::init(int status)
{
    PreviewActionInterface::init(status);

    reset();
}

void ActionDrawCircle::trigger()
{
    PreviewActionInterface::trigger();

    Transaction t(tr("Create Circle").toStdString(), pDocument);
    t.start();
    DmCircle* circle = new DmCircle(nullptr, *data);
    circle->setDocument(pDocument);
    pDocument->getEntityTable()->add(circle);
    t.commit();

    docView->moveRelativeZero(circle->getCenter());

    if (getSnapMode()->restriction == DM::RestrictOrthogonal)
    {
        setStatus(-1);
    }
    else
    {
        setStatus(SetCenter);
    }
    reset();
}

void ActionDrawCircle::mouseMoveEvent(QMouseEvent* e)
{
    DmVector mouse = snapPoint(e);
    switch (getStatus())
    {
        case SetCenter:
            data->setCenter(mouse);
            break;

        case SetRadius:
            if (data->getCenter().valid)
            {
                data->setRadius(data->getCenter().distanceTo(mouse));
                deletePreview();
                DmCircle* c = new DmCircle(nullptr, *data);
                c->setDocument(pDocument);
                preview->addEntity(c);
                drawPreview();
            }
            break;

        default:
            break;
    }
}

void ActionDrawCircle::mouseReleaseEvent(QMouseEvent* e)
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

void ActionDrawCircle::coordinateEvent(GuiCoordinateEvent* e)
{
    if (!e)
    {
        return;
    }

    DmVector mouse = e->getCoordinate();

    switch (getStatus())
    {
        case SetCenter:
            data->setCenter(mouse);
            docView->moveRelativeZero(mouse);
            setStatus(SetRadius);
            break;

        case SetRadius:
            if (data->getCenter().valid)
            {
                docView->moveRelativeZero(mouse);
                data->setRadius(data->getCenter().distanceTo(mouse));
                trigger();
            }
            // setStatus(SetCenter);
            finishOrthogonal();
            break;

        default:
            break;
    }
}

void ActionDrawCircle::commandEvent(GuiCommandEvent* e)
{
    QString c = e->getCommand().toLower();

    if (checkCommand("help", c))
    {
        GUIDIALOGFACTORY->commandMessage(msgAvailableCommands() + getAvailableCommands().join(", "));
        return;
    }

    switch (getStatus())
    {
        case SetRadius:
        {
            bool ok = false;
            double r = Math2d::eval(c, &ok);
            if (ok)
            {
                data->setRadius(r);
                e->accept();
                trigger();
            }
            else
            {
                GUIDIALOGFACTORY->commandMessage(tr("Not a valid expression"));
            }
            // setStatus(SetCenter);
        }
            break;

        default:
            break;
    }
}

QStringList ActionDrawCircle::getAvailableCommands()
{
    QStringList cmd;
    return cmd;
}

void ActionDrawCircle::updateMouseButtonHints()
{
    switch (getStatus())
    {
        case SetCenter:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify center"), tr("Cancel"));
            break;
        case SetRadius:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify point on circle"), tr("Back"));
            break;
        default:
            GUIDIALOGFACTORY->updateMouseWidget();
            break;
    }
}

void ActionDrawCircle::showOptions()
{
    ActionInterface::showOptions();
}

void ActionDrawCircle::hideOptions()
{
    ActionInterface::hideOptions();
}

void ActionDrawCircle::updateMouseCursor()
{
    docView->setMouseCursor(DM::CadCursor);
}
