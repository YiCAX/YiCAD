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

/// @file UISnapMiddleOptions.cpp
/// @brief 捕捉中点选项控件实现

#include "UISnapMiddleOptions.h"

#include <QVariant>
#include "ui_UISnapMiddleOptions.h"

namespace
{
    constexpr int MIN_MIDDLE_POINTS = 1;
    constexpr int MAX_MIDDLE_POINTS = 99;
    constexpr int DEFAULT_MIDDLE_POINTS = 3;
}

UISnapMiddleOptions::UISnapMiddleOptions(int& i, QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, middlePoints(&i)
	, ui(new Ui::Ui_SnapMiddleOptions())
{
	ui->setupUi(this);
}

UISnapMiddleOptions::~UISnapMiddleOptions()
{
	saveSettings();
}

void UISnapMiddleOptions::languageChange()
{
	ui->retranslateUi(this);
}

void UISnapMiddleOptions::saveSettings()
{
	DMSETTINGS->beginGroup("/Snap");
	DMSETTINGS->writeEntry("/MiddlePoints", *middlePoints);
	DMSETTINGS->endGroup();
}

void UISnapMiddleOptions::setMiddlePoints(int& i, bool initial)
{
	middlePoints = &i;
	if (initial)
	{
		DMSETTINGS->beginGroup("/Snap");
		*middlePoints = DMSETTINGS->readNumEntry("/MiddlePoints", DEFAULT_MIDDLE_POINTS);
		if (!(*middlePoints >= MIN_MIDDLE_POINTS && *middlePoints <= MAX_MIDDLE_POINTS))
		{
			*middlePoints = MIN_MIDDLE_POINTS;
			DMSETTINGS->writeEntry("/MiddlePoints", MIN_MIDDLE_POINTS);
		}
		ui->sbMiddlePoints->setValue(*middlePoints);
		DMSETTINGS->endGroup();
	}
	else
	{
		*middlePoints = ui->sbMiddlePoints->value();
	}
}

void UISnapMiddleOptions::updateMiddlePoints()
{
	if (middlePoints)
	{
		*middlePoints = ui->sbMiddlePoints->value();
	}
}

void UISnapMiddleOptions::on_sbMiddlePoints_valueChanged(int i)
{
	if (middlePoints)
	{
		*middlePoints = i;
	}
}
