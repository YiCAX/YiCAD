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

/// @file LineStripData.cpp
/// @brief 线段串数据结构类实现

#include "LineStripData.h"

/// @brief 默认构造函数
LineStripData::LineStripData()
    : m_isClosed(false)
{
    setEntityType(EEntityType::eLineStrip);
}

/// @brief 带顶点集合的构造函数
/// @param pts 顶点坐标列表
/// @param isClosed 是否闭合
LineStripData::LineStripData(const std::vector<DmVector>& pts, bool isClosed)
{
    m_points = pts;
    m_isClosed = isClosed;
    setEntityType(EEntityType::eLineStrip);
}

std::vector<DmVector> LineStripData::getPoints() const
{
    return m_points;
}

void LineStripData::setPoints(const std::vector<DmVector>& pts)
{
    m_points = pts;
}

void LineStripData::appendPoint(const DmVector& pt)
{
    m_points.emplace_back(pt);
}

int LineStripData::getPointCount() const
{
    return (int)m_points.size();
}

DmVector LineStripData::getPointAt(int i) const
{
    return m_points.at(i);
}

void LineStripData::setPointAt(int i, const DmVector& pt)
{
    m_points.at(i) = pt;
}

void LineStripData::clear()
{
    m_points.clear();
}

bool LineStripData::isClosed() const
{
    return m_isClosed;
}

void LineStripData::setIsClosed(bool isClosed)
{
    m_isClosed = isClosed;
}

const std::vector<float>& LineStripData::getVerticesRef() const
{
    return m_vertices;
}

void LineStripData::setVertices(const std::vector<float>& vs)
{
    m_vertices = vs;
}
