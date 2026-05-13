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

/// @file LeaderData.h
/// @brief 引线数据类

#ifndef LEADERDATA_H
#define LEADERDATA_H

#include "EntityData.h"
#include "DmVector.h"

/// @brief 引线
class LeaderData : public EntityData
{
public:
    LeaderData();

    /// @brief 获取引线顶点列表
    /// @return 顶点向量
    std::vector<DmVector> getVertexs() const;

    /// @brief 设置引线顶点列表
    /// @param [in] vertexs 顶点向量
    void setVertexs(const std::vector<DmVector>& vertexs);

    /// @brief 获取是否有箭头
    /// @return 是否有箭头
    bool getIsArrow() const;

    /// @brief 设置是否有箭头
    /// @param [in] isArrow 是否有箭头
    void setIsArrow(const bool& isArrow);

private:
    std::vector<DmVector> m_vecVertexs; ///< 引线顶点列表
    bool                   m_isArrow;   ///< 是否有箭头
};

#endif // LEADERDATA_H
