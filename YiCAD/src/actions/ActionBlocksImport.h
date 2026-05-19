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

/// @file ActionBlocksImport.h
/// @brief 从外部文件导入块定义到当前文档的动作类头文件

#ifndef ACTIONBLOCKSIMPORT_H
#define ACTIONBLOCKSIMPORT_H

#include "ActionInterface.h"

class DmEntity;

/// @brief 从外部文件导入块定义到当前文档
class ActionBlocksImport : public ActionInterface
{
	Q_OBJECT
public:
	/// @brief 构造函数
	/// @param doc 文档指针
	/// @param docView 文档视图指针
	ActionBlocksImport(DmDocument* doc, GuiDocumentView* docView);

	/// @brief 初始化动作
	/// @param status 状态参数，默认为0
	void init(int status = 0) override;

	/// @brief 触发动作执行
	void trigger() override;

	/// @brief 递归重定向实体的图层、文字样式、标注样式到目标文档
	/// @param entity 待处理的实体
	void repointEntityStyles(DmEntity* entity);

	/// @brief 检查是否为独占动作
	/// @return false表示非独占
	bool isExclusive() override { return false; }

	/// @brief 检查是否可以中断
	/// @return true表示可以中断
	bool canBeInterrupt() override { return true; }
};

#endif
