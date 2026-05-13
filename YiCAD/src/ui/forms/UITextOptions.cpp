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

/// @file UITextOptions.cpp
/// @brief 文字选项控件实现

#include "UITextOptions.h"

#include "ActionDrawText.h"
#include "Math2d.h"
#include "ui_UITextOptions.h"
#include "Debug.h"

UITextOptions::UITextOptions(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, ui(new Ui::Ui_TextOptions{})
{
	ui->setupUi(this);
}

/// @brief 析构函数（默认实现）
UITextOptions::~UITextOptions() = default;

/// @brief 根据当前语言刷新子控件字符串
void UITextOptions::languageChange()
{
	ui->retranslateUi(this);
}

void UITextOptions::setAction(ActionInterface* a, bool update)
{
	if (a && a->getEntityType() == DM::ActionDrawText)
	{
		action = static_cast<ActionDrawText*>(a);

		QString st;
		QString sa;
		if (update)
		{
			st = action->getText();
			sa = QString("%1").arg(Math2d::rad2deg(action->getAngle()));
		}
		else
		{
			st = "";
			sa = "0.0";
		}

		ui->teText->setText(st);
		ui->leAngle->setText(sa);
	}
	else
	{
		action = nullptr;
	}
}

void UITextOptions::updateText()
{
	if (action)
	{
		action->setText(ui->teText->toPlainText());
	}
}

void UITextOptions::updateAngle()
{
	if (action)
	{
		action->setAngle(Math2d::deg2rad(Math2d::eval(ui->leAngle->text())));
	}
}
