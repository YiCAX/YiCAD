/*
 * Copyright (C) 2026 YiCAD Team
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

/// @file UIDimStyleBox.cpp
/// @brief 标注样式选择下拉框控件

#include "UIDimStyleBox.h"
#include "DmDimensionStyle.h"
#include "DmDimensionStyleTable.h"

UIDimStyleBox::UIDimStyleBox(QWidget* parent /*= nullptr*/)
	: QComboBox(parent), m_pStyle(nullptr)
	, m_pDimStyleTable(nullptr)
{
}

DmDimensionStyle* UIDimStyleBox::getStyle()
{
	return m_pStyle;
}

void UIDimStyleBox::setStyle(const QString& style)
{
	setCurrentText(style);
	m_pStyle = m_pDimStyleTable->find(style);
	emit styleChanged();
}

void UIDimStyleBox::init(DmDimensionStyleTable* dimStyleTable, const QString& curStyle)
{
	m_pDimStyleTable = dimStyleTable;
	clear();
	QStringList list;
	for (auto& style : *m_pDimStyleTable)
	{
		list.append(style->getName());
	}
	addItems(list);

	connect(this, SIGNAL(currentTextChanged(const QString&)), this, SLOT(slotStyleChanged(const QString&)));
	DmDimensionStyle* pActive = m_pDimStyleTable->getActive();
	if (pActive)
	{
		if (curStyle.isEmpty())
		{
			int idx = indexOf(pActive);
			setCurrentIndex(idx);
			setStyle(pActive->getName());
		}
		else
		{
			int idx = indexOf(curStyle);
			if (idx != -1)
			{
				setCurrentIndex(idx);
				setStyle(curStyle);
			}
		}
	}
}

int UIDimStyleBox::indexOf(DmDimensionStyle* t)
{
    int index = -1;
    int i = 0;
    for (auto style : *m_pDimStyleTable)
    {
        if (style->getName() == t->getName())
        {
            index = i;
            break;
        }
        i++;
    }
    return index;
}

int UIDimStyleBox::indexOf(const QString& name)
{
    int index = -1;
    int i = 0;
    for (auto style : *m_pDimStyleTable)
    {
        if (style->getName() == name)
        {
            index = i;
            break;
        }
        i++;
    }
    return index;
}

void UIDimStyleBox::slotStyleChanged(const QString& style)
{
	m_pStyle = m_pDimStyleTable->find(style);
	emit styleChanged();
}

