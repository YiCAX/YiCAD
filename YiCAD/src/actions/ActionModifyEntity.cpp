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

/// @file ActionModifyEntity.cpp
/// @brief 修改实体属性的交互动作类实现

#include "ActionModifyEntity.h"

#include <QAction>
#include <QMouseEvent>

#include "ActionModifyMText.h"
#include "Debug.h"
#include "DmMText.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"

/// @brief 构造函数
/// @param [in] doc 文档指针
/// @param [in] docView 文档视图指针
ActionModifyEntity::ActionModifyEntity(DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Modify Entity", doc, docView),
    m_currentEntity(nullptr)
{
    actionType = DM::ActionModifyEntity;
}

/// @brief 触发修改操作
void ActionModifyEntity::trigger()
{
    if (m_currentEntity)
    {
        // 多行文字的编辑不是模态对话框，且与Action关联，需要特殊处理
        if (m_currentEntity->getEntityType() == DM::EntityMText)
        {
            ActionModifyMText* action = new ActionModifyMText(pDocument, docView);
            action->setText(static_cast<DmMText*>(m_currentEntity));
            docView->setCurrentAction(action);
            return;
        }

        // 不是多行文字类型处理
        GUIDIALOGFACTORY->requestModifyEntityDialog(m_currentEntity);
    }
}

/// @brief 鼠标释放事件处理
/// @param [in] e 鼠标事件指针
void ActionModifyEntity::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton)
    {
        init(getStatus() - 1);
    }
    else
    {
        m_currentEntity = catchEntity(e);
        if (m_currentEntity)
        {
            m_currentEntity->setSelected(true);
            docView->emitSelectedChanged();
        }
        trigger();
    }
}

/// @brief 更新鼠标光标为选择样式
void ActionModifyEntity::updateMouseCursor()
{
    docView->setMouseCursor(DM::SelectCursor);
}

/// @brief 更新鼠标按键提示
void ActionModifyEntity::updateMouseButtonHints()
{
    GUIDIALOGFACTORY->updateMouseWidget(tr("Click on entity to modify"), tr("Cancel"));
}
