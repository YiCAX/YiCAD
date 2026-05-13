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

/// @file UISnapMiddleOptions.h
/// @brief 捕捉中点选项控件

#ifndef UISNAPMIDDLEOPTIONS_H
#define UISNAPMIDDLEOPTIONS_H

#include <memory>
#include <QWidget>

namespace Ui
{
	class Ui_SnapMiddleOptions;
}

class UISnapMiddleOptions : public QWidget
{
	Q_OBJECT

public:
	/// @brief 构造函数
	/// @param i 中点数量引用
	/// @param parent 父窗口指针
	/// @param fl 窗口标志
	UISnapMiddleOptions(int& i, QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

	/// @brief 析构函数
	~UISnapMiddleOptions();

public slots:
	/// @brief 设置中点数量
	/// @param i 中点数量引用
	/// @param initial 是否从设置初始化
	virtual void setMiddlePoints(int& i, bool initial = true);

	/// @brief 从中点 SpinBox 更新中点数量
	virtual void updateMiddlePoints();

	virtual void languageChange();

	/// @brief SpinBox 值变更槽
	/// @param arg1 新值
	void on_sbMiddlePoints_valueChanged(int arg1);

private:
	void saveSettings();

protected:
	int* middlePoints = nullptr; ///< 中点数量指针

private:
	std::unique_ptr<Ui::Ui_SnapMiddleOptions> ui; ///< UI 对象
};

#endif // UISNAPMIDDLEOPTIONS_H
