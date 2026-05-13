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

/// @file UIDimStyleBox.h
/// @brief 标注样式选择下拉框控件

#ifndef UIDIMSTYLEBOX_H
#define UIDIMSTYLEBOX_H

#include <QComboBox>

class DmDimensionStyle;
class DmDimensionStyleTable;

class UIDimStyleBox : public QComboBox
{
	Q_OBJECT
public:
	UIDimStyleBox(QWidget* parent = nullptr);
	virtual ~UIDimStyleBox() = default;

public:
	DmDimensionStyle* getStyle();
	void setStyle(const QString& style);

	void init(DmDimensionStyleTable* styleList, const QString& curStyle);
    int indexOf(DmDimensionStyle* t);
    int indexOf(const QString& name);

private slots:
	void slotStyleChanged(const QString& text);
signals:
	void styleChanged();
private:
	DmDimensionStyle*		m_pStyle = nullptr;
	DmDimensionStyleTable*	m_pDimStyleTable = nullptr;
};
#endif 