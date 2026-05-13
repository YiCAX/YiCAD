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

/// @file UILineBisectorOptions.h
/// @brief 角平分线选项控件

#ifndef UILINEBISECTOROPTIONS_H
#define UILINEBISECTOROPTIONS_H

#include <memory>
#include <QWidget>

class ActionInterface;
class ActionDrawLineBisector;
namespace Ui {
	class Ui_LineBisectorOptions;
}

class UILineBisectorOptions : public QWidget
{
	Q_OBJECT

public:
	/// @brief 构造函数
	/// @param parent 父窗口指针
	/// @param fl 窗口标志
	UILineBisectorOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

	/// @brief 析构函数
	~UILineBisectorOptions();

public slots:
	/// @brief 设置关联的 Action
	/// @param a Action 接口指针
	/// @param update 是否从 Action 更新 UI
	virtual void setAction(ActionInterface* a, bool update);

	/// @brief 更新角平分线长度
	/// @param l 长度字符串
	virtual void updateLength(const QString& l);

	/// @brief 更新角平分线数量
	/// @param n 数量
	virtual void updateNumber(int n);

protected:
	ActionDrawLineBisector* action = nullptr; ///< 角平分线 Action 指针

protected slots:
	virtual void languageChange();

private:
	void saveSettings();
	std::unique_ptr<Ui::Ui_LineBisectorOptions> ui; ///< UI 对象
};

#endif // UILINEBISECTOROPTIONS_H
