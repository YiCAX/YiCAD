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

/// @file UIPolylineOptions.h
/// @brief 多段线选项控件

#ifndef UIPOLYLINEOPTIONS_H
#define UIPOLYLINEOPTIONS_H

#include <memory>
#include <QWidget>

class ActionInterface;
class ActionDrawPolyline;
namespace Ui
{
	class Ui_PolylineOptions;
}

/// @brief 多段线小工具条
class UIPolylineOptions : public QWidget
{
	Q_OBJECT

public:
	/// @brief 构造函数
	/// @param parent 父窗口指针
	/// @param fl 窗口标志
	UIPolylineOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

	/// @brief 析构函数
	~UIPolylineOptions();

public slots:
	/// @brief 设置关联的 Action
	/// @param a Action 接口指针
	/// @param update 是否从 Action 更新 UI
	virtual void setAction(ActionInterface* a, bool update);
	virtual void close();
	virtual void undo();
	virtual void updateRadius(const QString& s);
	virtual void updateAngle(const QString& s);
	virtual void updateDirection(bool);
	virtual void updateMode(int m);

	void updateStartLineWeight();
	void updateEndLineWeight();

protected:
	ActionDrawPolyline* action = nullptr; ///< 多段线 Action 指针

protected slots:
	virtual void languageChange();

private:
	void destroy();
	std::unique_ptr<Ui::Ui_PolylineOptions> ui; ///< UI 对象
};

#endif // UIPOLYLINEOPTIONS_H
