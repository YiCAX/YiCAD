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

/// @file DmBlockTable.cpp
/// @brief 块表的实现，包括块的增删查改、激活、重命名等操作

#include "DmBlockTable.h"

#include <set>
#include <iostream>
#include <algorithm>
#include <QString>
#include <QRegExp>

#include "Debug.h"
#include "DmBlock.h"
#include "DmDocument.h"

DmBlockTable::DmBlockTable()
{
    m_pActiveBlock = nullptr;
}

void DmBlockTable::setDocument(DmDocument* pDoc)
{
    ITable::setDocument(pDoc);
}

void DmBlockTable::startModify(DmObject* e)
{
    DmBlock* block = static_cast<DmBlock*>(e);
    BlockTableModifyCmd* cmd = new BlockTableModifyCmd(this, block);
    m_pDoc->getCmdManager()->addToCurrentCmd(cmd);
}

// Removes all blocks in the blocktable.
void DmBlockTable::clear()
{
    m_blocks.clear();
    m_blockMap.clear();
    m_pActiveBlock = nullptr;
}

// Activates the given block. Listeners are notified.
void DmBlockTable::activate(const QString& name)
{
    activate(find(name));
}

// Activates the given block. Listeners are notified.
void DmBlockTable::activate(DmBlock* block)
{
    m_pActiveBlock = block;
}

void DmBlockTable::activate_direct(const QString& name)
{
    activate_direct(find(name));
}

void DmBlockTable::activate_direct(DmBlock* block)
{
    m_pActiveBlock = block;
}

/// @brief Adds a block to the block list. If a block with the same name  exists already, the given block will be deleted if the blockTable owns the blocks.
/// @param block 要添加的块
/// @param notify true if you want listeners to be notified.
/// @return false: block already existed and was deleted.
bool DmBlockTable::add(DmBlock* block, bool notify)
{
    if (!block)
    {
        return false;
    }

    if (!m_pDoc)
    {
        return false;
    }

    // check if block already exists:
    DmBlock* b = find(block->getName());
    if (b)
    {
        return false;
    }

    // 通过命令系统添加
    DmId id = block->getId();
    if (!id.isValid())
    {
        id = m_pDoc->getIdManager()->assignID(block);
    }
    BlockTableAddCmd* cmd = new BlockTableAddCmd(this, block);
    m_pDoc->getCmdManager()->addAndExecuteCmd(cmd);

    return true;
}

// Removes a block from the list.
// Listeners are notified after the block was removed from the list but before it gets deleted.
void DmBlockTable::remove(DmBlock* block)
{
    if (!block)
    {
        return;
    }

    if (!m_pDoc)
    {
        return;
    }

    // 通过命令系统标记为已删除
    BlockTableRemoveCmd* cmd = new BlockTableRemoveCmd(this, block);
    m_pDoc->getCmdManager()->addAndExecuteCmd(cmd);

    // activate an other block if necessary:
    if (m_pActiveBlock == block)
    {
        m_pActiveBlock = nullptr;
    }
}

void DmBlockTable::add_direct(DmBlock* block)
{
    m_blocks.emplace_back(block);
    m_blockMap[block->getId()] = block;
}

void DmBlockTable::remove_direct(DmBlock* block)
{
    auto it2 = std::find(m_blocks.begin(), m_blocks.end(), block);
    if (it2 != m_blocks.end())
    {
        m_blocks.erase(it2);
    }
    m_blockMap.erase(block->getId());
}

/// @brief Tries to rename the given block to 'name'. Block names are unique in the block list.
/// @param block 要重命名的块
/// @param name 新名称
/// @retval true block was successfully renamed.
/// @retval false block couldn't be renamed.
bool DmBlockTable::rename(DmBlock* block, const QString& name)
{
    if (block)
    {
        if (!find(name))
        {
            block->setName(name);
            return true;
        }
    }

    return false;
}

/// @return Pointer to the block with the given name or nullptr if no such block was found.
DmBlock* DmBlockTable::find(const QString& name)
{
    for (DmBlock* b : m_blocks)
    {
        if (b->isErased())
        {
            continue;
        }
        if (b->getName() == name)
        {
            return b;
        }
    }
    return nullptr;
}

DmBlock* DmBlockTable::find(const DmId& id)
{
    auto it = m_blockMap.find(id);
    if (it == m_blockMap.end())
    {
        return nullptr;
    }
    return it->second;
}

/// @brief Finds a new unique block name.
/// @param suggestion Suggested name the new name will be based on.
QString DmBlockTable::newName(const QString& suggestion)
{
    if (!find(suggestion))
    {
        return suggestion;
    }

    QString name = suggestion;
    QRegExp const rx(R"(-\d+$)");
    int index = name.lastIndexOf(rx);
    int i = -1;
    if (index > 0)
    {
        i = name.mid(index + 1).toInt();
        name = name.mid(0, index);
    }
    for (DmBlock* b : m_blocks)
    {
        if (b->isErased())
        {
            continue;
        }
        index = b->getName().lastIndexOf(rx);
        if (index < 0)
        {
            continue;
        }
        QString const part1 = b->getName().mid(0, index);
        if (part1 != name)
        {
            continue;
        }
        i = std::max(b->getName().mid(index + 1).toInt(), i);
    }
    return QString("%1-%2").arg(name).arg(i + 1);
}

// Switches on / off the given block.
// Listeners are notified.
void DmBlockTable::toggle(const QString& name)
{
    toggle(find(name));
}

// Switches on / off the given block.
// Listeners are notified.
void DmBlockTable::toggle(DmBlock* block)
{
    if (!block)
    {
        return;
    }

    block->toggle();
}

unsigned int DmBlockTable::count() const
{
    unsigned int size = 0;
    for (auto block : m_blocks)
    {
        if (!block->isErased())
        {
            size++;
        }
    }
    return size;
}

DmBlockTable::iterator DmBlockTable::begin()
{
    return DmBlockTable::iterator(m_blocks.begin(), m_blocks.end());
}

DmBlockTable::iterator DmBlockTable::end()
{
    return DmBlockTable::iterator(m_blocks.end());
}

/// @return The active block of nullptr if no block is activated.
DmBlock* DmBlockTable::getActive()
{
    return m_pActiveBlock;
}
