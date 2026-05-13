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

/// @file UILayerBox.cpp
/// @brief 图层选择下拉框控件，管理和切换CAD文档中的图层

#include "UILayerBox.h"

#include "DmLayer.h"
#include "DmLayerTable.h"

// Default Constructor. You must call init manually before using this class.
UILayerBox::UILayerBox(QWidget* parent)
    : QComboBox(parent)
{
    m_isShowByBlock = false;
    m_pLayerTable = nullptr;
    m_pCurrentLayer = nullptr;
}

UILayerBox::~UILayerBox()
{
}

/// Initialisation (called manually only once).
/// @param m_pLayerTable Layer list which provides the layer names that are available.
/// @param m_isShowByBlock true: Show attribute ByBlock.
void UILayerBox::init(DmLayerTable& m_pLayerTable, bool m_isShowByBlock)
{
    this->m_isShowByBlock = m_isShowByBlock;
    this->m_pLayerTable = &m_pLayerTable;

    for (auto it = m_pLayerTable.begin(); it != m_pLayerTable.end(); ++it)
    {
        DmLayer* lay = *it;
        if (lay && (lay->getName() != "ByBlock" || m_isShowByBlock))
        {
            addItem(lay->getName());
        }
    }

    connect(this, SIGNAL(activated(int)), this, SLOT(slotLayerChanged(int)));

    setCurrentIndex(0);

    slotLayerChanged(currentIndex());
}

DmLayer* UILayerBox::getLayer()
{
    return m_pCurrentLayer;
}

// Sets the layer shown in the combobox to the given layer.
void UILayerBox::setLayer(DmLayer& layer)
{
    m_pCurrentLayer = &layer;

    int i = findText(layer.getName());
    setCurrentIndex(i);

    slotLayerChanged(currentIndex());
}

// Sets the layer shown in the combobox to the given layer.
void UILayerBox::setLayer(QString& layer)
{
    int i = findText(layer);
    setCurrentIndex(i);

    slotLayerChanged(currentIndex());
}

// Called when the color has changed. This method
// sets the current color to the value chosen or even
// offers a dialog to the user that allows him/ her to
// choose an individual color.
void UILayerBox::slotLayerChanged(int index)
{
    m_pCurrentLayer = m_pLayerTable->find(itemText(index));

    emit layerChanged(m_pCurrentLayer);
}
