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

/// @file UISpecialCharBox.h
/// @brief 特殊符号列表框

#ifndef UISPECIALCHARBOX_H
#define UISPECIALCHARBOX_H

#include <QComboBox>

/// @brief 特殊符号列表框，发送可显示的QString
class UISpecialCharBox : public QComboBox
{
	Q_OBJECT
public:
	UISpecialCharBox(QWidget* parent = nullptr);
	void init();

signals:
	void charActivated(const QString& text);

private slots:
	void slotTextActivated(const QString& text);
};
#endif //!UISPECIALCHARBOX_H