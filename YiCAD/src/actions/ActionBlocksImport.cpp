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

/// @file ActionBlocksImport.cpp
/// @brief 从外部文件导入块定义到当前文档的动作类实现文件

#include "ActionBlocksImport.h"

#include <QApplication>
#include <QMessageBox>

#include "ApplicationWindow.h"
#include "DmBlock.h"
#include "DmBlockTable.h"
#include "DmDocument.h"
#include "DmEntityContainer.h"
#include "GuiDialogFactory.h"
#include "MDIWindow.h"
#include "Transaction.h"
#include "UIFileDialog.h"


/// @brief 构造函数
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionBlocksImport::ActionBlocksImport(DmDocument* doc,
	GuiDocumentView* docView)
	: ActionInterface("Block Import", doc, docView)
{
}

/// @brief 初始化动作
/// @param status 状态参数，默认为0
void ActionBlocksImport::init(int status)
{
	ActionInterface::init(status);
	trigger();
}

/// @brief 触发动作执行
void ActionBlocksImport::trigger()
{
	ApplicationWindow* appWindow = ApplicationWindow::getAppWindow();
	if (!appWindow)
	{
		finish(false);
		return;
	}

	// 1. 打开文件对话框
	UIFileDialog dlg(appWindow->getMDIWindow(), Qt::WindowFlags(),
		UIFileDialog::BlockFile);
	QString formatType;
	QString const& fn = dlg.getOpenFile();
	if (fn.isEmpty())
	{
		finish(false);
		return;
	}

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	// 2. 创建临时文档并加载文件（复用已有 filter 系统）
	DmDocument tempDoc;
	bool ok = tempDoc.open(fn);
	if (!ok)
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::warning(appWindow, tr("Import Block"),
			tr("Failed to open file: %1").arg(fn));
		finish(false);
		return;
	}

	DmBlockTable* srcTable = tempDoc.getBlockTable();
	DmBlockTable* dstTable = pDocument->getBlockTable();

	if (srcTable->count() == 0)
	{
		QApplication::restoreOverrideCursor();
		QMessageBox::information(appWindow, tr("Import Block"),
			tr("No blocks found in file."));
		finish(false);
		return;
	}

	// 3. Transaction 包裹，逐个克隆块定义添加到当前文档
	Transaction t("Import Block", pDocument);
	t.start();

	for (auto it = srcTable->begin(); it != srcTable->end(); ++it)
	{
		DmBlock* src = *it;
		QString name = src->getName();

		// 重名处理：不导入
		if (dstTable->find(name))
		{
			continue;
		}

		// 创建新块定义
		DmBlockData newData;
		newData.name = name;
		newData.basePoint = src->getBasePoint();
		newData.frozen = src->isFrozen();
		newData.pathName = fn;
		DmBlock* newBlock = new DmBlock(pDocument, newData);

		// 克隆块内所有实体
		auto& srcContainer = src->getEntityTable();
		auto& dstContainer = newBlock->getEntityTable();
		for (auto entity : srcContainer)
		{
			DmEntity* clone = entity->clone();
			clone->setDocument(pDocument);
			dstContainer.add_direct(clone);
		}

		// 通过命令系统添加（支持 undo/redo）
		dstTable->add(newBlock);
	}

	t.commit();

	QApplication::restoreOverrideCursor();

	pDocument->regenerate();
	finish(false);
}
