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

/// @file DimDiametricData.h
/// @brief 直径标注数据类

#ifndef DIMDIAMETRICDATA_H
#define DIMDIAMETRICDATA_H

#include "DimensionData.h"
#include "DmVector.h"

/// @brief 直径标注
class DimDiametricData : public DimensionData
{
public:
    DimDiametricData();

    /// @brief 获取直径第一点
    /// @return 直径第一点坐标
    DmVector getDiameter1Point() const;

    /// @brief 设置直径第一点
    /// @param [in] pt 坐标点
    void setDiameter1Point(const DmVector& pt);

    /// @brief 获取直径第二点
    /// @return 直径第二点坐标
    DmVector getDiameter2Point() const;

    /// @brief 设置直径第二点
    /// @param [in] pt 坐标点
    void setDiameter2Point(const DmVector& pt);

    /// @brief 获取引线长度
    /// @return 引线长度
    double getLeaderLength() const;

    /// @brief 设置引线长度
    /// @param [in] dLeaderLength 引线长度
    void setLeaderLength(const double dLeaderLength);

private:
    DmVector m_ptDiameter1Point; ///< 直径第一点
    DmVector m_ptDiameter2Point; ///< 直径第二点
    double   m_dLeaderLength;    ///< 引线长度
};

#endif // DIMDIAMETRICDATA_H
