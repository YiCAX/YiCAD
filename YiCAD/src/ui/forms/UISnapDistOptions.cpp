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

/// @file UISnapDistOptions.cpp
/// @brief 捕捉距离选项控件实现

#include "UISnapDistOptions.h"

#include <QVariant>
#include "Math2d.h"
#include "ui_UISnapDistOptions.h"

/// @brief 构造捕捉距离选项控件
/// @param parent 父窗口指针
/// @param fl 窗口标志
UISnapDistOptions::UISnapDistOptions(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, ui(new Ui::Ui_SnapDistOptions{})
{
	ui->setupUi(this);
}

/// @brief 析构函数，保存设置后释放资源
UISnapDistOptions::~UISnapDistOptions()
{
	saveSettings();
}

/// @brief 根据当前语言刷新子控件字符串
void UISnapDistOptions::languageChange()
{
	ui->retranslateUi(this);
}

void UISnapDistOptions::saveSettings()
{
	DMSETTINGS->beginGroup("/Snap");
	DMSETTINGS->writeEntry("/Distance", ui->leDist->text());
	DMSETTINGS->endGroup();
}

void UISnapDistOptions::setDist(double& d, bool initial)
{
	dist = &d;
	if (initial)
	{
		DMSETTINGS->beginGroup("/Snap");
		QString r = DMSETTINGS->readEntry("/Distance", "1.0");
		DMSETTINGS->endGroup();

		ui->leDist->setText(r);
		*dist = r.toDouble();
	}
	else
	{
		*dist = ui->leDist->text().toDouble();
	}
}

void UISnapDistOptions::updateDist(const QString& d)
{
	if (dist)
	{
		*dist = Math2d::eval(d, 1.0);
	}
}
