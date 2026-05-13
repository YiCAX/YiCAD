/*
 * Copyright (C) 2026 YiCAD Contributors
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

/// @file UILinePolygonOptions.cpp
/// @brief 多边形选项控件实现

#include "UILinePolygonOptions.h"

#include "ActionDrawLinePolygon.h"
#include "DmSettings.h"
#include "ui_UILinePolygonOptions.h"
#include "Debug.h"

/// @brief 构造多边形选项控件
/// @param parent 父窗口指针
/// @param fl 窗口标志
UILinePolygonOptions::UILinePolygonOptions(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, ui(new Ui::Ui_LinePolygonOptions{})
{
	ui->setupUi(this);
}

/// @brief 析构函数，保存设置后释放资源
UILinePolygonOptions::~UILinePolygonOptions()
{
	saveSettings();
}

/// @brief 根据当前语言刷新子控件字符串
void UILinePolygonOptions::languageChange()
{
	ui->retranslateUi(this);
}

void UILinePolygonOptions::saveSettings()
{
	DMSETTINGS->beginGroup("/Draw");
	DMSETTINGS->writeEntry("/LinePolygonNumber", ui->sbNumber->text());
	DMSETTINGS->endGroup();
}

void UILinePolygonOptions::setAction(ActionInterface* a, bool update)
{
	if (a && a->getEntityType() == DM::ActionDrawLinePolygonCenCor)
	{
		action = static_cast<ActionDrawLinePolygonCenCor*>(a);

		QString sn;
		if (update)
		{
			sn = QString("%1").arg(action->getNumber());
		}
		else
		{
			DMSETTINGS->beginGroup("/Draw");
			sn = DMSETTINGS->readEntry("/LinePolygonNumber", "3");
			DMSETTINGS->endGroup();
		}
		ui->sbNumber->setValue(sn.toInt());
	}
	else
	{
		action = nullptr;
	}
}

void UILinePolygonOptions::updateNumber(int n)
{
	if (action)
	{
		action->setNumber(n);
	}
}
