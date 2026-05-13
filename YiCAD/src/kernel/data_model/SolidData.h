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

/// @file SolidData.h
/// @brief 二维填充实体数据类

#ifndef SOLIDDATA_H
#define SOLIDDATA_H

#include <array>

#include "EntityData.h"
#include "DmVector.h"

class SolidData : public EntityData
{
public:
    SolidData();

    /// @brief 使用顶点列表构造填充实体
    /// @param [in] corners 顶点列表
    SolidData(const std::vector<DmVector>& corners);

    /// @brief 获取填充实体顶点列表
    /// @return 顶点向量
    std::vector<DmVector> getCorners() const;

    /// @brief 设置填充实体顶点列表
    /// @param [in] corners 顶点向量
    void setCorners(const std::vector<DmVector>& corners);

    /// @brief 获取指定索引位置的顶点
    /// @param [in] index 顶点索引
    /// @return 顶点坐标
    DmVector getCornerAt(const int& index) const;

    /// @brief 设置指定索引位置的顶点
    /// @param [in] index 顶点索引
    /// @param [in] corner 顶点坐标
    void setCornerAt(const int& index, const DmVector& corner);

    /// @brief 获取顶点数量
    /// @return 顶点数量
    int getCornerSize() const;

private:
    std::vector<DmVector> m_corners; ///< 构成实体的顶点，至少3个顶点，构成方式为GL_TRIANGLE_FAN
};

#endif