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

/// @file UIMTextModifyOptions.cpp
/// @brief 多行文字修改选项控件实现

#include "UIMTextModifyOptions.h"
#include "ActionModifyMText.h"
#include "DmDocument.h"
#include "Math2d.h"

UIMTextModifyOptions::UIMTextModifyOptions(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
	, m_isDlgShow(false)
	, action(nullptr)
{
	setupUi(this);
}

void UIMTextModifyOptions::setAction(ActionModifyMText* a)
{
	action = a;
	init();
}

void UIMTextModifyOptions::init()
{
	// 文字样式
	connect(cbStyle, SIGNAL(styleChanged()), this, SLOT(slotStyleChanged()));
	DmTextStyleTable* textStyleTable = action->getDocument()->getTextStyleTable();
	cbStyle->init(textStyleTable);

	//其他
	connect(leHeight, SIGNAL(editingFinished()), this, SLOT(slotEditLineEditingFinished()));
	connect(leLineSpaceFactor, SIGNAL(editingFinished()), this, SLOT(slotEditLineEditingFinished()));
	connect(leAngle, SIGNAL(editingFinished()), this, SLOT(slotEditLineEditingFinished()));
	connect(leLineSpace, SIGNAL(editingFinished()), this, SLOT(slotEditLineEditingFinished()));

	updateUIFromData();
}

void UIMTextModifyOptions::updateUIFromData()
{
	cbStyle->setStyle(action->getStyle()->getName());
	leHeight->setText(QString::number(action->getHeight()));
	leLineSpaceFactor->setText(QString::number(action->getLineSpaceFatctor()));
	leLineSpace->setText(QString::number(action->getLineSpace()));
	double angle_deg = Math2d::rad2deg(action->getAngle());
	leAngle->setText(QString::number(angle_deg));
}

void UIMTextModifyOptions::showEvent(QShowEvent* ev)
{
	QWidget::showEvent(ev);
	m_isDlgShow = true;
}

void UIMTextModifyOptions::languageChange()
{
	retranslateUi(this);
}

void UIMTextModifyOptions::slotEditLineEditingFinished()
{
	if (!m_isDlgShow)
	{
		return;
	}

	if (sender() == leHeight)
	{
		double height = leHeight->text().toDouble();
		if (height <= 0.0)
		{
			return;
		}
		action->setHeight(height);
	}
	else if (sender() == leLineSpaceFactor)
	{
		double factor = leLineSpaceFactor->text().toDouble();
		if (factor <= 0.0)
		{
			return;
		}
		action->setLineSpaceFatctor(factor);
		double lineSpace = action->getLineSpace();
		leLineSpace->setText(QString::number(lineSpace));
	}
	else if (sender() == leLineSpace)
	{
		double lineSpace = leLineSpace->text().toDouble();
		action->setLineSpace(lineSpace);
		double factor = action->getLineSpaceFatctor();
		leLineSpaceFactor->setText(QString::number(factor));
	}
	else if (sender() == leAngle)
	{
		double angle_deg = leAngle->text().toDouble();
		action->setAngle(Math2d::deg2rad(angle_deg));
	}
}

void UIMTextModifyOptions::slotStyleChanged()
{
	if (!m_isDlgShow)
	{
		return;
	}
	action->setStyle(cbStyle->getStyle());
}
