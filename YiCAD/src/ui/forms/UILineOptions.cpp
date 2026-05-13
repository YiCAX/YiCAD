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

/// @file UILineOptions.cpp
/// @brief 两点画线交互设置实现

#include "UILineOptions.h"

#include "ActionDrawLine.h"
#include "ui_UILineOptions.h"
#include "Debug.h"

UILineOptions::UILineOptions(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, ui(new Ui::Ui_LineOptions())
{
	ui->setupUi(this);
}

UILineOptions::~UILineOptions() = default;

void UILineOptions::languageChange()
{
	ui->retranslateUi(this);
}

void UILineOptions::setAction(ActionInterface* a)
{
	if (a && a->getEntityType() == DM::ActionDrawLine)
	{
		action = static_cast<ActionDrawLine*>(a);
	}
	else
	{
		action = nullptr;
	}
}

void UILineOptions::close()
{
	if (action)
	{
		action->close();
	}
}

void UILineOptions::undo()
{
	if (action)
	{
		action->undo();
	}
}

void UILineOptions::redo()
{
	if (action)
	{
		action->redo();
	}
}
