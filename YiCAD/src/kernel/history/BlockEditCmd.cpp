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

/// @file BlockEditCmd.cpp
/// @brief 块编辑括号命令实现

#include "BlockEditCmd.h"

#include "DmDocument.h"
#include "DmBlock.h"
#include "DmBlockTable.h"
#include "DmBlockReference.h"
#include "EntityTable.h"
#include "GuiDocumentView.h"

// ============================================================================
// BlockEditEnterCmd
// ============================================================================

BlockEditEnterCmd::BlockEditEnterCmd(DmDocument* doc, const QString& blockName,
                                     GuiDocumentView* docView)
    : m_pDocument(doc)
    , m_blockName(blockName)
    , m_pDocView(docView)
{
}

void BlockEditEnterCmd::execute()
{
    if (m_isExecuted)
    {
        return;
    }

    DmBlockTable* blockTable = m_pDocument->getBlockTable();
    if (!blockTable)
    {
        return;
    }

    DmBlock* block = blockTable->find(m_blockName);
    if (!block)
    {
        return;
    }

    m_pDocument->setEditBlock(block);
    m_pDocument->regenerate();

    m_isExecuted = true;
}

void BlockEditEnterCmd::undo()
{
    if (!m_isExecuted)
    {
        return;
    }

    m_pDocument->setEditBlock(nullptr);
    updateBlockRefs();
    m_pDocument->regenerate();

    m_isExecuted = false;
}

void BlockEditEnterCmd::redo()
{
    if (m_isExecuted)
    {
        return;
    }

    DmBlockTable* blockTable = m_pDocument->getBlockTable();
    if (!blockTable)
    {
        return;
    }

    DmBlock* block = blockTable->find(m_blockName);
    if (!block)
    {
        return;
    }

    m_pDocument->setEditBlock(block);
    m_pDocument->regenerate();

    m_isExecuted = true;
}

void BlockEditEnterCmd::clear()
{
}

void BlockEditEnterCmd::updateBlockRefs()
{
    // This is called after setEditBlock(nullptr) in undo(),
    // so getEntityTable() returns the document's entity table
    auto entTable = m_pDocument->getEntityTable();
    for (auto e : *entTable)
    {
        if (e && !e->isErased()
            && e->getEntityType() == DM::EntityBlockReference)
        {
            DmBlockReference* ref = static_cast<DmBlockReference*>(e);
            if (ref->getName() == m_blockName)
            {
                ref->update();
            }
        }
    }
}

// ============================================================================
// BlockEditExitCmd
// ============================================================================

BlockEditExitCmd::BlockEditExitCmd(DmDocument* doc, const QString& blockName,
                                   GuiDocumentView* docView, bool save)
    : m_pDocument(doc)
    , m_blockName(blockName)
    , m_pDocView(docView)
    , m_save(save)
{
}

void BlockEditExitCmd::execute()
{
    if (m_isExecuted)
    {
        return;
    }

    m_pDocument->setEditBlock(nullptr);

    if (m_save)
    {
        // setEditBlock(nullptr) has been called, so getEntityTable()
        // now returns the document's entity table
        updateBlockRefs();
    }

    m_pDocument->regenerate();

    m_isExecuted = true;
}

void BlockEditExitCmd::undo()
{
    if (!m_isExecuted)
    {
        return;
    }

    DmBlockTable* blockTable = m_pDocument->getBlockTable();
    if (!blockTable)
    {
        return;
    }

    DmBlock* block = blockTable->find(m_blockName);
    if (!block)
    {
        return;
    }

    m_pDocument->setEditBlock(block);
    m_pDocument->regenerate();

    m_isExecuted = false;
}

void BlockEditExitCmd::redo()
{
    if (m_isExecuted)
    {
        return;
    }

    m_pDocument->setEditBlock(nullptr);

    if (m_save)
    {
        updateBlockRefs();
    }

    m_pDocument->regenerate();

    m_isExecuted = true;
}

void BlockEditExitCmd::clear()
{
}

void BlockEditExitCmd::updateBlockRefs()
{
    auto entTable = m_pDocument->getEntityTable();
    for (auto e : *entTable)
    {
        if (e && !e->isErased()
            && e->getEntityType() == DM::EntityBlockReference)
        {
            DmBlockReference* ref = static_cast<DmBlockReference*>(e);
            if (ref->getName() == m_blockName)
            {
                ref->update();
            }
        }
    }
}
