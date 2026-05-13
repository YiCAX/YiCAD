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

/// @file MLeaderData.cpp
/// @brief 多重引线数据类实现

#include "MLeaderData.h"

MLeaderData::MLeaderData()
    : EntityData()
    , m_vecVertexs(std::vector<std::vector<DmVector>>())
    , m_isArrow(true)
{
    setEntityType(EEntityType::eMLeader);
}

std::vector<std::vector<DmVector>> MLeaderData::getVertexs() const
{
    return m_vecVertexs;
}

void MLeaderData::setVertexs(const std::vector<std::vector<DmVector>>& vertexs)
{
    m_vecVertexs = vertexs;
}

bool MLeaderData::getIsArrow() const
{
    return m_isArrow;
}

void MLeaderData::setIsArrow(const bool& isArrow)
{
    m_isArrow = isArrow;
}
