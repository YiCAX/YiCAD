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

/// @file ActionBlocksDelete.cpp
/// @brief 删除块定义的动作类实现文件

#include "ActionBlocksDelete.h"
#include "DmDocument.h"
#include "DmBlockTable.h"
#include "UIBlockDelete.h"
#include "ApplicationWindow.h"


/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionBlocksDelete::ActionBlocksDelete(DmDocument* doc,
	GuiDocumentView* docView)
	: ActionInterface("Delete Block", doc, docView)
	, m_pDialog(nullptr)
{
}

/// @brief 初始化动作
/// @param status 状态参数，默认为0
void ActionBlocksDelete::init(int status)
{
	ActionInterface::init(status);
	trigger();
}

/// @brief 触发动作执行
void ActionBlocksDelete::trigger()
{
	if (!m_pDialog)
	{
		m_pDialog = new UIBlockDelete(ApplicationWindow::getAppWindow());
	}
	m_pDialog->setBlockTable(pDocument->getBlockTable());
	m_pDialog->setDocument(pDocument);
	m_pDialog->exec();

	finish();
}
