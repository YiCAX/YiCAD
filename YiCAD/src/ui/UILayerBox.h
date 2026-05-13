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

/// @file UILayerBox.h
/// @brief 图层选择下拉列表框

#ifndef UILAYERBOX_H
#define UILAYERBOX_H

#include <QComboBox>

class DmLayer;
class DmLayerTable;

/// @brief 图层选择下拉列表框
class UILayerBox : public QComboBox 
{
	Q_OBJECT

public:
	UILayerBox(QWidget* parent = nullptr);
	virtual ~UILayerBox();

	DmLayer* getLayer();
	void setLayer(DmLayer& layer);
	void setLayer(QString& layer);

	void init(DmLayerTable& layerList, bool showByBlock);

private slots:
	void slotLayerChanged(int index);

signals:
	void layerChanged(DmLayer* layer);

private:
	DmLayerTable*	m_pLayerTable = nullptr;
	DmLayer*		m_pCurrentLayer = nullptr;
	bool			m_isShowByBlock = false;
};

#endif
