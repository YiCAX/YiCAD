/*
 * Copyright (C) 2026 YiCAD Team
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

/// @file UIDimStyleListBox.h
/// @brief 标注样式列表控件，支持新建、删除、修改标注样式

#ifndef UIDIMSTYLELISTBOX_H
#define UIDIMSTYLELISTBOX_H

#include <QListWidget>
#include <QAction>
class DmDocument;
class DmDimensionStyleTable;
class DmDimensionStyle;
class UIDimStyleListBox : public QListWidget
{
	Q_OBJECT
public:
	UIDimStyleListBox(QWidget* parent = nullptr);
	virtual ~UIDimStyleListBox() = default;

public:
	void init(DmDimensionStyleTable* dimStyleTable, DmDocument* document);
	DmDimensionStyle* activeStyle() const;
	bool canSelectedDelete() const;
	DmDimensionStyle* selectedStyle() const;
    DmDimensionStyle* styleAt(int i) const;
    int indexOf(DmDimensionStyle* t);
    /// @brief 是否可删除标注样式
    bool canRemoveDimStyle(DmDimensionStyle* style);

private slots:
	void slotItemSelectionChanged();
	void slotItemDoubleClicked(QListWidgetItem* item);

public slots:
	void slotNewDimStyle();
	void slotDeleteDimStyle();
	void slotSetActiveDimStyle();
	void slotRenameDimStyle();
	void slotModifyDimStyle();

signals:
    /// @brief 当前样式修改后触发的信号
	void activeStyleChanged(QString);
	void selectedStyleChanged();
	/// @brief 标注样式修改后触发的信号
	void styleChanged();

private:
	void updateUI();

private:
	DmDocument*				m_pDocument = nullptr;
	DmDimensionStyleTable*	m_pDimStyleTable = nullptr;
	DmDimensionStyle*		m_pActiveStyle = nullptr;
	DmDimensionStyle*		m_pSelectedStyle = nullptr;
};

#endif 