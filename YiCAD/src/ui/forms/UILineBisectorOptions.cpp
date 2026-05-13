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

/// @file UILineBisectorOptions.cpp
/// @brief 角平分线选项控件实现

#include "UILineBisectorOptions.h"

#include "ActionDrawLineBisector.h"
#include "DmSettings.h"
#include "Math2d.h"
#include "Debug.h"
#include "ui_UILineBisectorOptions.h"

UILineBisectorOptions::UILineBisectorOptions(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, ui(new Ui::Ui_LineBisectorOptions{})
{
	ui->setupUi(this);
}

UILineBisectorOptions::~UILineBisectorOptions()
{
	saveSettings();
}

void UILineBisectorOptions::languageChange()
{
	ui->retranslateUi(this);
}

void UILineBisectorOptions::saveSettings()
{
	DMSETTINGS->beginGroup("/Draw");
	DMSETTINGS->writeEntry("/LineBisectorLength", ui->leLength->text());
	DMSETTINGS->writeEntry("/LineBisectorNumber", ui->sbNumber->text());
	DMSETTINGS->endGroup();
}

void UILineBisectorOptions::setAction(ActionInterface* a, bool update)
{
	if (a && a->getEntityType() == DM::ActionDrawLineBisector)
	{
		action = static_cast<ActionDrawLineBisector*>(a);

		QString sl;
		QString sn;
		if (update)
		{
			sl = QString("%1").arg(action->getLength());
			sn = QString("%1").arg(action->getNumber());
		}
		else
		{
			DMSETTINGS->beginGroup("/Draw");
			sl = DMSETTINGS->readEntry("/LineBisectorLength", "3000.0");
			sn = DMSETTINGS->readEntry("/LineBisectorNumber", "1");
			DMSETTINGS->endGroup();
		}
		ui->leLength->setText(sl);
		ui->sbNumber->setValue(sn.toInt());
	}
	else
	{
		action = nullptr;
	}
}

void UILineBisectorOptions::updateLength(const QString& l)
{
	if (action)
	{
		action->setLength(Math2d::eval(l));
	}
}

void UILineBisectorOptions::updateNumber(int n)
{
	if (action)
	{
		action->setNumber(n);
	}
}
