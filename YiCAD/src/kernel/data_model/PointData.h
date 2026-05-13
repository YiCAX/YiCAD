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

/// @file PointData.h
/// @brief 点数据结构类，定义点的位置坐标

#ifndef POINTDATA_H
#define POINTDATA_H

#include "EntityData.h"
#include "DmVector.h"

/// @brief 点数据结构
class PointData : public EntityData
{
public:
    /// @brief 默认构造函数
    PointData();

    /// @brief 带位置参数的构造函数
    /// @param pos 点坐标
    PointData(const DmVector& pos);

public:
    /// @brief 获取位置
    /// @return 点坐标
    DmVector getPosition() const;

    /// @brief 设置位置
    /// @param pt 点坐标
    void setPosition(const DmVector& pt);

private:
    DmVector    m_pPosition;    ///< 定位点坐标
};

#endif // POINTDATA_H
