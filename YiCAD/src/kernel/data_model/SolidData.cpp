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

/// @file SolidData.cpp
/// @brief 二维填充实体数据类实现

#include "SolidData.h"

SolidData::SolidData()
    : EntityData()
{
    setEntityType(EEntityType::eSolid);
}

SolidData::SolidData(const std::vector<DmVector>& corners)
    : EntityData()
    , m_corners(corners)
{
    setEntityType(EEntityType::eSolid);
}

std::vector<DmVector> SolidData::getCorners() const
{
    return m_corners;
}

void SolidData::setCorners(const std::vector<DmVector>& corners)
{
    m_corners = corners;
}

DmVector SolidData::getCornerAt(const int& index) const
{
    if (index < m_corners.size())
    {
        return m_corners.at(index);
    }
    else
    {
        return DmVector(false);
    }
}

void SolidData::setCornerAt(const int& index, const DmVector& corner)
{
    if (index < m_corners.size())
    {
        m_corners.at(index) = corner;
    }
}

int SolidData::getCornerSize() const
{
    return static_cast<int>(m_corners.size());
}
