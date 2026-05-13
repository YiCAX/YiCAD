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

/// @file LineData.cpp
/// @brief 直线数据结构类实现

#include "LineData.h"

/// @brief 默认构造函数
LineData::LineData()
    : EntityData()
    , m_startPoint(DmVector(0, 0, 0))
    , m_endPoint(DmVector(0, 0, 0))
{
    setEntityType(EEntityType::eLine);
}

/// @brief 带起始终止点的构造函数
/// @param startPt 起点坐标
/// @param endPt 终点坐标
LineData::LineData(const DmVector& startPt, const DmVector& endPt)
    : EntityData()
    , m_startPoint(startPt)
    , m_endPoint(endPt)
{
    setEntityType(EEntityType::eLine);
}

DmVector LineData::getStartPoint() const
{
    return m_startPoint;
}

void LineData::setStartPoint(const DmVector& pt)
{
    m_startPoint = pt;
}

DmVector LineData::getEndPoint() const
{
    return m_endPoint;
}

void LineData::setEndPoint(const DmVector& pt)
{
    m_endPoint = pt;
}

const std::vector<float>& LineData::getVerticesRef() const
{
    return m_vertices;
}

void LineData::setVertices(const std::vector<float>& vs)
{
    m_vertices = vs;
}
