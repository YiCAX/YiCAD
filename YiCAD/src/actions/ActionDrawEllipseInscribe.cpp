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

/// @file ActionDrawEllipseInscribe.cpp
/// @brief 通过内切四边形边线绘制椭圆的交互动作实现

#include <QAction>
#include "ActionDrawEllipseInscribe.h"

#include <QMouseEvent>

#include "Debug.h"
#include "DmEllipse.h"
#include "DmLine.h"
#include "GuiCommandEvent.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Preview.h"
#include "Transaction.h"

struct ActionDrawEllipseInscribe::Points
{
    std::vector<DmLine*> lines; ///< 已选中的四条边线
    EllipseData eData;          ///< 计算得到的椭圆数据
    bool valid{ false };        ///< 椭圆数据是否有效
};

ActionDrawEllipseInscribe::ActionDrawEllipseInscribe(DmDocument* doc, GuiDocumentView* docView) :
    PreviewActionInterface("Draw ellipse inscribed", doc, docView), pPoints(new Points{})
{
    actionType = DM::ActionDrawEllipseInscribe;
}

ActionDrawEllipseInscribe::~ActionDrawEllipseInscribe() = default;

void ActionDrawEllipseInscribe::clearLines(bool checkStatus)
{
    while (pPoints->lines.size())
    {
        if (checkStatus && static_cast<int>(pPoints->lines.size()) <= getStatus())
        {
            break;
        }
        pPoints->lines.back()->setHighlighted(false);
        docView->redraw();
        pPoints->lines.pop_back();
    }
}

void ActionDrawEllipseInscribe::init(int status)
{
    PreviewActionInterface::init(status);
    if (status >= 0)
    {
        Snapper::suspend();
    }
    clearLines(true);
}

void ActionDrawEllipseInscribe::finish(bool updateTB)
{
    clearLines(false);
    PreviewActionInterface::finish(updateTB);
}

void ActionDrawEllipseInscribe::trigger()
{
    PreviewActionInterface::trigger();

    DmEllipse* ellipse = new DmEllipse(nullptr, pPoints->eData);
    ellipse->setDocument(PreviewActionInterface::pDocument);
    ellipse->update();
    deletePreview();

    Transaction t(tr("Add ellipse").toStdString(), pDocument);
    t.start();
    pDocument->getEntityTable()->add(ellipse);
    t.commit();

    for (DmLine* const p : pPoints->lines)
    {
        if (!p)
        {
            continue;
        }
        p->setHighlighted(false);
        docView->redraw();
    }
    drawSnapper();

    clearLines(false);
    if (getSnapMode()->restriction == DM::RestrictOrthogonal)
    {
        setStatus(-1);
    }
    else
    {
        setStatus(SetLine1);
    }
}

void ActionDrawEllipseInscribe::mouseMoveEvent(QMouseEvent* e)
{
    if (getStatus() == SetLine4)
    {
        DmEntity* en = catchEntity(e, DM::EntityLine, DM::ResolveAll);
        if (!en)
        {
            return;
        }
        if (!(en->isVisible() && en->getEntityType() == DM::EntityLine))
        {
            return;
        }
        for (auto p : pPoints->lines)
        {
            if (en == p)
            {
                return; // 不重复选择同一条线
            }
        }

        clearLines(true);
        pPoints->lines.push_back(static_cast<DmLine*>(en));
        if (preparePreview())
        {
            pPoints->lines.back()->setHighlighted(true);
            docView->redraw();
            DmEllipse* ellipse = new DmEllipse(preview->getEntityContainer(), pPoints->eData);
            ellipse->setDocument(pDocument);
            preview->addEntity(ellipse);
            drawPreview();
        }
    }
}

bool ActionDrawEllipseInscribe::preparePreview()
{
    pPoints->valid = false;
    if (getStatus() == SetLine4)
    {
        DmEllipse e(nullptr, EllipseData());
        pPoints->valid = e.createInscribeQuadrilateral(pPoints->lines);
        if (pPoints->valid)
        {
            pPoints->eData = e.getData();
        }
        else if (GUIDIALOGFACTORY)
        {
            GUIDIALOGFACTORY->commandMessage(tr("Can not determine uniquely an ellipse"));
        }
    }
    return pPoints->valid;
}

void ActionDrawEllipseInscribe::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (!e)
        {
            return;
        }
        DmEntity* en = catchEntity(e, DM::EntityLine, DM::ResolveAll);
        if (!en)
        {
            return;
        }
        if (!(en->isVisible() && en->getEntityType() == DM::EntityLine))
        {
            return;
        }
        for (int i = 0; i < getStatus(); ++i)
        {
            if (en->getId() == pPoints->lines[i]->getId())
            {
                return; // 不重复选择同一条线
            }
        }
        clearLines(true);
        pPoints->lines.push_back(static_cast<DmLine*>(en));

        switch (getStatus())
        {
        case SetLine1:
        case SetLine2:
        case SetLine3:
            {
                DmEntity* li = en->clone();
                li->setDocument(PreviewActionInterface::pDocument);
                li->setHighlighted(true);
                li->setParent(nullptr);
                preview->addEntity(li);
                setStatus(getStatus() + 1);
                break;
            }

        case SetLine4:
            if (preparePreview())
            {
                trigger();
                finishOrthogonal();
            }
            deletePreview();
        default:
            break;
        }
    }
    else if (e->button() == Qt::RightButton)
    {
        // 返回上一个状态
        if (getStatus() > 0)
        {
            clearLines(true);
            pPoints->lines.back()->setHighlighted(false);
            docView->redraw();
            pPoints->lines.pop_back();
            deletePreview();
        }
        init(getStatus() - 1);
    }
}

QStringList ActionDrawEllipseInscribe::getAvailableCommands()
{
    QStringList cmd;
    return cmd;
}

void ActionDrawEllipseInscribe::updateMouseButtonHints()
{
    switch (getStatus())
    {
    case SetLine1:
        GUIDIALOGFACTORY->updateMouseWidget(tr("Specify the first line"), tr("Cancel"));
        break;

    case SetLine2:
        GUIDIALOGFACTORY->updateMouseWidget(tr("Specify the second line"), tr("Back"));
        break;

    case SetLine3:
        GUIDIALOGFACTORY->updateMouseWidget(tr("Specify the third line"), tr("Back"));
        break;

    case SetLine4:
        GUIDIALOGFACTORY->updateMouseWidget(tr("Specify the fourth line"), tr("Back"));
        break;

    default:
        GUIDIALOGFACTORY->updateMouseWidget();
        break;
    }
}

void ActionDrawEllipseInscribe::updateMouseCursor()
{
    docView->setMouseCursor(DM::SelectCursor);
}
