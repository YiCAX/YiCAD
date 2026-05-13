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

/// @file ActionModifySingleOffset.cpp
/// @brief 单个实体偏移交互命令实现

#include "ActionModifySingleOffset.h"

#include <cmath>
#include <QAction>
#include <QMouseEvent>

#include "Debug.h"
#include "DmArc.h"
#include "DmLine.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Information.h"
#include "Modification.h"
#include "Preview.h"
#include "Transaction.h"

/// @brief 默认偏移距离
constexpr double DEFAULT_OFFSET_DISTANCE = 30.0;

/// @brief 默认偏移数量
constexpr unsigned DEFAULT_OFFSET_NUMBER = 1;

ActionModifySingleOffset::ActionModifySingleOffset(DmDocument* doc, GuiDocumentView* docView)
    : PreviewActionInterface("Modify Single Offset", doc, docView)
    , m_pOriginalEntity(nullptr)
    , m_pData(new OffsetData())
{
    actionType = DM::ActionModifySingleOffset;

    m_pData->distance = DEFAULT_OFFSET_DISTANCE;
    m_pData->number = DEFAULT_OFFSET_NUMBER;
    m_pData->useCurrentAttributes = true;
    m_pData->useCurrentLayer = true;
    m_pData->coord = DmVector();
}

ActionModifySingleOffset::~ActionModifySingleOffset()
{
    unhighlightEntity();
}

void ActionModifySingleOffset::unhighlightEntity()
{
    if (prevHighlighted)
    {
        prevHighlighted->setHighlighted(false);
        docView->specifyDocumentModified();
        docView->redraw();
        prevHighlighted = nullptr;
    }
}

void ActionModifySingleOffset::init(int status)
{
    GUIDIALOGFACTORY->updateMouseWidget(tr("Choose the original entity"));

    PreviewActionInterface::init(status);
}

/// @brief 执行偏移操作
///
/// 根据设置的偏移距离和方向，对选中的原始实体执行偏移操作。
/// 如果偏移成功，将新实体添加到文档中。
void ActionModifySingleOffset::trigger()
{
    if (!m_pOriginalEntity || !m_pData->coord.valid)
        return;

    deletePreview();
    unhighlightEntity();

    Transaction t(tr("Offset").toStdString(), pDocument);
    t.start();

    DmEntity* ec = m_pOriginalEntity->clone();
    ec->setLayerToActive();
    ec->setPenToActive();
    ec->setHighlighted(false);

    if (!ec->offset(m_pData->coord, m_pData->distance))
    {
        delete ec;
    }
    else
    {
        pDocument->getEntityTable()->add(ec);
    }

    t.commit();

    m_pOriginalEntity = nullptr;
    m_pData->coord = DmVector();
    docView->redraw();
    GUIDIALOGFACTORY->updateSelectionWidget(pDocument->getEntityTable()->countSelect());
    finish(false);
}

void ActionModifySingleOffset::mouseMoveEvent(QMouseEvent* e)
{
    DmEntity* se = catchEntity(e);

    switch (getStatus())
    {
        case ChooseEntity:
        {
            // 未选中实体时：悬停高亮
            if (!m_pOriginalEntity)
            {
                if (se != prevHighlighted)
                {
                    unhighlightEntity();
                    if (se)
                    {
                        se->setHighlighted(true);
                        docView->specifyDocumentModified();
                        docView->redraw();
                        prevHighlighted = se;
                    }
                }
            }

            // 已选中实体后：偏移预览
            if (m_pOriginalEntity)
            {
                m_pData->coord = docView->toGraph(e->pos().x(), e->pos().y());

                deletePreview();
                DmEntity* clone = m_pOriginalEntity->clone();
                if (clone->offset(m_pData->coord, m_pData->distance))
                {
                    preview->addEntity(clone);
                }
                else
                {
                    delete clone;
                }
                drawPreview();
            }
        }
            break;

        default:
            break;
    }
}

void ActionModifySingleOffset::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        switch (getStatus())
        {
            case ChooseEntity:
            {
                if (!m_pOriginalEntity)
                {
                    m_pOriginalEntity = catchEntity(e);
                    if (m_pOriginalEntity)
                    {
                        unhighlightEntity();
                        m_pOriginalEntity->setHighlighted(true);
                        docView->specifyDocumentModified();
                        docView->redraw();
                        prevHighlighted = m_pOriginalEntity;
                    }
                }
                else
                {
                    trigger();
                }
            }
                break;

            default:
                break;
        }
    }
    else if (e->button() == Qt::RightButton)
    {
        deleteSnapper();
        unhighlightEntity();
        deletePreview();

        m_pOriginalEntity = nullptr;
        m_pData->coord = DmVector();

        init(getStatus() - 1);
    }
}

void ActionModifySingleOffset::showOptions()
{
    ActionInterface::showOptions();
    GUIDIALOGFACTORY->requestModifySingleOffsetOptions(m_pData->distance, true, false);
}

void ActionModifySingleOffset::hideOptions()
{
    ActionInterface::hideOptions();
    GUIDIALOGFACTORY->requestModifySingleOffsetOptions(m_pData->distance, false, false);
}

void ActionModifySingleOffset::setDist(const double& d)
{
    m_pData->distance = d;
}

double ActionModifySingleOffset::getDist() const
{
    return m_pData->distance;
}

void ActionModifySingleOffset::setNumber(unsigned n)
{
    m_pData->number = n;
}

int ActionModifySingleOffset::getNumber() const
{
    return m_pData->number;
}

void ActionModifySingleOffset::updateMouseCursor()
{
    docView->setMouseCursor(DM::SelectCursor);
}
