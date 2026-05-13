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

/// @file ActionBlocksEdit.h
/// @brief 编辑块定义的动作类头文件

#ifndef ACTIONBLOCKSEDIT_H
#define ACTIONBLOCKSEDIT_H

#include "ActionInterface.h"
#include "DmBlockReference.h"

#include <memory>
#include <set>
#include <vector>

class TransactionGroup;
class ICmd;
class DmBlock;


/// @brief 编辑块定义的动作类
class ActionBlocksEdit : public ActionInterface
{
	Q_OBJECT
public:
	/// @brief 动作状态枚举
	enum EStatus
	{
		eEditing = 0 ///< 编辑中
	};

	/// @brief 构造函数
	/// @param doc 文档指针
	/// @param docView 文档视图指针
	/// @param selectedRef 选中的块参照，默认为nullptr
	ActionBlocksEdit(DmDocument* doc, GuiDocumentView* docView,
		DmBlockReference* selectedRef = nullptr);

	/// @brief 析构函数
	~ActionBlocksEdit() override;

	/// @brief 初始化动作
	/// @param status 状态参数，默认为0
	void init(int status = 0) override;

	/// @brief 触发动作执行
	void trigger() override;

	/// @brief 鼠标移动事件处理
	/// @param e 鼠标事件指针
	void mouseMoveEvent(QMouseEvent* e) override;

	/// @brief 鼠标按下事件处理
	/// @param e 鼠标事件指针
	void mousePressEvent(QMouseEvent* e) override;

	/// @brief 鼠标释放事件处理
	/// @param e 鼠标事件指针
	void mouseReleaseEvent(QMouseEvent* e) override;

	/// @brief 键盘按下事件处理
	/// @param e 键盘事件指针
	void keyPressEvent(QKeyEvent* e) override;

	/// @brief 检查是否可以中断
	/// @return true表示可以中断，false表示不可以中断
	bool canBeInterrupt() override;

	/// @brief 检查是否为独占动作
	/// @return true表示独占，false表示非独占
	bool isExclusive() override;

	/// @brief 检查是否为子动作
	/// @return true表示是子动作，false表示不是子动作
	bool isSubAction() override;

	/// @brief 显示选项栏
	void showOptions() override;

	/// @brief 隐藏选项栏
	void hideOptions() override;

	/// @brief 更新鼠标按钮提示
	void updateMouseButtonHints() override;

	/// @brief 更新鼠标光标
	void updateMouseCursor() override;

	/// @brief 完成编辑，将修改保存回块定义
	void completeEditing();

	/// @brief 取消编辑，丢弃所有修改
	void cancelEditing();

	/// @brief 获取正在编辑的块名称
	/// @return 块名称
	QString getBlockName() const { return m_blockName; }

private:
	/// @brief 进入编辑模式
	/// @param blockRef 块参照指针
	void enterEditing(DmBlockReference* blockRef);

	/// @brief 应用正向变换（块坐标 → 世界坐标）
	/// @param entity 实体指针
	void applyForwardTransform(DmEntity* entity);

	/// @brief 应用逆向变换（世界坐标 → 块坐标）
	/// @param entity 实体指针
	void applyInverseTransform(DmEntity* entity);

	/// @brief 收集文档中引用指定块名的所有 DmBlockReference
	/// @param blockName 块名称
	/// @param refs 块参照列表（输出）
	void collectBlockReferences(const QString& blockName,
		std::vector<DmBlockReference*>& refs);

private:
	DmBlockReference* m_blockRefBeingEdited; ///< 被选中的块参照
	DmBlockReferenceData m_blockRefData;     ///< 块参照的变换参数
	DmVector m_basePoint;                    ///< 块定义的基点

	std::set<DmId> m_originalEntityIds;      ///< 编辑开始时块中的实体ID集合
	QString m_blockName;                     ///< 编辑的块名称

	MacroCmd* m_blockTableMacroModifyCmd = nullptr; ///< 块表修改宏命令指针
	//ICmd* m_blockTableModifyCmd = nullptr;  // BlockTableModifyCmd 指针
};

#endif
