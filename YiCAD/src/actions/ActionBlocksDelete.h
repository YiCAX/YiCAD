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

/// @file ActionBlocksDelete.h
/// @brief 删除块定义的动作类头文件

#ifndef ACTIONBLOCKSDELETE_H
#define ACTIONBLOCKSDELETE_H

#include "ActionInterface.h"

class UIBlockDelete;

/// @brief 删除块定义的动作类
class ActionBlocksDelete : public ActionInterface
{
	Q_OBJECT
public:
	/// @brief 构造函数
	/// @param doc 文档指针
	/// @param docView 文档视图指针
	ActionBlocksDelete(DmDocument* doc, GuiDocumentView* docView);

	/// @brief 初始化动作
	/// @param status 状态参数，默认为0
	void init(int status = 0) override;

	/// @brief 触发动作执行
	void trigger() override;

private:
	UIBlockDelete* m_pDialog; ///< 块删除对话框指针
};

#endif //!ACTIONBLOCKSDELETE_H
