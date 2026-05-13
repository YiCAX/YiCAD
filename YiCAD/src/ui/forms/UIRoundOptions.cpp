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

/// @file UIRoundOptions.cpp
/// @brief 倒圆角选项控件实现

#include "UIRoundOptions.h"

#include "ActionModifyRound.h"
#include "DmSettings.h"
#include "Math2d.h"
#include "ui_UIRoundOptions.h"
#include "Debug.h"

UIRoundOptions::UIRoundOptions(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, ui(new Ui::Ui_RoundOptions{})
{
	ui->setupUi(this);
}

/// @brief 析构函数，保存设置后释放资源
UIRoundOptions::~UIRoundOptions()
{
	saveSettings();
}

/// @brief 根据当前语言刷新子控件字符串
void UIRoundOptions::languageChange()
{
	ui->retranslateUi(this);
}

void UIRoundOptions::saveSettings()
{
	DMSETTINGS->beginGroup("/Modify");
	DMSETTINGS->writeEntry("/RoundRadius", ui->leRadius->text());
	DMSETTINGS->writeEntry("/RoundTrim", (int)ui->cbTrim->isChecked());
	DMSETTINGS->endGroup();
}

void UIRoundOptions::setAction(ActionInterface* a, bool update)
{
	if (a && a->getEntityType() == DM::ActionModifyRound)
	{
		action = static_cast<ActionModifyRound*>(a);

		QString sr;
		QString st;
		if (update)
		{
			sr = QString("%1").arg(action->getRadius());
			st = QString("%1").arg((int)action->isTrimOn());
		}
		else
		{
			DMSETTINGS->beginGroup("/Modify");
			sr = DMSETTINGS->readEntry("/RoundRadius", "1.0");
			st = DMSETTINGS->readEntry("/RoundTrim", "1");
			DMSETTINGS->endGroup();
		}
		ui->leRadius->setText(sr);
		ui->cbTrim->setChecked(st == "1");
	}
	else
	{
		action = nullptr;
	}
}

void UIRoundOptions::updateData()
{
	if (action)
	{
		action->setTrim(ui->cbTrim->isChecked());
		action->setRadius(Math2d::eval(ui->leRadius->text()));
	}
}
