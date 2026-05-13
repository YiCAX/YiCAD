/*
 * Copyright (C) 2024 YiCAD Team
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

/// @file UIPatternBox.h
/// @brief 填充图案选择下拉列表框

#ifndef UIPATTERNBOX_H
#define UIPATTERNBOX_H

#include <QComboBox>

class DmPattern;

/// @brief 填充图案选择下拉列表框
class UIPatternBox : public QComboBox 
{
	Q_OBJECT

public:
	UIPatternBox(QWidget* parent = nullptr);
	virtual ~UIPatternBox();

	DmPattern* getPattern();

	void setPattern(const QString& pName);

	void init();

private slots:
	void slotPatternChanged(int index);

signals:
	void patternChanged();

private:
	DmPattern* m_pCurrentPattern = nullptr;
};

#endif
