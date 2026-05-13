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

/// @file XLineData.cpp
/// @brief 构造线(双向射线)数据类实现

#include "XLineData.h"

XLineData::XLineData()
    : EntityData()
    , m_ptBasePoint(DmVector(0, 0, 0))
    , m_ptDirection(DmVector(0, 0, 0))
{
    setEntityType(EEntityType::eXLine);
}

XLineData::XLineData(const DmVector& startPt, const DmVector& dir)
    : EntityData()
    , m_ptBasePoint(startPt)
    , m_ptDirection(dir)
{
    setEntityType(EEntityType::eXLine);
}

DmVector XLineData::getBasePoint() const
{
    return m_ptBasePoint;
}

void XLineData::setBasePoint(const DmVector& pt)
{
    m_ptBasePoint = pt;
}

DmVector XLineData::getDirection() const
{
    return m_ptDirection;
}

void XLineData::setDirection(const DmVector& pt)
{
    m_ptDirection = pt;
}
