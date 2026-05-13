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

/// @file DimRadialData.cpp
/// @brief 半径标注数据类实现

#include "DimRadialData.h"

DimRadialData::DimRadialData()
    : DimensionData()
    , m_ptCenterPoint(0, 0, 0)
    , m_ptDiameterPoint(0, 0, 0)
    , m_dLeaderLength(0)
{
    setEntityType(EEntityType::eDimradial);
}

DmVector DimRadialData::getCenterPoint() const
{
    return m_ptCenterPoint;
}

void DimRadialData::setCenterPoint(const DmVector& pt)
{
    m_ptCenterPoint = pt;
}

DmVector DimRadialData::getDiameterPoint() const
{
    return m_ptDiameterPoint;
}

void DimRadialData::setDiameterPoint(const DmVector& pt)
{
    m_ptDiameterPoint = pt;
}

double DimRadialData::getLeaderLength() const
{
    return m_dLeaderLength;
}

void DimRadialData::setLeaderLength(const double length)
{
    m_dLeaderLength = length;
}
