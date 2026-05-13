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

/// @file ActionModifyDelete.cpp
/// @brief 删除选中实体的交互动作类实现

#include "ActionModifyDelete.h"

#include <QAction>

#include "Debug.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "Modification.h"

/// @brief 构造函数
/// @param [in] doc 文档指针
/// @param [in] docView 文档视图指针
ActionModifyDelete::ActionModifyDelete(DmDocument* doc, GuiDocumentView* docView)
    : ActionInterface("Delete Entities", doc, docView)
{
    actionType = DM::ActionModifyDelete;
}

/// @brief 初始化动作，直接触发删除
/// @param [in] status 初始状态
void ActionModifyDelete::init(int status)
{
    ActionInterface::init(status);

    trigger();
}

/// @brief 触发删除操作
void ActionModifyDelete::trigger()
{
    Modification m(docView);
    m.remove();

    finish(false);

    GUIDIALOGFACTORY->updateSelectionWidget(pDocument->getEntityTable()->countSelect());
}

/// @brief 更新鼠标按键提示
void ActionModifyDelete::updateMouseButtonHints()
{
    GUIDIALOGFACTORY->updateMouseWidget();
}

/// @brief 更新鼠标光标为删除样式
void ActionModifyDelete::updateMouseCursor()
{
    docView->setMouseCursor(DM::DelCursor);
}
