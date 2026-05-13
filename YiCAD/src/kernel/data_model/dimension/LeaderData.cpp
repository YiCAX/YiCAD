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

/// @file LeaderData.cpp
/// @brief 引线数据类实现

#include "LeaderData.h"

LeaderData::LeaderData()
    : EntityData()
    , m_vecVertexs(std::vector<DmVector>())
    , m_isArrow(true)
{
    setEntityType(EEntityType::eLeader);
}

std::vector<DmVector> LeaderData::getVertexs() const
{
    return m_vecVertexs;
}

void LeaderData::setVertexs(const std::vector<DmVector>& vertexs)
{
    m_vecVertexs = vertexs;
}

bool LeaderData::getIsArrow() const
{
    return m_isArrow;
}

void LeaderData::setIsArrow(const bool& isArrow)
{
    m_isArrow = isArrow;
}
