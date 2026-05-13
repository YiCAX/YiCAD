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

/// @file ActionDimDiametric.h
/// @brief 直径标注操作类头文件

#ifndef ACTIONDIMDIAMETRIC_H
#define ACTIONDIMDIAMETRIC_H

#include "ActionDimension.h"

struct DmDimDiametricData;

/// @brief 直径标注操作类，处理用户绘制直径标注的交互事件
class ActionDimDiametric : public ActionDimension
{
	Q_OBJECT

private:
	/// @brief 操作状态枚举
	enum Status
	{
		SetEntity, ///< 选择实体（圆/圆弧）
		SetPos,    ///< 选择标注线位置
		SetText    ///< 在命令行中设置文本标签
	};

public:
	/// @brief 构造函数
	/// @param [in] doc 文档对象
	/// @param [in] docView 文档视图对象
	ActionDimDiametric(DmDocument* doc, GuiDocumentView* docView);

	/// @brief 析构函数
	~ActionDimDiametric() override;

	/// @brief 重置操作状态
	void reset() override;

	/// @brief 触发创建直径标注
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

private:
	DmEntity* entity;                           ///< 选择的实体（圆/圆弧）
	std::unique_ptr<DmVector> pos;              ///< 拾取圆后鼠标移动时的位置
	std::unique_ptr<DmDimDiametricData> edata;  ///< 直径标注数据
	Status lastStatus;                          ///< 进入文本输入前的上一状态
};

#endif // ACTIONDIMDIAMETRIC_H
