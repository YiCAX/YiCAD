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

/// @file LineData.h
/// @brief 直线数据结构类，定义直线的起点和终点

#ifndef LINEDATA_H
#define LINEDATA_H

#include "EntityData.h"
#include "DmVector.h"

/// @brief 直线数据结构
class LineData : public EntityData
{
public:
    /// @brief 默认构造函数
    LineData();

    /// @brief 带起始终止点的构造函数
    /// @param startPt 起点坐标
    /// @param endPt 终点坐标
    LineData(const DmVector& startPt, const DmVector& endPt);

public:
    /// @brief 获取起点
    /// @return 起点坐标
    DmVector getStartPoint() const;

    /// @brief 设置起点
    /// @param pt 起点坐标
    void setStartPoint(const DmVector& pt);

    /// @brief 获取终点
    /// @return 终点坐标
    DmVector getEndPoint() const;

    /// @brief 设置终点
    /// @param pt 终点坐标
    void setEndPoint(const DmVector& pt);

    /// @brief 获取渲染顶点数据引用
    /// @return 顶点数据常量引用
    const std::vector<float>& getVerticesRef() const;

    /// @brief 设置渲染顶点数据
    /// @param vs 顶点数据向量
    void setVertices(const std::vector<float>& vs);

private:
    DmVector            m_startPoint;   ///< 起点
    DmVector            m_endPoint;     ///< 终点

    std::vector<float>  m_vertices;     ///< 用于渲染的特定结构数据（x,y,z, parameter, total_length）
};

#endif // LINEDATA_H
