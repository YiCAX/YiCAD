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

/// @file TriangleData.cpp
/// @brief 三角形数据类实现

#include "TriangleData.h"

TriangleData::TriangleData(const DmVector& p0, const DmVector& p1, const DmVector& p2)
    : m_pts{p0, p1, p2}
{
}

std::array<DmVector, 3> TriangleData::getPoints() const
{
    return m_pts;
}

void TriangleData::setPoints(const std::array<DmVector, 3>& pts)
{
    m_pts = pts;
}

DmVector TriangleData::getPointAt(int i) const
{
    return m_pts[i];
}

void TriangleData::setPointAt(int i, const DmVector& pt)
{
    m_pts[i] = pt;
}
