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

/// @file ActionInfoTotalLength.cpp
/// @brief 总长度测量 Action 类的实现

#include <QAction>
#include "ActionInfoTotalLength.h"

#include "Debug.h"
#include "DmDocument.h"
#include "GuiDialogFactory.h"

/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionInfoTotalLength::ActionInfoTotalLength(DmDocument* doc, GuiDocumentView* docView)
    : ActionInterface("Info Total Length", doc, docView)
{
    actionType = DM::ActionInfoTotalLength;
}

/// @brief 初始化并立即触发
/// @param status 初始状态
void ActionInfoTotalLength::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 触发总长度计算和显示
void ActionInfoTotalLength::trigger()
{
    double totalLength = 0.0;
    auto table = pDocument->getEntityTable();
    for (auto it = table->begin(); it != table->end(); ++it)
    {
        if ((*it)->isSelected())
        {
            totalLength += (*it)->getLength();
        }
    }
    if (totalLength > 0.0)
    {
        QString len = DmUnits::formatLinear(totalLength, pDocument->getUnit(), pDocument->getLinearFormat(), pDocument->getLinearPrecision());
        GUIDIALOGFACTORY->commandMessage(tr("Total Length of selected entities: %1").arg(len));
    }
    else
    {
        GUIDIALOGFACTORY->commandMessage(tr("At least one of the selected entities cannot be measured."));
    }

    finish(false);
}
