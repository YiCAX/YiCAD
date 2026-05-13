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

/// @file UIWidgetPen.cpp
/// @brief 画笔属性控件实现

#include "UIWidgetPen.h"

#include <QVariant>

#include "UIColorBox.h"
#include "UILineTypeBox.h"

UIWidgetPen::UIWidgetPen(QWidget* parent, Qt::WindowFlags fl)
	: QWidget(parent, fl)
{
	setupUi(this);
}

UIWidgetPen::~UIWidgetPen()
{
	// no need to delete child widgets, Qt does it all for us
}

void UIWidgetPen::setPen(DmPen pen, bool showByLayer, const QString& title)
{
	cbColor->init(showByLayer);
	cbWidth->init(showByLayer);
	cbLineType->init(showByLayer);

	cbColor->setColor(pen.getColor());
	cbWidth->setWidth(pen.getWidth());
	cbLineType->setLineType(pen.getLineType());

	if (!title.isEmpty())
	{
		bgPen->setTitle(title);
	}
}

DmPen UIWidgetPen::getPen()
{
	DmPen pen;

	pen.setColor(cbColor->getColor());
	pen.setWidth(cbWidth->getWidth());
	pen.setLineType(cbLineType->getLineType());

	return pen;
}

void UIWidgetPen::languageChange()
{
	retranslateUi(this);
}
