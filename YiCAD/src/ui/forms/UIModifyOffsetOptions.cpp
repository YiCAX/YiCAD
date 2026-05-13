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

/// @file UIModifyOffsetOptions.cpp
/// @brief 偏移修改选项控件实现

#include "UIModifyOffsetOptions.h"

#include "DmSettings.h"
#include "ui_UIModifyOffsetOptions.h"
#include "Math2d.h"

UIModifyOffsetOptions::UIModifyOffsetOptions(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, ui(new Ui::Ui_ModifyOffsetOptions{})
{
	ui->setupUi(this);
}

UIModifyOffsetOptions::~UIModifyOffsetOptions()
{
	saveSettings();
}

void UIModifyOffsetOptions::languageChange()
{
	ui->retranslateUi(this);
}

void UIModifyOffsetOptions::saveSettings()
{
	DMSETTINGS->beginGroup("/Draw");
	DMSETTINGS->writeEntry("/ModifyOffsetDistance", ui->leDist->text());
	DMSETTINGS->endGroup();
}

void UIModifyOffsetOptions::setDist(double& d, bool initial)
{
	dist = &d;
	bool ok = false;
	if (initial)
	{
		DMSETTINGS->beginGroup("/Draw");
		QString r = DMSETTINGS->readEntry("/ModifyOffsetDistance", "1.0");
		DMSETTINGS->endGroup();

		ui->leDist->setText(r);
		*dist = Math2d::eval(r, &ok);
		if (!ok)
		{
			*dist = 1.;
		}
	}
	else
	{
		*dist = Math2d::eval(ui->leDist->text(), &ok);
		if (!ok)
		{
			*dist = 1.;
		}
	}
}

void UIModifyOffsetOptions::updateDist(const QString& d)
{
	if (dist)
	{
		*dist = Math2d::eval(d);
	}
}
