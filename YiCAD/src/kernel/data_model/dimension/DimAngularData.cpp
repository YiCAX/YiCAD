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

/// @file DimAngularData.cpp
/// @brief 角度标注(两线)数据类实现

#include "DimAngularData.h"

DimAngularData::DimAngularData()
    : DimensionData()
    , m_ptxLine1Start(DmVector(0, 0, 0))
    , m_ptxLine1End(DmVector(0, 0, 0))
    , m_ptxLine2Start(DmVector(0, 0, 0))
    , m_ptxLine2End(DmVector(0, 0, 0))
{
    setEntityType(EEntityType::eDimangular);
}

DmVector DimAngularData::getXLine1Start() const
{
    return m_ptxLine1Start;
}

void DimAngularData::setXLine1Start(const DmVector& pt)
{
    m_ptxLine1Start = pt;
}

DmVector DimAngularData::getXLine1End() const
{
    return m_ptxLine1End;
}

void DimAngularData::setXLine1End(const DmVector& pt)
{
    m_ptxLine1End = pt;
}

DmVector DimAngularData::getXLine2Start() const
{
    return m_ptxLine2Start;
}

void DimAngularData::setXLine2Start(const DmVector& pt)
{
    m_ptxLine2Start = pt;
}

DmVector DimAngularData::getXLine2End() const
{
    return m_ptxLine2End;
}

void DimAngularData::setXLine2End(const DmVector& pt)
{
    m_ptxLine2End = pt;
}

DmVector DimAngularData::getArcPoint() const
{
    return m_ptArcPoint;
}

void DimAngularData::setArcPoint(const DmVector& pt)
{
    m_ptArcPoint = pt;
}
