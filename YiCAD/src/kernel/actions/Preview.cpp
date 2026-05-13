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

/// @file Preview.cpp
/// @brief 预览功能类的实现

#include "Preview.h"

#include "DmEntityContainer.h"
#include "DmLine.h"
#include "GuiDocumentView.h"
#include "Information.h"
#include "DmSettings.h"
#include "DmDocument.h"

Preview::Preview(DmDocument* pDocument)
    : m_pDocument(pDocument)
    , m_pPreviewContainer(nullptr)
{
    if (m_pDocument)
    {
        auto docView = m_pDocument->getDocumentView();
        m_pPreviewContainer = docView->getPreviewContainer();
    }
}

DM::EntityType Preview::getEntityType() const
{
    return DM::EntityPreview;
}

void Preview::addCloneOf(DmEntity* entity)
{
    if (!entity)
    {
        return;
    }

    DmEntity* clone = entity->clone();
    clone->setParent(nullptr);
    addEntity(clone);
}

void Preview::addAllFrom(DmEntityContainer& container)
{
    int c = 0;
    for (auto e : container)
    {
        DmEntity* clone = e->clone();
        clone->setSelected(false);
        clone->setParent(nullptr);

        c++;
        addEntity(clone);
    }
}

void Preview::addSelectionFromDocument()
{
    if (!m_pDocument)
    {
        return;
    }
    auto table = m_pDocument->getEntityTable();
    for (auto e : *table)
    {
        if (e->isSelected())
        {
            DmEntity* clone = e->clone();
            clone->setSelected(false);
            clone->setParent(nullptr);
            addEntity(clone);
        }
    }
}

void Preview::addEntity(DmEntity* pEntity)
{
    //pEntity->setDocument(m_pDocument);
    m_pPreviewContainer->addEntity(pEntity);
    specifyPreviewModified();
}

void Preview::appendEntity(DmEntity* pEntity)
{
    //pEntity->setDocument(m_pDocument);
    m_pPreviewContainer->appendEntity(pEntity);
    specifyPreviewModified();
}

void Preview::removeEntity(DmEntity* pEntity)
{
    m_pPreviewContainer->removeEntity(pEntity);
    specifyPreviewModified();
}

void Preview::clear()
{
    m_pPreviewContainer->clear();
    specifyPreviewModified();
}

void Preview::setModelOffset(const DmVector& offset)
{
    if (m_pDocument)
    {
        auto docView = m_pDocument->getDocumentView();
        docView->setPreviewModelOffset(offset);
    }
}

bool Preview::isEmpty()
{
    return m_pPreviewContainer->isEmpty();
}

void Preview::move(DmVector offset)
{
    m_pPreviewContainer->move(offset);
    specifyPreviewModified();
}

void Preview::moveRef(DmVector v1, DmVector v2)
{
    m_pPreviewContainer->moveRef(v1, v2);
    specifyPreviewModified();
}

void Preview::setVisible(bool isVisble)
{
    m_pPreviewContainer->setVisible(isVisble);
    specifyPreviewModified();
}

DmEntityContainer* Preview::getEntityContainer()
{
    return m_pPreviewContainer;
}

/// @brief 通知文档视图预览内容已修改，触发重绘
void Preview::specifyPreviewModified()
{
    if (m_pDocument)
    {
        auto docView = m_pDocument->getDocumentView();
        docView->specifyPreviewModified();
    }
}
