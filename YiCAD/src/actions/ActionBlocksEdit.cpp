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

/// @file ActionBlocksEdit.cpp
/// @brief 编辑块定义的动作类实现文件

#include "ActionBlocksEdit.h"

#include <cmath>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>

#include "DmBlock.h"
#include "DmDocument.h"
#include "DmBlockReference.h"
#include "DmBlockTable.h"
#include "DmEntityContainer.h"
#include "EntityTable.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "GuiEventHandler.h"
#include "Math2d.h"
#include "BlockTableCmd.h"
#include "CmdManager.h"
#include "MacroCmd.h"
#include "Transaction.h"
#include "UIDialogFactory.h"


/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
/// @param selectedRef 选中的块参照，默认为nullptr
ActionBlocksEdit::ActionBlocksEdit(DmDocument* doc,
	GuiDocumentView* docView, DmBlockReference* selectedRef)
	: ActionInterface("Edit Block", doc, docView)
	, m_blockRefBeingEdited(selectedRef)
{
	actionType = DM::ActionBlocksEdit;
}

/// @brief 析构函数
ActionBlocksEdit::~ActionBlocksEdit() = default;

/// @brief 初始化动作
/// @param status 状态参数，默认为0
void ActionBlocksEdit::init(int status)
{
	ActionInterface::init(status);

	if (!m_blockRefBeingEdited)
	{
		GUIDIALOGFACTORY->commandMessage(
			tr("No block reference selected. Command cancelled."));
		finish();
		return;
	}

	enterEditing(m_blockRefBeingEdited);
	setStatus(eEditing);
}

/// @brief 触发动作执行
void ActionBlocksEdit::trigger()
{
	if (m_blockRefBeingEdited)
	{
		enterEditing(m_blockRefBeingEdited);
		setStatus(eEditing);
	}
}

/// @brief 鼠标移动事件处理
/// @param e 鼠标事件指针
void ActionBlocksEdit::mouseMoveEvent(QMouseEvent* e)
{
	if (getStatus() == eEditing)
	{
		// 编辑模式下转发给默认Action（更新坐标、光标、捕捉）
		auto* handler = docView->getEventHandler();
		if (handler && handler->getDefaultAction())
		{
			handler->getDefaultAction()->mouseMoveEvent(e);
		}
	}
}

/// @brief 鼠标按下事件处理
/// @param e 鼠标事件指针
void ActionBlocksEdit::mousePressEvent(QMouseEvent* e)
{
	if (getStatus() == eEditing)
	{
		auto* handler = docView->getEventHandler();
		if (handler && handler->getDefaultAction())
		{
			handler->getDefaultAction()->mousePressEvent(e);
		}
	}
}

