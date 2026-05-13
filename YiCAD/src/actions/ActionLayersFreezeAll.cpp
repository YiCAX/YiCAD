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

/// @file ActionLayersFreezeAll.cpp
/// @brief 冻结/解冻所有图层交互动作的实现

#include "ActionLayersFreezeAll.h"

#include <QAction>

#include "ApplicationWindow.h"
#include "CustomComboboxItem.h"
#include "Debug.h"
#include "DmDocument.h"
#include "GuiDocumentView.h"
#include "Transaction.h"

ActionLayersFreezeAll::ActionLayersFreezeAll(const bool freeze, DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Freeze all Layers", doc, docView)
    , freeze(freeze)
{
}

void ActionLayersFreezeAll::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

void ActionLayersFreezeAll::trigger()
{
    if (pDocument)
    {
        Transaction t(tr("Freeze Layer").toStdString(), pDocument);
        t.start();
        auto table = pDocument->getLayerTable();
        for (auto it = table->begin(); it != table->end(); ++it)
        {
            table->startModify(*it);
            (*it)->freeze(freeze);
        }
        t.commit();
    }
    finish(false);
}

// EOF
