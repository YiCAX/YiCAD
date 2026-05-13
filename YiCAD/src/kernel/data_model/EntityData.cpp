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

/// @file EntityData.cpp
/// @brief 实体数据基类实现

#include "EntityData.h"

/// @brief 默认构造函数，初始化为未知类型
EntityData::EntityData()
    : m_eType(EEntityType::eUnknown)
{
}

EEntityType EntityData::getEntityType() const
{
    return m_eType;
}

void EntityData::setEntityType(const EEntityType& entType)
{
    m_eType = entType;
}
