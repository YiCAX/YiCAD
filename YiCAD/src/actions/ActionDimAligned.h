/*
 * Copyright (C) 2026 YiCAD
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

/// @file ActionDimAligned.h
/// @brief 对齐标注操作类头文件

#ifndef ACTIONDIMALIGNED_H
#define ACTIONDIMALIGNED_H

#include "ActionDimension.h"

struct DmDimAlignedData;

/// @brief 对齐标注操作类，处理用户绘制对齐标注的交互事件
class ActionDimAligned : public ActionDimension
{
	Q_OBJECT

public:
	/// @brief 操作状态枚举
	enum Status
	{
		SetExtPoint1, ///< 设置第一条延伸线起点
		SetExtPoint2, ///< 设置第二条延伸线起点
		SetDefPoint,  ///< 设置标注线位置
		SetText       ///< 在命令行中设置文本标签
	};

public:
	/// @brief 构造函数
	/// @param [in] doc 文档对象
	/// @param [in] docView 文档视图对象
	ActionDimAligned(DmDocument* doc, GuiDocumentView* docView);

	/// @brief 析构函数
	~ActionDimAligned() override;

	/// @brief 重置操作状态
	void reset();

	/// @brief 触发创建对齐标注
	void trigger() override;

	/// @brief 准备预览图形
	void preparePreview();

	/// @brief 鼠标移动事件处理
	/// @param [in] e 鼠标事件
	void mouseMoveEvent(QMouseEvent* e) override;

	/// @brief 鼠标释放事件处理
	/// @param [in] e 鼠标事件
	void mouseReleaseEvent(QMouseEvent* e) override;

	/// @brief 坐标事件处理
	/// @param [in] e 坐标事件
	void coordinateEvent(GuiCoordinateEvent* e) override;

	/// @brief 命令事件处理
	/// @param [in] e 命令事件
	void commandEvent(GuiCommandEvent* e) override;

	/// @brief 获取可用命令列表
	/// @return 命令字符串列表
	QStringList getAvailableCommands() override;

	/// @brief 隐藏选项面板
	void hideOptions() override;

	/// @brief 显示选项面板
	void showOptions() override;

	/// @brief 更新鼠标按钮提示
	void updateMouseButtonHints() override;

protected:
	std::unique_ptr<DmDimAlignedData> edata; ///< 对齐标注数据
	Status lastStatus;                       ///< 进入文本输入前的上一状态
};

#endif // ACTIONDIMALIGNED_H
