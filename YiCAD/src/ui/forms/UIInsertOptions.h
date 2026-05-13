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

/// @file UIInsertOptions.h
/// @brief 块插入选项控件

#ifndef UIINSERTOPTIONS_H
#define UIINSERTOPTIONS_H

#include <memory>
#include <QWidget>

class ActionInterface;
class ActionBlocksInsert;
namespace Ui {
	class Ui_InsertOptions;
}

class UIInsertOptions : public QWidget
{
	Q_OBJECT

public:
	/// @brief 构造函数
	/// @param parent 父窗口指针
	/// @param fl 窗口标志
	UIInsertOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

	/// @brief 析构函数
	~UIInsertOptions();

public slots:
	/// @brief 设置关联的 Action
	/// @param a Action 接口指针
	/// @param update 是否从 Action 更新 UI
	virtual void setAction(ActionInterface* a, bool update);

	/// @brief 将 UI 数据更新到 Action
	virtual void updateData();

protected:
	ActionBlocksInsert* action = nullptr; ///< 块插入 Action 指针

protected slots:
	virtual void languageChange();

private:
	void saveSettings();
	std::unique_ptr<Ui::Ui_InsertOptions> ui; ///< UI 对象
};

#endif // UIINSERTOPTIONS_H
