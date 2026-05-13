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

/// @file DimLinearData.cpp
/// @brief 线性标注数据类实现

#include "DimLinearData.h"

DimLinearData::DimLinearData()
    : DimensionData()
    , m_dAngle(0)
    , m_ptXLine1Point(DmVector(0, 0, 0))
    , m_ptXLine2Point(DmVector(0, 0, 0))
    , m_ptMidLinePoint(DmVector(0, 0, 0))
    , m_dOblique(0)
{
    setEntityType(EEntityType::eDimlinear);
}

double DimLinearData::getAngle() const
{
    return m_dAngle;
}

void DimLinearData::setAngle(const double& dAngle)
{
    m_dAngle = dAngle;
}

DmVector DimLinearData::getXLine1Point() const
{
    return m_ptXLine1Point;
}

void DimLinearData::setXLine1Point(const DmVector& pt)
{
    m_ptXLine1Point = pt;
}

DmVector DimLinearData::getXLine2Point() const
{
    return m_ptXLine2Point;
}

void DimLinearData::setXLine2Point(const DmVector& pt)
{
    m_ptXLine2Point = pt;
}

DmVector DimLinearData::getMidLinePoint() const
{
    return m_ptMidLinePoint;
}

void DimLinearData::setMidLinePoint(const DmVector& pt)
{
    m_ptMidLinePoint = pt;
}

double DimLinearData::getOblique() const
{
    return m_dOblique;
}

void DimLinearData::setOblique(const double& dOblique)
{
    m_dOblique = dOblique;
}
