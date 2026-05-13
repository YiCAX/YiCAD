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

/// @file ActionLayersPrint.h
/// @brief 图层打印动作类头文件

#ifndef ACTIONLAYERSPRINT_H
#define ACTIONLAYERSPRINT_H

#include "ActionInterface.h"

class DmLayer;

/// @brief 图层打印动作类
class ActionLayersPrint : public ActionInterface
{
	Q_OBJECT
public:
	/// @brief 构造函数
	/// @param sender 发送者对象指针
	/// @param doc 文档指针
	/// @param docView 文档视图指针
	ActionLayersPrint(QObject* sender, DmDocument* doc, GuiDocumentView* docView);

	/// @brief 初始化动作
	/// @param status 状态参数，默认为0
	void init(int status = 0) override;

	/// @brief 触发动作
	void trigger() override;

	/// @brief 设置目标图层
	/// @param layerName 图层名称
	void setLayer(const QString& layerName);

	/// @brief 设置是否打印
	/// @param print 是否打印
	void setToPrint(bool print);

protected:
	DmLayer* layer;		///< 目标图层指针
	bool toPrint;		///< 是否打印标志
	QObject* theButton;	///< 触发按钮对象指针
};

#endif //!ACTIONLAYERSPRINT_H
