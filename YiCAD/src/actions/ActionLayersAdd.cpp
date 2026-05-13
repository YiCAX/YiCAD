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

/// @file ActionLayersAdd.cpp
/// @brief 图层添加 Action 类的实现

#include "ActionLayersAdd.h"

#include <QAction>

#include "Debug.h"
#include "DmDocument.h"
#include "GuiDialogFactory.h"
#include "ApplicationWindow.h"
#include "Transaction.h"

/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionLayersAdd::ActionLayersAdd(DmDocument* doc, GuiDocumentView* docView)
    : ActionInterface("Add Layer", doc, docView)
{
}

/// @brief 触发图层添加操作
void ActionLayersAdd::trigger()
{
    if (pDocument)
    {
        DmLayer* layer = GUIDIALOGFACTORY->requestNewLayerDialog(pDocument->getLayerTable());
        if (layer)
        {
            Transaction t("Add Layer", pDocument);
            t.start();
            pDocument->getLayerTable()->add(layer);
            t.commit();
        }
    }
    finish(false);
}

/// @brief 初始化并立即触发
/// @param status 初始状态
void ActionLayersAdd::init(int status)
{
    ActionInterface::init(status);
    trigger();
}
