/*
 * Copyright (C) 2024 YiCAD Team
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

/// @file UILineTypeBox.h
/// @brief 线型下拉列表框

#ifndef UILINETYPEBOX_H
#define UILINETYPEBOX_H

#include <QComboBox>
#include <QMessageBox>

#include "Datamodel.h"
#include "DmDocument.h"

class DmLineTypeTable;
class DmLineType;

/// @brief 线型下拉列表框
class UILineTypeBox : public QComboBox 
{
	Q_OBJECT
public:
	UILineTypeBox(QWidget* parent = nullptr);
	virtual ~UILineTypeBox();

	DmLineType* getLineType();
	void setLineType(DmLineType* t);
	void setLineType(DmLineType* t, DmDocument* d);
    int indexOf(DmLineType* t);
    DmLineType* lineTypeAt(int idx);

	void init(bool showByLayer);

	//update list
	void updateLineTypeTable();
	void updateLineTypeTable(DmDocument* doc);

private slots:
	void slotLineTypeChanged(int index);

signals:
	void lineTypeChanged(DmLineType* data);

private:
	bool				m_isShowByLayer = false;
    bool                m_isChangingByCode = false;

	DmLineType*			m_currentLineType = nullptr;
	DmLineTypeTable*	m_LineTypeTable = nullptr;    ///< 操作的线型表
	DmDocument*			m_document = nullptr;         ///< 线型表所在文档，默认为当前文档
	
};

#endif

