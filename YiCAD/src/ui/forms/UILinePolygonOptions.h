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

/// @file UILinePolygonOptions.h
/// @brief 多边形选项控件

#ifndef UILINEPOLYGONOPTIONS_H
#define UILINEPOLYGONOPTIONS_H

#include <memory>
#include <QWidget>

class ActionInterface;
class ActionDrawLinePolygonCenCor;
namespace Ui {
	class Ui_LinePolygonOptions;
}

class UILinePolygonOptions : public QWidget
{
	Q_OBJECT

public:
	/// @brief 构造函数
	/// @param parent 父窗口指针
	/// @param fl 窗口标志
	UILinePolygonOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

	/// @brief 析构函数
	~UILinePolygonOptions();

public slots:
	/// @brief 设置关联的 Action
	/// @param a Action 接口指针
	/// @param update 是否从 Action 更新 UI
	virtual void setAction(ActionInterface* a, bool update);

	/// @brief 更新多边形边数
	/// @param n 边数
	virtual void updateNumber(int n);

protected:
	ActionDrawLinePolygonCenCor* action = nullptr; ///< 多边形 Action 指针

protected slots:
	virtual void languageChange();

private:
	void saveSettings();
	std::unique_ptr<Ui::Ui_LinePolygonOptions> ui; ///< UI 对象
};

#endif // UILINEPOLYGONOPTIONS_H
