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

/// @file DmClipboard.cpp
/// @brief 剪贴板实现

#include <iostream>
#include "DmClipboard.h"
#include "DmBlock.h"
#include "DmLayer.h"
#include "DmEntity.h"

DmClipboard* DmClipboard::uniqueInstance = nullptr;

DmClipboard::DmClipboard()
{
}

DmClipboard* DmClipboard::instance()
{
    if (uniqueInstance == nullptr)
    {
        uniqueInstance = new DmClipboard();
    }
    return uniqueInstance;
}

void DmClipboard::clear()
{
    pDocument.getEntityTable()->clear_direct();

    auto layerTable = pDocument.getLayerTable();
    if (layerTable)
    {
        std::vector<DmLayer*> layers;
        for (auto it = layerTable->begin(); it != layerTable->end(); ++it)
        {
            layers.push_back(*it);
        }
        for (auto l : layers)
        {
            layerTable->remove_direct(l);
        }
    }
}

void DmClipboard::addBlock(DmBlock* b)
{
    if (b)
    {
        pDocument.getBlockTable()->add_direct(b);
    }
}

bool DmClipboard::hasBlock(const QString& name)
{
    return (pDocument.getBlockTable()->find(name) != nullptr);
}

int DmClipboard::countBlocks()
{
    return pDocument.getBlockTable()->count();
}

void DmClipboard::addLayer(DmLayer* l)
{
    if (l)
    {
        pDocument.getLayerTable()->add_direct(l);
    }
}

bool DmClipboard::hasLayer(const QString& name)
{
    return (pDocument.getLayerTable()->find(name) != nullptr);
}

void DmClipboard::addEntity(DmEntity* e)
{
    if (e)
    {
        pDocument.getEntityTable()->add_direct(e);
        pDocument.getEntityTable()->updateContainer();
    }
}

unsigned DmClipboard::count()
{
    return pDocument.getEntityTable()->count();
}

DmDocument* DmClipboard::getDocument()
{
    return &pDocument;
}
