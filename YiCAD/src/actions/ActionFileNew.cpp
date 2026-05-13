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

/// @file ActionFileNew.cpp
/// @brief 新建文件 Action 类的实现

#include "ActionFileNew.h"
#include "ApplicationWindow.h"
#include "UITabDrawWidget.h"
#include <QAction>

#include "Debug.h"

/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionFileNew::ActionFileNew(DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("File New", doc, docView)
    , m_strFileName("")
{
}

/// @brief 触发新建文件操作
void ActionFileNew::trigger()
{
    ApplicationWindow::getAppWindow()->getTabDrawWidget()->slotFileNew(m_strFileName);
    if (docView)
    {
        finish(false);
    }
    else
    {
        // docView 为空时不完成操作
    }
}

/// @brief 获取文件名
/// @return 文件全路径
QString ActionFileNew::getFileName() const
{
    return m_strFileName;
}

/// @brief 设置文件名
/// @param fileName 文件全路径
void ActionFileNew::setFileName(const QString& fileName)
{
    m_strFileName = fileName;
}

/// @brief 判断是否为独占模式
/// @return 始终返回 true
bool ActionFileNew::isExclusive()
{
    return true;
}

/// @brief 初始化并立即触发新建
/// @param status 初始状态
void ActionFileNew::init(int status)
{
    ActionInterface::init(status);
    trigger();
}
