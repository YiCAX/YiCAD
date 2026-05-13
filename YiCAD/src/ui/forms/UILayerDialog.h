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

/// @file UILayerDialog.h
/// @brief 图层配置弹窗

#ifndef UILAYERDIALOG_H
#define UILAYERDIALOG_H

#include "ui_UILayerDialog.h"

class DmLayer;
class DmLayerTable;

/// @brief 图层配置弹窗
class UILayerDialog : public QDialog, public Ui::UILayerDialog
{
	Q_OBJECT

public:
	/// @brief 构造函数
	/// @param parent 父窗口指针
	/// @param name 对象名称
	/// @param modal 是否模态
	/// @param fl 窗口标志
	UILayerDialog(QWidget* parent = nullptr, QString name = QString(), bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());
	~UILayerDialog() = default;

public slots:
	virtual void setLayer(DmLayer* l);
	virtual void updateLayer();
	virtual void validate();
	virtual void setLayerTable(DmLayerTable* layerTable);
	virtual void setEditLayer(bool el);

	/// @return a reference to the QLineEdit object.
	virtual QLineEdit* getQLineEdit();

protected:
	DmLayer*		m_pLayer = nullptr;        ///< 图层对象指针
	DmLayerTable*	m_pLayerTable = nullptr;   ///< 图层表指针
	QString			m_strLayerName;            ///< 图层名称
	bool			m_isEditLayer = false;     ///< 是否为编辑图层模式

protected slots:
	virtual void languageChange();

private:
	void init();

};

#endif // UILAYERDIALOG_H
