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

/// @file ActionFileExportImage.cpp
/// @brief 图像导出 Action 类的实现

#include "ActionFileExportImage.h"
#include "ApplicationWindow.h"
#include "UITabDrawWidget.h"

/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionFileExportImage::ActionFileExportImage(DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Image Export", doc, docView)
{
}

/// @brief 判断是否为独占模式
/// @return 始终返回 true
bool ActionFileExportImage::isExclusive()
{
    return true;
}

/// @brief 初始化并立即触发导出
/// @param status 初始状态
void ActionFileExportImage::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 触发图像导出操作
void ActionFileExportImage::trigger()
{
    ApplicationWindow::getAppWindow()->getTabDrawWidget()->slotFileExportImage();
    finish(false);
}
