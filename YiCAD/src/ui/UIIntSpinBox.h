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

/// @file UIIntSpinBox.h
/// @brief 一种不显示结尾0的整数SpinBox

#ifndef UIINTSPINBOX_H
#define UIINTSPINBOX_H

#include <QSpinBox>

/// @class UIIntSpinBox
/// @brief 一种不显示结尾0的整数SpinBox
class UIIntSpinBox : public QSpinBox
{
	Q_OBJECT
public:
	UIIntSpinBox(QWidget* parent = nullptr);
	virtual ~UIIntSpinBox() = default;
	UIIntSpinBox(const UIIntSpinBox&) = delete;
	UIIntSpinBox& operator=(const UIIntSpinBox&) = delete;
};
#endif