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

/// @file ActionLayersLock.cpp
/// @brief 图层锁定动作类实现文件

#include "ActionLayersLock.h"
#include "DmLayer.h"
#include "DmDocument.h"
#include "ApplicationWindow.h"
#include "CustomComboboxItem.h"
#include "GuiDocumentView.h"
#include "SARibbonComboBox.h"
#include "MDIWindow.h"
#include "Transaction.h"

/// @brief 构造函数
/// @param sender 触发动作的按钮对象
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionLayersLock::ActionLayersLock(QObject* sender, DmDocument* doc, GuiDocumentView* docView)
	: ActionInterface("Lock Layer", doc, docView)
	, layer(nullptr)
	, toLock(false)
	, theButton(sender)
{
}

/// @brief 初始化动作
/// @param status 状态参数，默认为0
void ActionLayersLock::init(int status)
{
	ActionInterface::init(status);
	trigger();
}

/// @brief 触发动作执行
void ActionLayersLock::trigger()
{
	ApplicationWindow* appWin = ApplicationWindow::getAppWindow();
	auto cbxItems = appWin->getLayerComboboxItems();

	// 按钮触发的action，获得按钮所在的图层，判断需要锁定还是解锁
	if (theButton && layer == nullptr)
	{
		// 当前图层按钮触发
		ComboBoxData* currentData = appWin->getCurrentLayerItem();
		if (currentData->btnLock == theButton)
		{
			setLayer(currentData->getLayerName());
			setToLock(!currentData->isLock);
		}
		// 图层下拉列表按钮触发
		else
		{
			for (auto& item : cbxItems)
			{
				auto data = item->getData();
				if (data->btnLock == theButton)
				{
					setLayer(data->getLayerName());
					setToLock(!data->isLock);
				}
			}
		}
	}

	if (layer)
	{
		Transaction t(tr("Lock Layer").toStdString(), pDocument);
		t.start();
		pDocument->getLayerTable()->startModify(layer);
		layer->lock(toLock);
		t.commit();

		// 取消选择已锁定图层实体
		auto table = pDocument->getEntityTable();
		for (auto it = table->begin(); it != table->end(); ++it)
		{
			if ((*it)->getLayer() == layer && (*it)->isSelected())
			{
				(*it)->setSelected(false);
			}
		}
	}
	finish();
}

/// @brief 设置要操作的图层
/// @param layerName 图层名称
void ActionLayersLock::setLayer(const QString& layerName)
{
	layer = pDocument->getLayerTable()->find(layerName);
}

/// @brief 设置锁定/解锁状态
/// @param lock true表示锁定，false表示解锁
void ActionLayersLock::setToLock(bool lock)
{
	toLock = lock;
}
