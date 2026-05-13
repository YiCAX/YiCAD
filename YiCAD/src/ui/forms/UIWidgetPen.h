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

/// @file UIWidgetPen.h
/// @brief 画笔属性控件（颜色、线宽、线型）

#ifndef UIWIDGETPEN_H
#define UIWIDGETPEN_H

#include "ui_UIWidgetPen.h"
#include "DmPen.h"

class UIWidgetPen : public QWidget, public Ui::UIWidgetPen
{
	Q_OBJECT

public:
	/// @brief 构造函数
	/// @param parent 父窗口指针
	/// @param fl 窗口标志
	UIWidgetPen(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

	/// @brief 析构函数
	~UIWidgetPen();

public slots:
	/// @brief 设置画笔属性
	/// @param pen 画笔对象
	/// @param showByLayer 是否显示随层选项
	/// @param title 标题
	virtual void setPen(DmPen pen, bool showByLayer, const QString& title);

	/// @brief 获取当前画笔属性
	/// @return 画笔对象
	virtual DmPen getPen();

protected slots:
	virtual void languageChange();

};

#endif // UIWIDGETPEN_H
