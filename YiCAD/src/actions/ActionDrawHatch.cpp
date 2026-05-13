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

/// @file ActionDrawHatch.cpp
/// @brief 填充图案（Hatch）绘制交互动作实现

#include <iostream>
#include "ActionDrawHatch.h"

#include <QAction>
#include <QMouseEvent>

#include "Debug.h"
#include "DmHatch.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Information.h"
#include "ApplicationWindow.h"
#include "UIDlgHatch.h"
#include "Transaction.h"
#include "Preview.h"
#include <QMessageBox>

ActionDrawHatch::ActionDrawHatch(DmDocument* doc, GuiDocumentView* docView, bool isModify)
    : PreviewActionInterface("Draw Hatch", doc, docView)
    , m_pData(new HatchData())
{
    Q_UNUSED(isModify);
    actionType = DM::ActionDrawHatch;
}

ActionDrawHatch::~ActionDrawHatch() = default;

void ActionDrawHatch::init(int status)
{
    ActionInterface::init(status);
    auto table = pDocument->getEntityTable();

    for (auto entity : *table)
    {
        if (entity->isSelected())
        {
            m_hasSelectEntity = true;
            break;
        }
    }

    // 仅在没有选择实体时绑定视图变化信号
    if (!m_hasSelectEntity)
    {
        connect(docView, SIGNAL(viewChanged()), this, SLOT(slotViewChanged()));
    }

    DmHatch tmp(nullptr, *m_pData);
    tmp.setDocument(pDocument);

    if (GUIDIALOGFACTORY->requestHatchDialog(&tmp))
    {
        *m_pData = tmp.getData();
        if (!m_hasSelectEntity)
        {
            slotViewChanged();
        }
        else
        {
            initFindMethodForSelectedEntities();
        }

        setStatus(PrepareAdd);
    }
    else
    {
        finish(false);
    }
}

void ActionDrawHatch::trigger()
{
    if (!m_pickPoint.valid)
    {
        return;
    }

    auto table = pDocument->getEntityTable();
    m_findMethod.calculate();
    auto region = m_findMethod.findRegionContainPoint(m_pickPoint);

    if (region)
    {
        auto dmRegion = region->getDmRegion();
        Transaction t(tr("Hatch").toStdString(), pDocument);
        t.start();
        HatchData d(*m_pData);
        d.setBoundary(dmRegion);
        DmHatch* hatch = new DmHatch(nullptr, d);
        hatch->setDocument(pDocument);
        hatch->update();
        table->add(hatch);
        t.commit();
    }
    else
    {
        QMessageBox::information(nullptr, tr("Tips"), tr("Failure to create hatch, can't find the region!"));
    }
}

void ActionDrawHatch::mouseMoveEvent(QMouseEvent* e)
{
    if (getStatus() == PrepareAdd)
    {
        deletePreview();
        DmVector mouseCoord = docView->toGraph(e->x(), e->y());
        m_findMethod.calculate();
        auto region = m_findMethod.findRegionContainPoint(mouseCoord);

        if (region)
        {
            auto dmRegion = region->getDmRegion();
            HatchData d(*m_pData);
            d.setBoundary(dmRegion);
            DmHatch* hatch = new DmHatch(preview->getEntityContainer(), d);
            hatch->setDocument(pDocument);
            hatch->update();
            preview->addEntity(hatch);
        }
        drawPreview();
    }
}

void ActionDrawHatch::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        snapPoint(e);

        switch (getStatus())
        {
        case ShowDialog:
            break;
        case PrepareAdd:
            {
                m_pickPoint = docView->toGraph(e->x(), e->y());
                trigger();
            }
            break;
        default:
            break;
        }
    }
    else if (e->button() == Qt::RightButton)
    {
        finish(false);
    }
}

void ActionDrawHatch::updateMouseButtonHints()
{
    switch (getStatus())
    {
    case ShowDialog:
        break;
    case PrepareAdd:
        {
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify point to create hatch."), tr("Cancel"));
        }
        break;
    default:
        GUIDIALOGFACTORY->updateMouseWidget();
        break;
    }
}

void ActionDrawHatch::updateMouseCursor()
{
    docView->setMouseCursor(DM::CadCursor);
}

void ActionDrawHatch::initFindMethodForSelectedEntities()
{
    auto table = pDocument->getEntityTable();

    for (auto entity : *table)
    {
        if (entity->isSelected())
        {
            m_findMethod.addEntity(entity);
        }
    }
}

void ActionDrawHatch::slotViewChanged()
{
    DmRect rect = docView->getViewRect();
    std::vector<DmEntity*> ents;
    pDocument->searchEntities(rect.minP(), rect.maxP(), ents, true, false);

    for (auto entity : ents)
    {
        m_findMethod.addEntity(entity);
    }
}
