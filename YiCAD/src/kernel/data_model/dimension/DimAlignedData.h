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

/// @file DimAlignedData.h
/// @brief 对齐标注数据类

#ifndef DIMALIGNEDDATA_H
#define DIMALIGNEDDATA_H

#include "DimensionData.h"
#include "DmVector.h"

/// @brief 对齐标注
class DimAlignedData : public DimensionData
{
public:
    DimAlignedData();

    /// @brief 获取旋转角度
    /// @return 旋转角度
    double getAngle() const;

    /// @brief 设置旋转角度
    /// @param [in] dAngle 旋转角度
    void setAngle(const double& dAngle);

    /// @brief 获取第一条尺寸界线起点
    /// @return 尺寸界线第一点坐标
    DmVector getXLine1Point() const;

    /// @brief 设置第一条尺寸界线起点
    /// @param [in] pt 坐标点
    void setXLine1Point(const DmVector& pt);

    /// @brief 获取第二条尺寸界线起点
    /// @return 尺寸界线第二点坐标
    DmVector getXLine2Point() const;

    /// @brief 设置第二条尺寸界线起点
    /// @param [in] pt 坐标点
    void setXLine2Point(const DmVector& pt);

    /// @brief 获取中心线点
    /// @return 中心线点坐标
    DmVector getMidLinePoint() const;

    /// @brief 设置中心线点
    /// @param [in] pt 坐标点
    void setMidLinePoint(const DmVector& pt);

    /// @brief 获取倾斜角度
    /// @return 倾斜角度
    double getOblique() const;

    /// @brief 设置倾斜角度
    /// @param [in] dOblique 倾斜角度
    void setOblique(const double& dOblique);

private:
    double   m_dAngle;         ///< 旋转角度
    DmVector m_ptXLine1Point;  ///< 第一点坐标
    DmVector m_ptXLine2Point;  ///< 第二点坐标
    DmVector m_ptMidLinePoint; ///< 中心线坐标
    double   m_dOblique;       ///< 标注图元的倾斜角度
};

#endif // DIMALIGNEDDATA_H
