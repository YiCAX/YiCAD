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

/// @file ActionBlockInsertPrepare.h
/// @brief 块插入准备动作类头文件

#ifndef ACTIONBLOCKINSERTPREPARE_H
#define ACTIONBLOCKINSERTPREPARE_H

#include "ActionInterface.h"

class QDialog;
class UIBlockListWidget;

/// @brief 块插入准备动作类
class ActionBlockInsertPrepare : public ActionInterface
{
	Q_OBJECT
public:
	/// @brief 构造函数
	/// @param doc 文档指针
	/// @param docView 文档视图指针
	ActionBlockInsertPrepare(DmDocument* doc, GuiDocumentView* docView);

	/// @brief 析构函数
	~ActionBlockInsertPrepare();

	/// @brief 初始化动作
	/// @param status 状态参数，默认为0
	void init(int status = 0) override;

	/// @brief 检查是否可以中断
	/// @return true表示可以中断，false表示不可以中断
	bool canBeInterrupt() override;

	/// @brief 检查是否为独占动作
	/// @return true表示独占，false表示非独占
	bool isExclusive() override;

public:
	/// @brief 鼠标移动事件处理
	/// @param e 鼠标事件指针
	void mouseMoveEvent(QMouseEvent* e) override;

	/// @brief 鼠标释放事件处理
	/// @param e 鼠标事件指针
	void mouseReleaseEvent(QMouseEvent* e) override;

private:
	QDialog* m_pBlockBack; ///< 块列表对话框指针
	UIBlockListWidget* m_pBlockWidget; ///< 块列表控件指针
};

#endif //!ACTIONBLOCKINSERTPREPARE_H