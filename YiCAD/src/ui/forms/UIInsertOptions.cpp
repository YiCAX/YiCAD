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

/// @file UIInsertOptions.cpp
/// @brief 块插入选项控件实现

#include "UIInsertOptions.h"

#include "ActionInterface.h"
#include "ActionBlocksInsert.h"
#include "DmSettings.h"
#include "Math2d.h"
#include "ui_UIInsertOptions.h"
#include "Debug.h"

UIInsertOptions::UIInsertOptions(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, ui(new Ui::Ui_InsertOptions{})
{
	ui->setupUi(this);
}

/// @brief 析构函数，保存设置后释放资源
UIInsertOptions::~UIInsertOptions()
{
	saveSettings();
}

/// @brief 根据当前语言刷新子控件字符串
void UIInsertOptions::languageChange()
{
	ui->retranslateUi(this);
}

void UIInsertOptions::saveSettings()
{
	DMSETTINGS->beginGroup("/Insert");
	DMSETTINGS->writeEntry("/InsertAngle", ui->leAngle->text());
	DMSETTINGS->writeEntry("/InsertFactor", ui->leFactor->text());
	DMSETTINGS->writeEntry("/InsertColumns", ui->sbColumns->text());
	DMSETTINGS->writeEntry("/InsertRows", ui->sbRows->text());
	DMSETTINGS->writeEntry("/InsertColumnSpacing", ui->leColumnSpacing->text());
	DMSETTINGS->writeEntry("/InsertRowSpacing", ui->leRowSpacing->text());
	DMSETTINGS->endGroup();
}

void UIInsertOptions::setAction(ActionInterface* a, bool update)
{
	if (a && a->getEntityType() == DM::ActionBlocksInsert)
	{
		action = static_cast<ActionBlocksInsert*>(a);

		QString sAngle;
		QString sFactor;
		QString sColumns;
		QString sRows;
		QString sColumnSpacing;
		QString sRowSpacing;
		if (update)
		{
			sAngle = QString("%1").arg(Math2d::rad2deg(action->getAngle()));
			sFactor = QString("%1").arg(action->getFactor());
			sColumns = QString("%1").arg(action->getColumns());
			sRows = QString("%1").arg(action->getRows());
			sColumnSpacing = QString("%1").arg(action->getColumnSpacing());
			sRowSpacing = QString("%1").arg(action->getRowSpacing());
		}
		else
		{
			DMSETTINGS->beginGroup("/Insert");
			sAngle = DMSETTINGS->readEntry("/InsertAngle", "0.0");
			sFactor = DMSETTINGS->readEntry("/InsertFactor", "1.0");
			sColumns = DMSETTINGS->readEntry("/InsertColumns", "1");
			sRows = DMSETTINGS->readEntry("/InsertRows", "1");
			sColumnSpacing = DMSETTINGS->readEntry("/InsertColumnSpacing", "1.0");
			sRowSpacing = DMSETTINGS->readEntry("/InsertRowSpacing", "1.0");
			DMSETTINGS->endGroup();
		}
		ui->leAngle->setText(sAngle);
		ui->leFactor->setText(sFactor);
		ui->sbColumns->setValue(sColumns.toInt());
		ui->sbRows->setValue(sRows.toInt());
		ui->leColumnSpacing->setText(sColumnSpacing);
		ui->leRowSpacing->setText(sRowSpacing);
	}
	else
	{
		action = nullptr;
	}
}

void UIInsertOptions::updateData()
{
	if (action)
	{
		action->setAngle(Math2d::deg2rad(Math2d::eval(ui->leAngle->text())));
		action->setFactor(Math2d::eval(ui->leFactor->text()));
		action->setColumns(ui->sbColumns->value());
		action->setRows(ui->sbRows->value());
		action->setColumnSpacing(Math2d::eval(ui->leColumnSpacing->text()));
		action->setRowSpacing(Math2d::eval(ui->leRowSpacing->text()));
	}
}
