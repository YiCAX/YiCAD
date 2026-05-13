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

/// @file UIDoubleSpinBox.h
/// @brief 不显示结尾0的QDoubleSpinBox派生控件

#ifndef UIDOUBLESPINBOX_H
#define UIDOUBLESPINBOX_H

#include <QDoubleSpinBox>

/// @class UIDoubleSpinBox
/// @brief 一种不显示结尾0的SpinBox
class UIDoubleSpinBox : public QDoubleSpinBox
{
	Q_OBJECT
public:
	UIDoubleSpinBox(QWidget* parent = nullptr);
	virtual ~UIDoubleSpinBox() = default;
	UIDoubleSpinBox(const UIDoubleSpinBox&) = delete;
	UIDoubleSpinBox& operator=(const UIDoubleSpinBox&) = delete;

	virtual QString textFromValue(double value) const override;
};
#endif