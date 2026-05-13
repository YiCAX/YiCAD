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

/// @file ActionFileOpen.cpp
/// @brief 文件打开 Action 类的实现

#include "ActionFileOpen.h"
#include "ApplicationWindow.h"
#include "UITabDrawWidget.h"
#include "UIActionHandler.h"
#include "UIBottomWidget.h"
#include <QAction>

#include "Debug.h"

/// @brief 默认文件格式类型
constexpr const char* DEFAULT_FORMAT_TYPE = "ycd";

/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionFileOpen::ActionFileOpen(DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("File Open", doc, docView)
{
}

/// @brief 触发文件打开操作
void ActionFileOpen::trigger()
{
    DMSYSTEM->setCurrentFormatType(DEFAULT_FORMAT_TYPE);
    ApplicationWindow::getAppWindow()->getTabDrawWidget()->slotFileOpen();
    UIBottomWindow* bottomWidget = ApplicationWindow::getAppWindow()->getBottomWidget();
    ApplicationWindow::getAppWindow()->getActionHandler()->slotSetSnaps(bottomWidget->getSnapToolBar()->getSnaps());
    finish(false);
}

/// @brief 判断是否为独占模式
/// @return 始终返回 true
bool ActionFileOpen::isExclusive()
{
    return true;
}

/// @brief 初始化并立即触发打开
/// @param status 初始状态
void ActionFileOpen::init(int status)
{
    ActionInterface::init(status);
    trigger();
}
