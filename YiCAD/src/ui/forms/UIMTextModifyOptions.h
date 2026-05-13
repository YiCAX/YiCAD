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

/// @file UIMTextModifyOptions.h
/// @brief 多行文字修改选项控件

#ifndef UIMTEXTMODIFYOPTIONS_H
#define UIMTEXTMODIFYOPTIONS_H

#include "ui_UIMTextModifyOptions.h"

class ActionModifyMText;

class UIMTextModifyOptions : public QWidget, public Ui::Ui_MTextModifyOptions
{
	Q_OBJECT

public:
	/// @brief 构造函数
	/// @param parent 父窗口指针
	/// @param fl 窗口标志
	UIMTextModifyOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());
	~UIMTextModifyOptions() = default;

public:
	/// @brief 设置关联的修改 Action
	/// @param a 修改多行文字 Action 指针
	virtual void setAction(ActionModifyMText* a);

	/// @brief 初始化控件连接和数据
	void init();

	/// @brief 从数据更新 UI 显示
	void updateUIFromData();

protected:
	/// @brief 窗口显示事件
	/// @param ev 显示事件
	void showEvent(QShowEvent* ev) override;

protected slots:
	virtual void languageChange();

private slots:
	void slotEditLineEditingFinished();
	void slotStyleChanged();

protected:
	ActionModifyMText* action = nullptr; ///< 修改多行文字 Action 指针
	bool m_isDlgShow = false; ///< 窗体是否已显示
};

#endif // !UIMTEXTMODIFYOPTIONS_H
