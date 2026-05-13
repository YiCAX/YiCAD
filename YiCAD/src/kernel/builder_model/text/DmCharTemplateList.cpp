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

/// @file DmCharTemplateList.cpp
/// @brief 文字模板列表类实现

#include "DmCharTemplateList.h"
#include "DmCharTemplate.h"

DmCharTemplateList::~DmCharTemplateList()
{
    // 释放文字模板
    for (auto item : m_fontChars)
    {
        delete item.second;
        item.second = nullptr;
    }
}

void DmCharTemplateList::add(const QString& name, DmCharTemplate* letter)
{
    m_fontChars[name] = letter;
    letter->setOwner(this);
}

DmCharTemplate* DmCharTemplateList::find(const QString& name) const
{
    auto it = m_fontChars.find(name);
    if (it == m_fontChars.end())
    {
        return nullptr;
    }
    return it->second;
}

DmFont* DmCharTemplateList::getFont() const
{
    return m_pFont;
}

void DmCharTemplateList::setFont(DmFont* f)
{
    m_pFont = f;
}
