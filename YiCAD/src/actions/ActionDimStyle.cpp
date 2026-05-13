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

/// @file ActionDimStyle.cpp
/// @brief 标注样式管理操作类实现

#include "ActionDimStyle.h"
#include "ApplicationWindow.h"
#include "GuiDialogFactory.h"
#include "DmDocument.h"

ActionDimStyle::ActionDimStyle(DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Dimension Style", doc, docView)
{
}

void ActionDimStyle::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

void ActionDimStyle::trigger()
{
    DmDocument* pDocument =
                ApplicationWindow::getAppWindow()->getDocument();
    GUIDIALOGFACTORY->requestDimStyleMgrDialog(
                pDocument->getDimStyleTable(), pDocument);
    finish(false);
}

