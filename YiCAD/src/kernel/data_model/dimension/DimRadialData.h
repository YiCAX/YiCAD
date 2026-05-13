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

/// @file DimRadialData.h
/// @brief 半径标注数据类

#ifndef DIMRADIALDATA_H
#define DIMRADIALDATA_H

#include "DimensionData.h"
#include "DmVector.h"

/// @brief 半径标注
class DimRadialData : public DimensionData
{
public:
    DimRadialData();

    /// @brief 获取圆心点
    /// @return 圆心点坐标
    DmVector getCenterPoint() const;

    /// @brief 设置圆心点
    /// @param [in] pt 坐标点
    void setCenterPoint(const DmVector& pt);

    /// @brief 获取自定义位置点
    /// @return 自定义位置点坐标
    DmVector getDiameterPoint() const;

    /// @brief 设置自定义位置点
    /// @param [in] pt 坐标点
    void setDiameterPoint(const DmVector& pt);

    /// @brief 获取引线长度
    /// @return 引线长度
    double getLeaderLength() const;

    /// @brief 设置引线长度
    /// @param [in] length 引线长度
    void setLeaderLength(const double length);

private:
    DmVector m_ptCenterPoint;   ///< 中点
    DmVector m_ptDiameterPoint; ///< 自定义位置点
    double   m_dLeaderLength;   ///< 引线长度
};

#endif // DIMRADIALDATA_H
