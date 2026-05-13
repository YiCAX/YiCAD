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

/// @file ActionLayersLockAll.cpp
/// @brief 图层全部锁定/解锁 Action 类的实现

#include "ActionLayersLockAll.h"

#include <QAction>

#include "Debug.h"
#include "DmDocument.h"
#include "ApplicationWindow.h"
#include "CustomComboboxItem.h"
#include "GuiDocumentView.h"
#include "Transaction.h"

/// @brief 构造函数
/// @param lock true 表示锁定所有图层，false 表示解锁所有图层
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionLayersLockAll::ActionLayersLockAll(const bool lock, DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Lock all Layers", doc, docView)
    , lock(lock)
{
}

/// @brief 触发全部锁定/解锁图层操作
void ActionLayersLockAll::trigger()
{
    if (pDocument)
    {
        // 如果是锁定操作，先取消选择所有实体
        if (lock)
        {
            auto table = pDocument->getEntityTable();
            for (auto it = table->begin(); it != table->end(); ++it)
            {
                (*it)->setSelected(false);
            }
        }

        Transaction t(tr("Lock All Layers").toStdString(), pDocument);
        t.start();
        auto layerTable = pDocument->getLayerTable();
        for (auto it = layerTable->begin(); it != layerTable->end(); ++it)
        {
            layerTable->startModify(*it);
            (*it)->lock(lock);
        }
        t.commit();
    }
    finish(false);
}

/// @brief 初始化并立即触发
/// @param status 初始状态
void ActionLayersLockAll::init(int status)
{
    ActionInterface::init(status);
    trigger();
}
