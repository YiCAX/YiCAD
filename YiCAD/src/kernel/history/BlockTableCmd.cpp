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

/// @file BlockTableCmd.cpp
/// @brief 块表操作命令实现

#include "BlockTableCmd.h"
#include "DmBlockTable.h"
#include "DmBlock.h"
#include <sstream>

BlockTableAddCmd::BlockTableAddCmd(DmBlockTable *table, DmBlock *addedBlock)
: m_table(table)
, m_addedBlock(addedBlock)
, m_isOwnByCommand(false)
{

}

void BlockTableAddCmd::execute()
{
    if (m_isExecuted)
    {
        return;
    }
    auto e = m_table->find(m_addedBlock->getId());
    if (e == nullptr)
    {
        m_isOwnByCommand = true;
        m_table->add_direct(m_addedBlock);
    }
    else
    {
        m_isOwnByCommand = false;
        e->setErased(false);
    }
    ICmd::execute();
}

void BlockTableAddCmd::undo()
{
    if (!m_isExecuted)
    {
        return;
    }
    auto e = m_table->find(m_addedBlock->getId());
    if (e == nullptr)
    {
        return;
    }
    else
    {
        e->setErased(true);
    }
    ICmd::undo();
}

void BlockTableAddCmd::redo()
{
    if (m_isExecuted)
    {
        return;
    }
    auto e = m_table->find(m_addedBlock->getId());
    if (e == nullptr)
    {
        m_table->add_direct(m_addedBlock);
    }
    else
    {
        e->setErased(false);
    }
    ICmd::redo();
}

void BlockTableAddCmd::clear()
{
    auto block = m_table->find(m_addedBlock->getId());
    if (m_isOwnByCommand)
    {
        m_table->remove_direct(block);
    }
}

BlockTableRemoveCmd::BlockTableRemoveCmd(DmBlockTable *table, DmBlock *removedBlock)
: m_table(table)
, m_removedBlock(removedBlock)
{

}

void BlockTableRemoveCmd::execute()
{
    if (m_isExecuted)
    {
        return;
    }
    auto e = m_table->find(m_removedBlock->getId());
    if (e == nullptr)
    {
        return;
    }
    else
    {
        e->setErased(true);
    }
    ICmd::execute();
}

void BlockTableRemoveCmd::undo()
{
    if (!m_isExecuted)
    {
        return;
    }
    auto e = m_table->find(m_removedBlock->getId());
    if (e == nullptr)
    {
        return;
    }
    else
    {
        e->setErased(false);
    }
    ICmd::undo();
}

void BlockTableRemoveCmd::redo()
{
    if (m_isExecuted)
    {
        return;
    }
    execute();
    ICmd::redo();
}

void BlockTableRemoveCmd::clear()
{
}

BlockTableModifyCmd::BlockTableModifyCmd(DmBlockTable *table, DmBlock *modifiedBlock)
:m_table(table)
,m_modifiedBlock(modifiedBlock)
{
    std::ostringstream oss;
    OutputStream str(oss);
    m_modifiedBlock->saveStream(str);
    m_originData = oss.str();
}

void BlockTableModifyCmd::execute()
{
    if (m_isExecuted)
    {
        return;
    }
    std::ostringstream oss;
    OutputStream str(oss);
    m_modifiedBlock->saveStream(str);
    m_newData = oss.str();
    ICmd::execute();
}

void BlockTableModifyCmd::undo()
{
    if (!m_isExecuted)
    {
        return;
    }
    std::istringstream iss(m_originData);
    InputStream str(iss);
    std::vector<PAIR> revs;
    m_modifiedBlock->restoreStream(str, revs);
    m_modifiedBlock->getEntityTable().updateContainer();
    ICmd::undo();
}

void BlockTableModifyCmd::redo()
{
    if (m_isExecuted)
    {
        return;
    }
    std::istringstream iss(m_newData);
    InputStream str(iss);
    std::vector<PAIR> revs;
    m_modifiedBlock->restoreStream(str, revs);
    m_modifiedBlock->getEntityTable().updateContainer();
    ICmd::redo();
}

void BlockTableModifyCmd::clear()
{

}
