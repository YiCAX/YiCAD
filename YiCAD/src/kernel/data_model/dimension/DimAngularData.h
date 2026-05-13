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

/// @file DimAngularData.h
/// @brief 角度标注(两线)数据类

#ifndef DIMANGULARDATA_H
#define DIMANGULARDATA_H

#include "DimensionData.h"
#include "DmVector.h"

/// @brief 角度标注(两线)
class DimAngularData : public DimensionData
{
public:
    DimAngularData();

    /// @brief 获取第一条尺寸界线起点
    /// @return 第一条尺寸界线起点坐标
    DmVector getXLine1Start() const;

    /// @brief 设置第一条尺寸界线起点
    /// @param [in] pt 坐标点
    void setXLine1Start(const DmVector& pt);

    /// @brief 获取第一条尺寸界线终点
    /// @return 第一条尺寸界线终点坐标
    DmVector getXLine1End() const;

    /// @brief 设置第一条尺寸界线终点
    /// @param [in] pt 坐标点
    void setXLine1End(const DmVector& pt);

    /// @brief 获取第二条尺寸界线起点
    /// @return 第二条尺寸界线起点坐标
    DmVector getXLine2Start() const;

    /// @brief 设置第二条尺寸界线起点
    /// @param [in] pt 坐标点
    void setXLine2Start(const DmVector& pt);

    /// @brief 获取第二条尺寸界线终点
    /// @return 第二条尺寸界线终点坐标
    DmVector getXLine2End() const;

    /// @brief 设置第二条尺寸界线终点
    /// @param [in] pt 坐标点
    void setXLine2End(const DmVector& pt);

    /// @brief 获取弧上点
    /// @return 弧上点坐标
    DmVector getArcPoint() const;

    /// @brief 设置弧上点
    /// @param [in] pt 坐标点
    void setArcPoint(const DmVector& pt);

private:
    DmVector m_ptxLine1Start; ///< 第一条尺寸界线起点
    DmVector m_ptxLine1End;   ///< 第一条尺寸界线终点
    DmVector m_ptxLine2Start; ///< 第二条尺寸界线起点
    DmVector m_ptxLine2End;   ///< 第二条尺寸界线终点
    DmVector m_ptArcPoint;    ///< 弧上点
};

#endif // DIMANGULARDATA_H
