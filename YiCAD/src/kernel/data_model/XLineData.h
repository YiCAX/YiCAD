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

/// @file XLineData.h
/// @brief 构造线(双向射线)数据类

#ifndef XLINEDATA_H
#define XLINEDATA_H

#include "EntityData.h"
#include "DmVector.h"

/// @brief 构造线(双向射线)
class XLineData : public EntityData
{
public:
    XLineData();

    /// @brief 使用基点和方向构造
    /// @param [in] startPt 基点
    /// @param [in] dir 方向向量
    XLineData(const DmVector& startPt, const DmVector& dir);

    /// @brief 获取基点
    /// @return 基点坐标
    DmVector getBasePoint() const;

    /// @brief 设置基点
    /// @param [in] pt 基点坐标
    void setBasePoint(const DmVector& pt);

    /// @brief 获取方向向量
    /// @return 方向向量
    DmVector getDirection() const;

    /// @brief 设置方向向量
    /// @param [in] pt 方向向量
    void setDirection(const DmVector& pt);

private:
    DmVector m_ptBasePoint;  ///< 起点
    DmVector m_ptDirection;  ///< 方向点
};

#endif // XLINEDATA_H
