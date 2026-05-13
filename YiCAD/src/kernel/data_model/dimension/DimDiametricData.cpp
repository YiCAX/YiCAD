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

/// @file DimDiametricData.cpp
/// @brief 直径标注数据类实现

#include "DimDiametricData.h"

DimDiametricData::DimDiametricData()
    : DimensionData()
    , m_ptDiameter1Point(0, 0, 0)
    , m_ptDiameter2Point(0, 0, 0)
    , m_dLeaderLength(0)
{
    setEntityType(EEntityType::eDimdiametric);
}

DmVector DimDiametricData::getDiameter1Point() const
{
    return m_ptDiameter1Point;
}

void DimDiametricData::setDiameter1Point(const DmVector& pt)
{
    m_ptDiameter1Point = pt;
}

DmVector DimDiametricData::getDiameter2Point() const
{
    return m_ptDiameter2Point;
}

void DimDiametricData::setDiameter2Point(const DmVector& pt)
{
    m_ptDiameter2Point = pt;
}

double DimDiametricData::getLeaderLength() const
{
    return m_dLeaderLength;
}

void DimDiametricData::setLeaderLength(const double dLeaderLength)
{
    m_dLeaderLength = dLeaderLength;
}
