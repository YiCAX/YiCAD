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

/// @file ActionLayersRename.cpp
/// @brief 图层重命名 Action 类的实现

#include "ActionLayersRename.h"

#include "DmDocument.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "ApplicationWindow.h"
#include "Transaction.h"

/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionLayersRename::ActionLayersRename(DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Rename Layer", doc, docView)
{
}

/// @brief 初始化并立即触发
/// @param status 初始状态
void ActionLayersRename::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 触发图层重命名操作，通过编辑对话框修改图层数据
void ActionLayersRename::trigger()
{
    if (pDocument)
    {
        // 通过编辑对话框获取用户修改后的图层数据
        DmLayer* layer = GUIDIALOGFACTORY->requestEditLayerDialog(pDocument->getLayerTable());
        if (layer)
        {
            Transaction t(tr("Rename Layer").toStdString(), pDocument);
            t.start();
            auto activeLayer = pDocument->getLayerTable()->getActive();
            pDocument->getLayerTable()->startModify(activeLayer);
            activeLayer->setData(layer->getData());
            t.commit();
            delete layer;
        }
        else
        {
            // 用户取消编辑对话框，无需执行任何操作
        }
    }
    else
    {
        // 文档指针为空，无法执行操作
    }
    finish(false);
}
