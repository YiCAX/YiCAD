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

/// @file UITextAlignmentBox.h
/// @brief 文字对齐方式下拉列表

#ifndef UITEXTALIGNMENTBOX_H
#define UITEXTALIGNMENTBOX_H

#include <QComboBox>
#include "TextData.h"

/// @brief 文字对齐方式下拉列表
class UITextAlignmentBox : public QComboBox
{
	Q_OBJECT 
public:
	UITextAlignmentBox(QWidget* parent = nullptr);
	virtual ~UITextAlignmentBox() = default;

	void setAlignment(ETextHorzMode hAlign, ETextVertMode vAlign);
	void getAlignment(ETextHorzMode& hAlign, ETextVertMode& vAlign) const;

	void setAlignment(ETextMode align);
	ETextMode getAlignment() const;
};
#endif	//!UITEXTALIGNMENTBOX_H
