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

/// @file ActionDimLeader.cpp
/// @brief 引线标注交互操作类实现

#include <vector>
#include "ActionDimLeader.h"

#include <QAction>
#include <QMouseEvent>
#include <QInputDialog>

#include "Debug.h"
#include "DmLeader.h"
#include "DmLine.h"
#include "DmMText.h"
#include "GuiCommandEvent.h"
#include "GuiCoordinateEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "DmDocument.h"
#include "Preview.h"
#include "Transaction.h"

struct ActionDimLeader::Points
{
    std::vector<DmVector> points;
};

ActionDimLeader::ActionDimLeader(DmDocument* doc, GuiDocumentView* docView) :
    PreviewActionInterface("Draw leaders", doc, docView), pPoints(new Points{})
{
    actionType = DM::ActionDimLeader;
    reset();
}

ActionDimLeader::~ActionDimLeader() = default;

void ActionDimLeader::reset()
{
    pPoints->points.clear();
}

void ActionDimLeader::init(int status)
{
    PreviewActionInterface::init(status);

    reset();
}

void ActionDimLeader::trigger()
{
    PreviewActionInterface::trigger();

    if (pPoints->points.size() >= 2)
    {
        bool ok = false;
        QString text = QInputDialog::getText(nullptr,
                    tr("Set leader text"), tr("Leader text:"),
                    QLineEdit::Normal, QString(), &ok);
        DmDimensionStyle* pDimStyle =
                    docView->getDocument()->getDimStyleTable()->getActive();

        DmLeader* leader = new DmLeader(
                    nullptr, DmLeaderData(pDimStyle, pPoints->points));
        leader->setDocument(pDocument);
        leader->update();

        Transaction t(tr("Add leader").toStdString(), pDocument);
        t.start();
        pDocument->getEntityTable()->add(leader);

        // 追加文字
        DmMText* mtext = nullptr;
        if (ok && !text.isEmpty())
        {
            DmTextStyle* pStyle = pDimStyle->getDataRef().textStyle();
            double height = pDimStyle->getValidTextHeight();
            MTextData textData(DmVector(0.0, 0.0), height,
                        EMTextVertMode::kTextTop, EMTextHorzMode::kTextLeft,
                        1.0, 10.0, text, pStyle, 0.0);
            textData.setJustification(EMTextMode::kTextMiddleLeft);
            textData.setLineSpacingFactor(1.0);
            mtext = new DmMText(nullptr, textData);
            mtext->setDocument(pDocument);
            mtext->update();
            DmVector textSize = mtext->getWidthHeight();
            DmVector lastPt = pPoints->points.back();
            DmVector lastSecPt = pPoints->points.at(
                        pPoints->points.size() - 2);
            DmVector lastDir = (lastPt - lastSecPt).normalize();
            if (lastDir.x > 0)
            {
                mtext->move(lastPt + DmVector(0.0, 0.0));
            }
            else
            {
                mtext->move(lastPt
                            + DmVector(-textSize.x - height / 2.0, 0.0));
            }
            pDocument->getEntityTable()->add(mtext);
        }
        t.commit();

        deletePreview();
        DmVector rz = docView->getRelativeZero();
        docView->moveRelativeZero(rz);
    }
}

void ActionDimLeader::mouseMoveEvent(QMouseEvent* e)
{
    DmVector mouse = snapPoint(e);
    if (getStatus() == SetEndpoint
        && !pPoints->points.empty())
    {
        deletePreview();
        std::vector<DmVector> pts(pPoints->points);
        pts.emplace_back(mouse);
        DmDimensionStyleTable* pTable =
                    docView->getDocument()->getDimStyleTable();
        DmLeaderData ldata(pTable->getActive(), pts);

        DmLeader* leader = new DmLeader(
                    preview->getEntityContainer(), ldata);
        leader->setDocument(pDocument);
        leader->update();
        preview->addEntity(leader);
        drawPreview();
    }
}

void ActionDimLeader::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        GuiCoordinateEvent ce(snapPoint(e));
        coordinateEvent(&ce);
    }
    else if (e->button() == Qt::RightButton)
    {
        if (getStatus() == SetEndpoint
            && pPoints->points.size() >= 2)
        {
            trigger();
            finishOrthogonal();
            reset();
            setStatus(SetStartpoint);
        }
        else
        {
            deletePreview();
            init(getStatus() - 1);
        }
    }
}

void ActionDimLeader::keyPressEvent(QKeyEvent* e)
{
    if (getStatus() == SetEndpoint
        && e->key() == Qt::Key_Enter)
    {
        trigger();
        reset();
        setStatus(SetStartpoint);
    }
    else
    {
        ActionInterface::keyPressEvent(e);
    }
}

void ActionDimLeader::coordinateEvent(GuiCoordinateEvent* e)
{
    if (!e)
    {
        return;
    }

    DmVector mouse = e->getCoordinate();

    switch (getStatus())
    {
        case SetStartpoint:
            pPoints->points.clear();
            pPoints->points.push_back(mouse);
            setStatus(SetEndpoint);
            docView->moveRelativeZero(mouse);
            break;

        case SetEndpoint:
            pPoints->points.push_back(mouse);
            docView->moveRelativeZero(mouse);
            break;

        default:
            break;
    }
}

void ActionDimLeader::commandEvent(GuiCommandEvent* e)
{
    QString c = e->getCommand().toLower();

    if (checkCommand("help", c))
    {
        GUIDIALOGFACTORY->commandMessage(
                    msgAvailableCommands()
                    + getAvailableCommands().join(", "));
        return;
    }

    // 回车完成
    if (c == "")
    {
        trigger();
        reset();
        setStatus(SetStartpoint);
    }
}

QStringList ActionDimLeader::getAvailableCommands()
{
    QStringList cmd;

    return cmd;
}

void ActionDimLeader::updateMouseButtonHints()
{
    switch (getStatus())
    {
        case SetStartpoint:
            GUIDIALOGFACTORY->updateMouseWidget(
                        tr("Specify target point"), tr("Cancel"));
            break;
        case SetEndpoint:
            GUIDIALOGFACTORY->updateMouseWidget(
                        tr("Specify next point"), tr("Finish"));
            break;
        default:
            GUIDIALOGFACTORY->updateMouseWidget();
            break;
    }
}

void ActionDimLeader::updateMouseCursor()
{
    docView->setMouseCursor(DM::CadCursor);
}
