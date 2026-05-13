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

/// @file ActionFileSaveAs.cpp
/// @brief 文件另存为 Action 类的实现

#include "ActionFileSaveAs.h"
#include "ApplicationWindow.h"
#include "UITabDrawWidget.h"

#include <QAction>

#include "Debug.h"
#include "DmDocument.h"

/// @brief 默认文件保存格式类型
constexpr const char* YCD_FORMAT_TYPE = "ycd";

/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionFileSaveAs::ActionFileSaveAs(DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Save as", doc, docView)
{
}

/// @brief 触发文件另存为操作
void ActionFileSaveAs::trigger()
{
    DMSYSTEM->setCurrentFormatType(YCD_FORMAT_TYPE);
    ApplicationWindow::getAppWindow()->getTabDrawWidget()->slotFileSaveAs();
    finish(false);
}

/// @brief 判断是否为独占模式
/// @return 始终返回 true
bool ActionFileSaveAs::isExclusive()
{
    return true;
}

/// @brief 初始化并立即触发另存为
/// @param status 初始状态
void ActionFileSaveAs::init(int status)
{
    ActionInterface::init(status);
    trigger();
}
