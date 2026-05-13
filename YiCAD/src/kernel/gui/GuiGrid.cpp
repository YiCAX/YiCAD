/*
 * Copyright (C) 2026 YiCAD Team
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

/// @file GuiGrid.cpp
/// @brief 栅格背景类实现

#include "GuiGrid.h"

#include<cmath>

#include "Debug.h"


GuiGrid::GuiGrid()
    : m_baseGrid(false)
{
}

/// @brief 获取最近的栅格交点
/// @param coord 当前鼠标点击的点
/// @return 最近的栅格交点坐标
DmVector GuiGrid::snapGrid(const DmVector& coord) const
{
    if (m_cellV.x < DM_TOLERANCE || m_cellV.y < DM_TOLERANCE)
    {
        return coord;
    }

    DmVector vp(coord - m_baseGrid);

    return m_baseGrid + vp - DmVector(remainder(vp.x, m_cellV.x), remainder(vp.y, m_cellV.y));
}

/// @brief 获取栅格交点集合
std::vector<DmVector> const& GuiGrid::getPoints() const
{
    return m_pts;
}

/// @brief 设置栅格交点集合
void GuiGrid::setPoints(const std::vector<DmVector>& points)
{
    m_pts.clear();
    m_pts = points;
}

/// @brief 获取网格单元向量
DmVector const& GuiGrid::getCellVector() const
{
    return m_cellV;
}

/// @brief 设置网格单元向量
void GuiGrid::setCellVector(const DmVector& cellVec)
{
    m_cellV = cellVec;
}

/// @brief 设置基准网格点
void GuiGrid::setBaseGrid(const DmVector& baseGrid)
{
    m_baseGrid = baseGrid;
}
