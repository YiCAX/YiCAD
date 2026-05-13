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

/// @file GuiGrid.h
/// @brief 栅格背景类，管理栅格交点和网格捕捉

#ifndef GUIGRID_H
#define GUIGRID_H

#include "DmVector.h"

/// @brief 栅格背景
class GuiGrid
{
public:
    GuiGrid();

    /// @brief 获取栅格交点集合
    /// @return 栅格交点集合的常量引用
    std::vector<DmVector> const& getPoints() const;
    /// @brief 设置栅格交点集合
    /// @param points 栅格交点集合
    void setPoints(const std::vector<DmVector>& points);

    /// @brief 获取最近的栅格交点
    /// @param coord 当前鼠标点击的点
    /// @return 最近的栅格交点坐标
    DmVector snapGrid(const DmVector& coord) const;

    /// @brief 获取网格单元向量
    /// @return 网格单元向量 (dx, dy)
    DmVector const& getCellVector() const;
    /// @brief 设置网格单元向量
    /// @param cellVec 网格单元向量 (dx, dy)
    void setCellVector(const DmVector& cellVec);

    /// @brief 设置基准网格点
    /// @param baseGrid 左下角的基准网格点
    void setBaseGrid(const DmVector& baseGrid);

private:
    std::vector<DmVector>   m_pts;          ///< 栅格交点集
    DmVector                m_cellV;        ///< 网格单元向量 (dx, dy)
    DmVector                m_baseGrid;     ///< 左下角基准网格点
};

#endif
