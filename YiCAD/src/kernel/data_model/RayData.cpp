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

/// @file RayData.cpp
/// @brief 射线数据结构类实现

#include "RayData.h"

/// @brief 默认构造函数
RayData::RayData()
    : EntityData()
    , m_ptBasePoint(DmVector(0, 0, 0))
    , m_ptDirection(DmVector(0, 0, 0))
{
    setEntityType(EEntityType::eRay);
}

/// @brief 带起始终止点的构造函数
/// @param startPt 起点坐标
/// @param dir 方向向量
RayData::RayData(const DmVector& startPt, const DmVector& dir)
    : EntityData()
    , m_ptBasePoint(startPt)
    , m_ptDirection(dir)
{
    setEntityType(EEntityType::eRay);
}

DmVector RayData::getBasePoint() const
{
    return m_ptBasePoint;
}

void RayData::setBasePoint(const DmVector& pt)
{
    m_ptBasePoint = pt;
}

DmVector RayData::getDirection() const
{
    return m_ptDirection;
}

void RayData::setDirection(const DmVector& pt)
{
    m_ptDirection = pt;
}
