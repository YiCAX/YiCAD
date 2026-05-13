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

/// @file MLeaderData.h
/// @brief 多重引线数据类

#ifndef MLEADERDATA_H
#define MLEADERDATA_H

#include "EntityData.h"
#include "DmVector.h"

/// @brief 多重引线
class MLeaderData : public EntityData
{
public:
    MLeaderData();

    /// @brief 获取多重引线顶点列表
    /// @return 每段线段的点集
    std::vector<std::vector<DmVector>> getVertexs() const;

    /// @brief 设置多重引线顶点列表
    /// @param [in] vertexs 每段线段的点集
    void setVertexs(const std::vector<std::vector<DmVector>>& vertexs);

    /// @brief 获取是否有箭头
    /// @return 是否有箭头
    bool getIsArrow() const;

    /// @brief 设置是否有箭头
    /// @param [in] isArrow 是否有箭头
    void setIsArrow(const bool& isArrow);

private:
    std::vector<std::vector<DmVector>> m_vecVertexs; ///< 多重引线跟转折点相连的末端的每条线段的点集
    bool                               m_isArrow;    ///< 是否有箭头
};

#endif // MLEADERDATA_H
