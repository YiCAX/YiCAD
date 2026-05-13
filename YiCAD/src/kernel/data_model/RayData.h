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

/// @file RayData.h
/// @brief 射线数据结构类，定义射线的起点和方向向量

#ifndef RAYDATA_H
#define RAYDATA_H

#include "EntityData.h"
#include "DmVector.h"

/// @brief 射线数据结构
class RayData : public EntityData
{
public:
    /// @brief 默认构造函数
    RayData();

    /// @brief 带起始终止点的构造函数
    /// @param startPt 起点坐标
    /// @param dir 方向向量（起点到方向点的向量）
    RayData(const DmVector& startPt, const DmVector& dir);

public:
    /// @brief 获取起点
    /// @return 起点坐标
    DmVector getBasePoint() const;

    /// @brief 设置起点
    /// @param pt 起点坐标
    void setBasePoint(const DmVector& pt);

    /// @brief 获取方向向量
    /// @return 方向向量
    DmVector getDirection() const;

    /// @brief 设置方向向量
    /// @param pt 方向向量
    void setDirection(const DmVector& pt);

private:
    DmVector    m_ptBasePoint;      ///< 起点
    DmVector    m_ptDirection;      ///< 起点到方向点的向量
};

#endif // RAYDATA_H
