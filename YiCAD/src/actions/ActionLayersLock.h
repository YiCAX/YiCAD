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

/// @file ActionLayersLock.h
/// @brief 图层锁定动作类头文件

#ifndef ACTIONLAYERSLOCK_H
#define ACTIONLAYERSLOCK_H

#include "ActionInterface.h"

class DmLayer;

/// @brief 图层锁定/解锁动作类
class ActionLayersLock : public ActionInterface
{
	Q_OBJECT
public:
	/// @brief 构造函数
	/// @param sender 触发动作的按钮对象
	/// @param doc 文档指针
	/// @param docView 文档视图指针
	ActionLayersLock(QObject* sender, DmDocument* doc, GuiDocumentView* docView);

	/// @brief 初始化动作
	/// @param status 状态参数，默认为0
	void init(int status = 0) override;

	/// @brief 触发动作执行
	void trigger() override;

	/// @brief 设置要操作的图层
	/// @param layerName 图层名称
	void setLayer(const QString& layerName);

	/// @brief 设置锁定/解锁状态
	/// @param lock true表示锁定，false表示解锁
	void setToLock(bool lock);

protected:
	DmLayer* layer;       ///< 目标图层指针
	bool     toLock;      ///< 是否锁定
	QObject* theButton;   ///< 触发按钮指针
};

#endif // !ACTIONLAYERSLOCK_H
