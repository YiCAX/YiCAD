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

/// @file ActionPolylineDel.cpp
/// @brief 多段线删除节点操作实现

#include "ActionPolylineDel.h"

#include <QAction>
#include <QMouseEvent>

#include "Debug.h"
#include "DmPolyline.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Modification.h"
#include "Preview.h"
#include "Transaction.h"

// TODO: 此阈值含义待确认（点在线上的容差），建议从配置中获取
constexpr int POINT_ON_ENTITY_TOLERANCE = 3;   ///< 判断点是否在线上的容差值

/// @brief 构造函数，初始化删除节点操作
/// @param [in] doc 文档指针
/// @param [in] docView 文档视图指针
ActionPolylineDel::ActionPolylineDel(DmDocument* doc, GuiDocumentView* docView) :
    PreviewActionInterface("Delete node", doc, docView), delEntity(nullptr),
    delPoint(new DmVector{})
{
    actionType = DM::ActionPolylineDel;
}

ActionPolylineDel::~ActionPolylineDel() = default;

/// @brief 初始化操作状态，重置内部数据
/// @param [in] status 初始状态值，默认为0
void ActionPolylineDel::init(int status)
{
    ActionInterface::init(status);
    delEntity = nullptr;
    *delPoint = {};
}

/// @brief 完成操作，清除高亮状态
/// @param [in] updateTB 是否更新工具栏
void ActionPolylineDel::finish(bool updateTB)
{
    if (delEntity)
    {
        delEntity->setHighlighted(false);
        docView->specifyDocumentModified();
        docView->redraw();
    }

    PreviewActionInterface::finish(updateTB);
}

/// @brief 执行删除节点操作，从多段线中移除指定顶点
void ActionPolylineDel::trigger()
{
    if (delEntity && delPoint->valid && delEntity->isPointOnEntity(*delPoint, POINT_ON_ENTITY_TOLERANCE))
    {
        delEntity->setHighlighted(false);

        DmPolyline* poly = static_cast<DmPolyline*>(delEntity);
        Transaction t(tr("Append polyline point").toStdString(), pDocument);
        t.start();
        pDocument->getEntityTable()->startModify(poly);

        int vCount = poly->getVertexCount();
        if (vCount > 2)
        {
            double minDistSquare = DM_MAXDOUBLE * DM_MAXDOUBLE;
            int minIdx = -1;
            for (int i = 0; i < vCount; i++)
            {
                DmVector v = poly->getVertexAt(i);
                double ds2 = v.squaredTo(*delPoint);
                if (ds2 < minDistSquare)
                {
                    minDistSquare = ds2;
                    minIdx = i;
                }
            }

            if (minIdx != -1)
            {
                poly->getDataRef().removeVertex(minIdx);
                poly->update();
            }
        }

        t.commit();

        *delPoint = DmVector(false);
        GUIDIALOGFACTORY->updateSelectionWidget(pDocument->getEntityTable()->countSelect());
    }
}

/// @brief 处理鼠标移动事件，根据当前状态更新捕捉点
/// @param [in] e 鼠标事件指针
void ActionPolylineDel::mouseMoveEvent(QMouseEvent* e)
{
    switch (getStatus())
    {
        case ChooseEntity:
            break;

        case SetDelPoint:
            snapPoint(e);
            break;

        default:
            break;
    }
}

/// @brief 处理鼠标释放事件，根据当前状态执行选择或删除操作
/// @param [in] e 鼠标事件指针
void ActionPolylineDel::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        switch (getStatus())
        {
            case ChooseEntity:
                delEntity = catchEntity(e);
                if (delEntity == nullptr)
                {
                    GUIDIALOGFACTORY->commandMessage(tr("No Entity found."));
                }
                else if (delEntity->getEntityType() != DM::EntityPolyline)
                {
                    GUIDIALOGFACTORY->commandMessage(tr("Entity must be a polyline."));
                }
                else
                {
                    snapPoint(e);
                    delEntity->setHighlighted(true);
                    docView->specifyDocumentModified();
                    setStatus(SetDelPoint);
                    docView->redraw();
                }
                break;

            case SetDelPoint:
            {
                DmVector pt = snapPoint(e);
                double dist = DM_MAXDOUBLE;
                DmPolyline* poly = static_cast<DmPolyline*>(delEntity);

                *delPoint = poly->getNearestPointOnEntity(pt, true, &dist);
                if (delEntity == nullptr)
                {
                    GUIDIALOGFACTORY->commandMessage(tr("No Entity found."));
                }
                else if (!delPoint->valid)
                {
                    GUIDIALOGFACTORY->commandMessage(tr("Deleting point is invalid."));
                }
                else if (!delEntity->isPointOnEntity(*delPoint, POINT_ON_ENTITY_TOLERANCE))
                {
                    GUIDIALOGFACTORY->commandMessage(tr("Deleting point is not on entity."));
                }
                else
                {
                    deleteSnapper();
                    trigger();
                }
                break;
            }

            default:
                break;
        }
    }
    else if (e->button() == Qt::RightButton)
    {
        deleteSnapper();
        if (delEntity)
        {
            delEntity->setHighlighted(false);
            docView->specifyDocumentModified();
            docView->redraw();
        }

        init(getStatus() - 1);
    }
}

/// @brief 更新鼠标按钮提示文本
void ActionPolylineDel::updateMouseButtonHints()
{
    switch (getStatus())
    {
        case ChooseEntity:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify polyline to delete node"), tr("Cancel"));
            break;

        case SetDelPoint:
            GUIDIALOGFACTORY->updateMouseWidget(tr("Specify deleting node's point"), tr("Back"));
            break;

        default:
            GUIDIALOGFACTORY->updateMouseWidget();
            break;
    }
}

/// @brief 更新鼠标光标样式为选择光标
void ActionPolylineDel::updateMouseCursor()
{
    docView->setMouseCursor(DM::SelectCursor);
}
