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

/// @file TextStyleTableCmd.cpp
/// @brief 文字样式表相关的 Undo/Redo 命令类实现

#include "TextStyleTableCmd.h"
#include "DmTextStyle.h"
#include "DmTextStyleTable.h"
#include <sstream>

TextStyleTableAddCmd::TextStyleTableAddCmd(DmTextStyleTable* table, DmTextStyle* addedTextStyle)
    : m_table(table)
    , m_addedTextStyle(addedTextStyle)
    , m_isOwnByCommand(false)
{
}

void TextStyleTableAddCmd::execute()
{
    if (m_isExecuted)
    {
        return;
    }

    auto e = m_table->find(m_addedTextStyle->getId());
    if (e == nullptr)
    {
        m_isOwnByCommand = true;
        m_table->add_direct(m_addedTextStyle);
    }
    else
    {
        m_isOwnByCommand = false;
        e->setErased(false);
    }
    ICmd::execute();
}

void TextStyleTableAddCmd::undo()
{
    if (!m_isExecuted)
    {
        return;
    }

    auto e = m_table->find(m_addedTextStyle->getId());
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

void TextStyleTableAddCmd::redo()
{
    if (m_isExecuted)
    {
        return;
    }

    auto e = m_table->find(m_addedTextStyle->getId());
    if (e != nullptr)
    {
        e->setErased(false);
    }
    else
    {
        // TODO: 实体已被删除，重做时需要特殊处理
    }
    ICmd::redo();
}

void TextStyleTableAddCmd::clear()
{
    auto layer = m_table->find(m_addedTextStyle->getId());
    if (m_isOwnByCommand)
    {
        m_table->remove_direct(layer);
    }
}

TextStyleTableModifyCmd::TextStyleTableModifyCmd(DmTextStyleTable* table, DmTextStyle* modifiedTextStyle)
    : m_table(table)
    , m_modifiedTextStyle(modifiedTextStyle)
{
    std::ostringstream oss;
    OutputStream str(oss);
    m_modifiedTextStyle->saveStream(str);
    m_originData = oss.str();
}

void TextStyleTableModifyCmd::execute()
{
    if (m_isExecuted)
    {
        return;
    }

    std::ostringstream oss;
    OutputStream str(oss);
    m_modifiedTextStyle->saveStream(str);
    m_newData = oss.str();
    m_modifiedTextStyle->update();
    ICmd::execute();
}

void TextStyleTableModifyCmd::undo()
{
    if (!m_isExecuted)
    {
        return;
    }

    std::istringstream iss(m_originData);
    InputStream str(iss);
    m_modifiedTextStyle->restoreStream(str);
    ICmd::undo();
}

void TextStyleTableModifyCmd::redo()
{
    if (m_isExecuted)
    {
        return;
    }

    std::istringstream iss(m_newData);
    InputStream str(iss);
    m_modifiedTextStyle->restoreStream(str);
    ICmd::redo();
}

void TextStyleTableModifyCmd::clear()
{
}

TextStyleTableRemoveCmd::TextStyleTableRemoveCmd(DmTextStyleTable* table, DmTextStyle* removedTextStyle)
    : m_table(table)
    , m_removedTextStyle(removedTextStyle)
{
}

void TextStyleTableRemoveCmd::execute()
{
    if (m_isExecuted)
    {
        return;
    }

    auto e = m_table->find(m_removedTextStyle->getId());
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

void TextStyleTableRemoveCmd::undo()
{
    if (!m_isExecuted)
    {
        return;
    }

    auto e = m_table->find(m_removedTextStyle->getId());
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

void TextStyleTableRemoveCmd::redo()
{
    if (m_isExecuted)
    {
        return;
    }

    execute();
    ICmd::redo();
}

void TextStyleTableRemoveCmd::clear()
{
}

TextStyleTableActivateCmd::TextStyleTableActivateCmd(DmTextStyleTable* table, DmTextStyle* activatedTextStyle)
    : m_table(table)
    , m_activatedTextStyle(activatedTextStyle)
{
    m_originActiveTextStyle = table->getActive();
}

void TextStyleTableActivateCmd::execute()
{
    if (m_isExecuted)
    {
        return;
    }

    auto e = m_table->find(m_activatedTextStyle->getId());
    if (e == nullptr)
    {
        return;
    }
    else
    {
        m_table->activate_direct(m_activatedTextStyle);
    }
    ICmd::execute();
}

void TextStyleTableActivateCmd::undo()
{
    if (!m_isExecuted)
    {
        return;
    }

    m_table->activate_direct(m_originActiveTextStyle);
    ICmd::undo();
}

void TextStyleTableActivateCmd::redo()
{
    if (m_isExecuted)
    {
        return;
    }

    m_table->activate_direct(m_activatedTextStyle);
    ICmd::redo();
}

void TextStyleTableActivateCmd::clear()
{
}