/// @brief 鼠标释放事件处理
/// @param e 鼠标事件指针
void ActionBlocksEdit::mouseReleaseEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		if (getStatus() == eEditing)
		{
			// 编辑模式下转发给默认Action（选择/框选）
			auto* handler = docView->getEventHandler();
			if (handler && handler->getDefaultAction())
			{
				handler->getDefaultAction()->mouseReleaseEvent(e);
			}
		}
	}
	else if (e->button() == Qt::RightButton)
	{
		if (getStatus() == eEditing)
		{
			int ret = QMessageBox::question(nullptr,
				tr("Block Edit"),
				tr("Finish editing and save changes?"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

			if (ret == QMessageBox::Yes)
			{
				completeEditing();
			}
			else if (ret == QMessageBox::No)
			{
				cancelEditing();
			}
			// Cancel: continue editing
		}
		else
		{
			finish();
		}
	}
}

/// @brief 键盘按下事件处理
/// @param e 键盘事件指针
void ActionBlocksEdit::keyPressEvent(QKeyEvent* e)
{
	if (getStatus() == eEditing)
	{
		auto* handler = docView->getEventHandler();
		if (handler && handler->getDefaultAction())
		{
			handler->getDefaultAction()->keyPressEvent(e);
		}
	}
	else
	{
		ActionInterface::keyPressEvent(e);
	}
}

/// @brief 检查是否可以中断
/// @return true表示可以中断
bool ActionBlocksEdit::canBeInterrupt()
{
	return true;
}

/// @brief 检查是否为独占动作
/// @return false表示非独占
bool ActionBlocksEdit::isExclusive()
{
	return false;
}

/// @brief 检查是否为子动作
/// @return false表示不是子动作
bool ActionBlocksEdit::isSubAction()
{
	return false;
}

/// @brief 显示选项栏
void ActionBlocksEdit::showOptions()
{
	ActionInterface::showOptions();
	GUIDIALOGFACTORY->requestOptions(this, true);
}

/// @brief 隐藏选项栏
void ActionBlocksEdit::hideOptions()
{
	ActionInterface::hideOptions();
	GUIDIALOGFACTORY->requestOptions(this, false);
}

/// @brief 更新鼠标按钮提示
void ActionBlocksEdit::updateMouseButtonHints()
{
	switch (getStatus())
	{
	case eEditing:
		GUIDIALOGFACTORY->updateMouseWidget(
			tr("Edit block entities"), tr("Finish / Cancel"));
		break;
	default:
		GUIDIALOGFACTORY->updateMouseWidget();
		break;
	}
}

/// @brief 更新鼠标光标
void ActionBlocksEdit::updateMouseCursor()
{
	if (getStatus() == eEditing)
	{
		docView->setMouseCursor(DM::ArrowCursor);
	}
	else
	{
		docView->setMouseCursor(DM::CadCursor);
	}
}

/// @brief 进入编辑模式
/// @param blockRef 块参照指针
void ActionBlocksEdit::enterEditing(DmBlockReference* blockRef)
{
	if (!blockRef || !pDocument)
	{
		return;
	}

	m_blockRefBeingEdited = blockRef;
	m_blockRefData = blockRef->getData();
	m_blockName = blockRef->getName();

	DmBlockTable* blockTable = pDocument->getBlockTable();
	if (!blockTable)
	{
		return;
	}

	DmBlock* block = blockTable->find(m_blockName);
	if (!block)
	{
		GUIDIALOGFACTORY->commandMessage(
			tr("Block definition not found: %1").arg(m_blockName));
		return;
	}

	// 1. 设置编辑块（getEntityTable() 将返回块的实体表）
	pDocument->setEditBlock(block);

	// 2. 重绑定文档画笔到块的实体容器
	docView->setDocumentPainterContainer(
		block->getEntityTable().getEntityContainer());

	// 3. 启动事务，调用 startModify并提交，保存原始块信息
	Transaction t("Block Edit", pDocument);
	t.start();
	blockTable->startModify(block);
	t.commit();

	// 4. 保存 BlockTableModifyCmd 指针
	CmdManager* cmdMgr = pDocument->getCmdManager();
	MacroCmd* groupCmd = cmdMgr->getLastUndoCmd();
	m_blockTableMacroModifyCmd = dynamic_cast<MacroCmd*>(groupCmd);

	// 5. 记录块中原始实体ID
	m_basePoint = block->getBasePoint();
	m_originalEntityIds.clear();
	for (auto e : block->getEntityTable())
	{
		if (e && !e->isErased())
		{
			m_originalEntityIds.insert(e->getId());
		}
	}

	// 6. 正向变换块实体到世界坐标
	for (auto e : block->getEntityTable())
	{
		if (e && !e->isErased())
		{
			applyForwardTransform(e);
		}
	}

	// 7. 刷新渲染
	pDocument->regenerate();

	// 8. 显示选项栏
	showOptions();

	GUIDIALOGFACTORY->commandMessage(tr("Editing block: %1").arg(m_blockName));
}

/// @brief 应用正向变换（块坐标 → 世界坐标）
/// @param entity 实体指针
void ActionBlocksEdit::applyForwardTransform(DmEntity* entity)
{
	// 正向变换（块坐标 → 世界坐标），同 DmBlockReference::update()
	// 1. move(insertionPoint - basePoint)
	// 2. scale(insertionPoint, scaleFactor)
	// 3. rotateAngle(insertionPoint, angle)

	DmVector insertionPoint = m_blockRefData.insertionPoint;
	DmVector scaleFactor = m_blockRefData.scaleFactor;
	double angle = m_blockRefData.angle;

	entity->move(insertionPoint + m_basePoint * -1.0);
	entity->scale(insertionPoint, scaleFactor);
	entity->rotateAngle(insertionPoint, angle);
}

/// @brief 应用逆向变换（世界坐标 → 块坐标）
/// @param entity 实体指针
void ActionBlocksEdit::applyInverseTransform(DmEntity* entity)
{
	// 逆向变换（世界坐标 → 块坐标）
	// 1. rotateAngle(insertionPoint, -angle)
	// 2. scale(insertionPoint, 1/scaleFactor)
	// 3. move(basePoint - insertionPoint)

	DmVector insertionPoint = m_blockRefData.insertionPoint;
	DmVector scaleFactor = m_blockRefData.scaleFactor;
	double angle = m_blockRefData.angle;

	// 缩放为零保护
	const double EPSILON = 1.0e-6;
	if (std::fabs(scaleFactor.x) < EPSILON || std::fabs(scaleFactor.y) < EPSILON)
	{
		return;
	}

	DmVector invScale(1.0 / scaleFactor.x, 1.0 / scaleFactor.y);

	entity->rotateAngle(insertionPoint, -angle);
	entity->scale(insertionPoint, invScale);
	entity->move(m_basePoint + insertionPoint * -1.0);
}

/// @brief 完成编辑，将修改保存回块定义
void ActionBlocksEdit::completeEditing()
{
	DmBlock* block = pDocument->getEditingBlock();
	if (!pDocument || !block || !m_blockTableMacroModifyCmd)
	{
		cancelEditing();
		return;
	}

	// 逆向变换所有实体回块坐标
	for (auto e : block->getEntityTable())
	{
		if (e && !e->isErased())
		{
			applyInverseTransform(e);
		}
	}

	// 回滚绘图命令，仅保留 BlockTableModifyCmd
	auto cmdMgr = pDocument->getCmdManager();
	int index = cmdMgr->indexOfCmd(m_blockTableMacroModifyCmd);
	if (m_blockTableMacroModifyCmd && index != -1)
	{
		// 1. 设置块的新数据
		std::ostringstream oss;
		OutputStream str(oss);
		block->saveStream(str);
		static_cast<BlockTableModifyCmd*>(*(m_blockTableMacroModifyCmd->begin()))
			->setNewData(oss.str());
		static_cast<BlockTableModifyCmd*>(*(m_blockTableMacroModifyCmd->begin()))
			->setExecuted(false);

		// 2. 回滚并删除之后的所有Cmd（绘图命令）。这会导致创建的实体被删除
		cmdMgr->rollbackAndRemoveAfter(index + 1);
	}

	// 通过redo重新创建实体
	m_blockTableMacroModifyCmd->redo();

	// 退出编辑模式（先退出，使 getEntityTable() 返回文档实体表）
	pDocument->setEditBlock(nullptr);

	// 重绑定文档画笔到文档实体容器
	docView->setDocumentPainterContainer(
		pDocument->getEntityTable()->getEntityContainer());

	// 对文档中所有引用该块的 DmBlockReference 调用 update()
	std::vector<DmBlockReference*> refs;
	collectBlockReferences(m_blockName, refs);
	for (auto ref : refs)
	{
		ref->update();
	}

	m_blockTableMacroModifyCmd = nullptr;

	hideOptions();
	finish();
}

/// @brief 取消编辑，丢弃所有修改
void ActionBlocksEdit::cancelEditing()
{
	DmBlock* block = pDocument->getEditingBlock();
	if (!pDocument || !block)
	{
		auto cmdMgr = pDocument->getCmdManager();
		int index = cmdMgr->indexOfCmd(m_blockTableMacroModifyCmd);
		if (m_blockTableMacroModifyCmd && index != -1)
		{
			// 回滚并删除之后的所有Cmd（绘图命令）。这会导致创建的实体被删除
			cmdMgr->rollbackAndRemoveAfter(index);
		}
		m_blockTableMacroModifyCmd = nullptr;
		finish();
		return;
	}

	// 逆向变换原始实体回块坐标
	for (auto e : block->getEntityTable())
	{
		if (e && !e->isErased())
		{
			applyInverseTransform(e);
		}
	}

	m_blockTableMacroModifyCmd = nullptr;

	// 退出编辑模式
	pDocument->setEditBlock(nullptr);

	// 重绑定文档画笔到文档实体容器
	docView->setDocumentPainterContainer(
		pDocument->getEntityTable()->getEntityContainer());

	pDocument->regenerate();
	hideOptions();
	finish();
}

/// @brief 收集文档中引用指定块名的所有 DmBlockReference
/// @param blockName 块名称
/// @param refs 块参照列表（输出）
void ActionBlocksEdit::collectBlockReferences(const QString& blockName,
	std::vector<DmBlockReference*>& refs)
{
	if (!pDocument)
	{
		return;
	}

	for (auto e : *pDocument->getEntityTable())
	{
		if (e && !e->isErased()
			&& e->getEntityType() == DM::EntityBlockReference)
		{
			DmBlockReference* ref = static_cast<DmBlockReference*>(e);
			if (ref->getName() == blockName)
			{
				refs.push_back(ref);
			}
		}
	}
}

// EOF