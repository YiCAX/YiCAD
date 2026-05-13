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

/// @file UIDimzerosBox.h
/// @brief 尺寸零抑制选择下拉框控件

#ifndef UIDIMZEROSBOX_H
#define UIDIMZEROSBOX_H

#include <QComboBox>
#include <QStandardItemModel>
#include <QListView>

// DimZin value is mixed integer and bit flag value
// inches and feet are integer values, removal of left and right zeros are flags
// 0: removes 0' & 0"
// 1: draw 0' & 0"
// 2: removes 0"
// 4: removes 0'
// bit 3 set (4) remove 0 to the left
// bit 4 set (8) removes 0's to the right
// DimAZin value is integer or bit flag value
// 0: draw all
// 1: remove 0 to the left
// 2: removes 0's to the right
// 3: removes all zeros

class UIDimzerosBox : public QComboBox
{
	Q_OBJECT

public:
	explicit UIDimzerosBox(QWidget* parent = nullptr);
	~UIDimzerosBox();
	void setLinear();
	void setData(int i);
	int getData();
private:
	int convertDimZin(int v, bool toIdx);

private:
	QStandardItemModel*		m_pModel = nullptr;
	QListView*				m_pView = nullptr;
	bool					m_isDimLine = false;
};

#endif
