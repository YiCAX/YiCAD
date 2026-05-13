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

/// @file UILineOptions.h
/// @brief 两点画线交互设置

#ifndef UILINEOPTIONS_H
#define UILINEOPTIONS_H

#include <memory>
#include <QWidget>

class ActionInterface;
class ActionDrawLine;

namespace Ui
{
	class Ui_LineOptions;
}

/// @class UILineOptions
/// @brief 两点画线交互设置
class UILineOptions : public QWidget
{
	Q_OBJECT

public:
	/// @brief 构造函数
	/// @param parent 父窗口指针
	/// @param fl 窗口标志
	UILineOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

	/// @brief 析构函数
	~UILineOptions();

public slots:
	/// @brief 设置关联的 Action
	/// @param a Action 接口指针
	virtual void setAction(ActionInterface* a);
	virtual void close();
	virtual void undo();
	virtual void redo();

protected:
	ActionDrawLine* action = nullptr; ///< 画线 Action 指针

protected slots:
	virtual void languageChange();

private:
	std::unique_ptr<Ui::Ui_LineOptions> ui; ///< UI 对象
};

#endif // UILINEOPTIONS_H
