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

/// @file ActionSelectSingle.cpp
/// @brief 单选实体Action实现

#include "ActionSelectSingle.h"

#include <QAction>
#include <QMouseEvent>

#include "Debug.h"
#include "GuiDialogFactory.h"
#include "Selection.h"

/// @brief 构造函数，初始化单选操作
/// @param [in] doc 文档指针
/// @param [in] docView 文档视图指针
/// @param [in] action_select 调用此选择动作的父Action指针
/// @param [in] entityTypeList 允许选择的实体类型列表
ActionSelectSingle::ActionSelectSingle(DmDocument* doc,
                                       GuiDocumentView* docView,
                                       ActionInterface* action_select,
                                       std::list<DM::EntityType> const& entityTypeList)
    : ActionInterface("Select Entities", doc, docView)
    , entityTypeList(entityTypeList)
    , en(nullptr)
    , actionSelect(action_select)
{
    actionType = DM::ActionSelectSingle;
}

/// @brief 执行选择操作：如果捕获到匹配的实体，执行单选
void ActionSelectSingle::trigger()
{
    bool typeMatch = true;
    if (en && entityTypeList.size())
    {
        typeMatch = (std::find(entityTypeList.begin(), entityTypeList.end(),
                               en->getEntityType())
                     != entityTypeList.end());
    }
    if (en && typeMatch)
    {
        Selection s(pDocument, docView);
        s.selectSingle(en);

        GUIDIALOGFACTORY->updateSelectionWidget(
            pDocument->getEntityTable()->countSelect());
    }
}

/// @brief 处理键盘按键事件，Esc取消选择，Enter确认选择
/// @param [in] e 键盘事件指针
void ActionSelectSingle::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Escape)
    {
        finish(false);
        actionSelect->keyPressEvent(e);
    }

    if (pDocument->getEntityTable()->hasSelect()
        && e->key() == Qt::Key_Enter)
    {
        finish(false);
        actionSelect->keyPressEvent(e);
    }
}

/// @brief 处理鼠标释放事件，执行实体捕获与选择
/// @param [in] e 鼠标事件指针
void ActionSelectSingle::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton)
    {
        finish();
        if (actionSelect->getEntityType() == DM::ActionSelect)
        {
            actionSelect->finish();
        }
        else
        {
            actionSelect->mouseReleaseEvent(e);
        }
    }
    else
    {
        if (entityTypeList.size())
        {
            en = catchEntity(e, entityTypeList);
        }
        else
        {
            en = catchEntity(e);
        }
        trigger();
    }
}

/// @brief 更新鼠标光标样式为选择光标
void ActionSelectSingle::updateMouseCursor()
{
    docView->setMouseCursor(DM::SelectCursor);
}
