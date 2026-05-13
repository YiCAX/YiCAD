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

/// @file UISnapDistOptions.h
/// @brief 捕捉距离选项控件

#ifndef UISNAPDISTOPTIONS_H
#define UISNAPDISTOPTIONS_H

#include <memory>
#include <QWidget>

namespace Ui {
	class Ui_SnapDistOptions;
}

class UISnapDistOptions : public QWidget
{
	Q_OBJECT

public:
	/// @brief 构造函数
	/// @param parent 父窗口指针
	/// @param fl 窗口标志
	UISnapDistOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

	/// @brief 析构函数
	~UISnapDistOptions();

public slots:
	/// @brief 设置距离参数
	/// @param d 距离引用
	/// @param initial 是否从设置初始化
	virtual void setDist(double& d, bool initial = true);

	/// @brief 更新距离
	/// @param d 距离字符串
	virtual void updateDist(const QString& d);

protected:
	double* dist = nullptr; ///< 距离指针

protected slots:
	virtual void languageChange();

private:
	void saveSettings();
	std::unique_ptr<Ui::Ui_SnapDistOptions> ui; ///< UI 对象
};

#endif // UISNAPDISTOPTIONS_H
