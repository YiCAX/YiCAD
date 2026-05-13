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

/// @file ActionEditUndo.cpp
/// @brief 撤销/重做 Action 类的实现

#include "ActionEditUndo.h"

#include <QAction>

#include "DmDocument.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"

/// @brief 构造函数
/// @param undo true 为撤销，false 为重做
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionEditUndo::ActionEditUndo(bool undo, DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Edit Undo", doc, docView), undo(undo)
{
}

/// @brief 初始化并立即执行
/// @param status 初始状态
void ActionEditUndo::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 触发撤销或重做操作
void ActionEditUndo::trigger()
{
    if (!pDocument)
    {
        qWarning("undo: pDocument is null");
        return;
    }

    if (undo)
    {
        pDocument->undo();
    }
    else
    {
        pDocument->redo();
    }

    //docView->redraw();
    finish(false);
    //GUIDIALOGFACTORY->updateSelectionWidget(pDocument->getEntityContainer()->countSelected(), pDocument->getEntityContainer()->totalSelectedLength());
}
