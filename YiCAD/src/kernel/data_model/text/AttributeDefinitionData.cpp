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

/// @file AttributeDefinitionData.cpp
/// @brief 属性定义数据类实现

#include "AttributeDefinitionData.h"

AttributeDefinitionData::AttributeDefinitionData()
    : m_strTag("")
    , m_strPrompt("")
{
    setEntityType(EEntityType::eAttributeDefinition);
}

AttributeDefinitionData::AttributeDefinitionData(const QString& tag, const QString& prompt)
    : m_strTag(tag)
    , m_strPrompt(prompt)
{
    setEntityType(EEntityType::eAttributeDefinition);
}

QString AttributeDefinitionData::getTag() const
{
    return m_strTag;
}

void AttributeDefinitionData::setTag(const QString& tag)
{
    m_strTag = tag;
}

QString AttributeDefinitionData::getPrompt() const
{
    return m_strPrompt;
}

void AttributeDefinitionData::setPrompt(const QString& prompt)
{
    m_strPrompt = prompt;
}
