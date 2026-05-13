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

/// @file DmBoundingBox.h
/// @brief 包围框类，用于表示二维轴对齐包围框

#ifndef DMBOUNDINGBOX_H
#define DMBOUNDINGBOX_H

#include "DmVector.h"

class DmBoundingBox
{
public:
    DmBoundingBox() = default;

    /// @brief 通过两个点构造包围框
    /// @param pt1 第一个点
    /// @param pt2 第二个点
    DmBoundingBox(const DmVector& pt1, const DmVector& pt2);

    /// @brief 添加一个点到包围框中，扩展包围框以包含该点
    /// @param pt 要添加的点
    void add(const DmVector& pt);

    /// @brief 判断是否完全包含另一个包围框，边界重合也算包含
    /// @param box 要检查的包围框
    /// @return 如果完全包含则返回true
    bool contains(const DmBoundingBox& box) const;

    /// @brief 判断是否与另一个包围框相交，边界重合算相交
    /// @param box 要检查的包围框
    /// @return 如果相交则返回true
    bool isCross(const DmBoundingBox& box) const;

    /// @brief 判断点是否在包围框内，边界上算内部
    /// @param pt 要检查的点
    /// @return 如果点在包围框内则返回true
    bool isPointInside(const DmVector& pt) const;

    /// @brief 获取包围框的最小点坐标
    /// @return 最小点坐标
    DmVector getMin() const;

    /// @brief 获取包围框的最大点坐标
    /// @return 最大点坐标
    DmVector getMax() const;

protected:
    DmVector min{false}; ///< 包围框最小点
    DmVector max{false}; ///< 包围框最大点
};

#endif //DMBOUNDINGBOX_H
