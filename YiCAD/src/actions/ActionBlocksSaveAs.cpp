/*
 * Copyright (C) 2024 YiCAD Team
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

/// @file ActionBlocksSaveAs.cpp
/// @brief 块另存为动作类实现文件

#include "ActionBlocksSaveAs.h"

#include <QAction>

#include "ApplicationWindow.h"
#include "Debug.h"
#include "DmDocument.h"
#include "UIActionHandler.h"
#include "UIBlockSaveAs.h"

/// @brief 构造函数
/// @param[in] doc 文档指针
/// @param[in] docView 文档视图指针
ActionBlocksSaveAs::ActionBlocksSaveAs(
    DmDocument* doc, GuiDocumentView* docView)
    : ActionInterface("Block Save As", doc, docView)
    , m_pBlockSaveAs(nullptr)
{
}

/// @brief 析构函数
ActionBlocksSaveAs::~ActionBlocksSaveAs() = default;

/// @brief 触发动作执行，显示块另存为对话框
void ActionBlocksSaveAs::trigger()
{
    ApplicationWindow* appWin = ApplicationWindow::getAppWindow();
    UIActionHandler* handler = appWin->getActionHandler();
    if (!m_pBlockSaveAs)
    {
        m_pBlockSaveAs =
            new UIBlockSaveAs(handler, appWin, "Block SaveAs");
    }
    m_pBlockSaveAs->setBlockList(pDocument->getBlockTable());
    m_pBlockSaveAs->show();
    finish(false);
}

/// @brief 判断是否为独占动作
/// @return true 表示独占
bool ActionBlocksSaveAs::isExclusive()
{
    return true;
}

/// @brief 初始化动作
/// @param[in] status 初始状态
void ActionBlocksSaveAs::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

// EOF
