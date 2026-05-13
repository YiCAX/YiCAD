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

/// @file ActionBlockInsertPrepare.cpp
/// @brief 块插入准备动作类实现文件

#include <QDialog>
#include "ActionBlockInsertPrepare.h"
#include "UIBlockListWidget.h"
#include "ApplicationWindow.h"
#include "DmDocument.h"

/// @brief 对话框垂直居中位置系数
constexpr double DIALOG_CENTER_RATIO = 0.5;


/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionBlockInsertPrepare::ActionBlockInsertPrepare(DmDocument* doc,
	GuiDocumentView* docView)
	: ActionInterface("Block Insert Prepare", doc, docView)
	, m_pBlockBack(nullptr)
	, m_pBlockWidget(nullptr)
{
	actionType = DM::ActionBlockInsertPrepare;
}

/// @brief 析构函数
ActionBlockInsertPrepare::~ActionBlockInsertPrepare()
{
	if (m_pBlockBack)
	{
		m_pBlockBack->hide();
	}
}

/// @brief 初始化动作
/// @param status 状态参数，默认为0
void ActionBlockInsertPrepare::init(int status)
{
	ActionInterface::init(status);
	ApplicationWindow* appWin = ApplicationWindow::getAppWindow();
	m_pBlockBack = new QDialog(appWin);
	m_pBlockBack->setWindowTitle(QObject::tr("Block List"));
	m_pBlockBack->setMinimumWidth(300);
	m_pBlockBack->setMaximumWidth(300);
	m_pBlockBack->setMinimumHeight(600);
	m_pBlockBack->setMaximumHeight(600);

	// 计算对话框位置：右侧居中
	int xPos = appWin->width() - m_pBlockBack->width();
	int yPos = (appWin->height() - m_pBlockBack->height()) * DIALOG_CENTER_RATIO;
	m_pBlockBack->move(xPos, yPos);

	m_pBlockWidget = new UIBlockListWidget(
		appWin->getActionHandler(), m_pBlockBack, "Block");
	m_pBlockWidget->setBlockList(pDocument->getBlockTable());
	m_pBlockWidget->resize(m_pBlockBack->size());
	m_pBlockBack->show();
}

/// @brief 检查是否可以中断
/// @return false表示不可以中断
bool ActionBlockInsertPrepare::canBeInterrupt()
{
	return false;
}

/// @brief 检查是否为独占动作
/// @return true表示独占
bool ActionBlockInsertPrepare::isExclusive()
{
	return true;
}

/// @brief 鼠标移动事件处理
/// @param e 鼠标事件指针
void ActionBlockInsertPrepare::mouseMoveEvent(QMouseEvent* e)
{
	// 这样才能绘制鼠标，否则显示像卡顿的效果
	deleteSnapper();
}

/// @brief 鼠标释放事件处理
/// @param e 鼠标事件指针
void ActionBlockInsertPrepare::mouseReleaseEvent(QMouseEvent* e)
{
	finish();
}
