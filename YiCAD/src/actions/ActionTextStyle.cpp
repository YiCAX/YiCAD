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

/// @file ActionTextStyle.cpp
/// @brief 文字样式设置Action实现

#include "ActionTextStyle.h"
#include "ApplicationWindow.h"
#include "GuiDialogFactory.h"
#include "DmDocument.h"

/// @brief 构造函数，初始化文字样式Action
/// @param [in] doc 文档指针
/// @param [in] docView 文档视图指针
ActionTextStyle::ActionTextStyle(DmDocument* doc, GuiDocumentView* docView)
    : ActionInterface("Text Style", doc, docView)
{
}

/// @brief 初始化操作状态，立即触发文字样式对话框
/// @param [in] status 初始状态值
void ActionTextStyle::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 打开文字样式对话框
void ActionTextStyle::trigger()
{
    DmDocument* pDocument = ApplicationWindow::getAppWindow()->getDocument();
    GUIDIALOGFACTORY->requestTextStyleDialog(
        pDocument->getTextStyleTable(), pDocument);
    finish(false);
}
