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

/// @file AttributeData.cpp
/// @brief 属性数据类实现

#include "AttributeData.h"

AttributeData::AttributeData()
    : m_strTag("")
{
    setEntityType(EEntityType::eAttribute);
}

AttributeData::AttributeData(const QString& tag)
    : m_strTag(tag)
{
    setEntityType(EEntityType::eAttribute);
}

QString AttributeData::getTag() const
{
    return m_strTag;
}

void AttributeData::setTag(const QString& tag)
{
    m_strTag = tag;
}
