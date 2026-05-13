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

/// @file RegionData.h
/// @brief 面域数据类，包含边界和孔洞信息

#ifndef REGIONDATA_H
#define REGIONDATA_H

#include "EntityData.h"
#include "DmEntityContainer.h"

class RegionData : public EntityData
{
public:
    RegionData();

    /// @brief 带边界和孔洞的构造函数
    /// @param [in] boundary 面域边界
    /// @param [in] holes 面域孔洞列表
    RegionData(DmEntityContainerPtr boundary, const std::vector<DmEntityContainerPtr>& holes);

    /// @brief 获取面域边界
    /// @return 边界实体容器指针
    DmEntityContainerPtr getBoundary() const;

    /// @brief 设置面域边界
    /// @param [in] b 边界实体容器指针
    void setBoundary(DmEntityContainerPtr b);

    /// @brief 获取面域孔洞列表
    /// @return 孔洞实体容器指针向量
    std::vector<DmEntityContainerPtr> getHoles() const;

    /// @brief 设置面域孔洞列表
    /// @param [in] holes 孔洞实体容器指针向量
    void setHoles(const std::vector<DmEntityContainerPtr>& holes);

private:
    std::vector<DmEntityContainerPtr> m_holes; ///< 面域孔洞列表
    DmEntityContainerPtr m_boundary;           ///< 面域边界
};

#endif // REGIONDATA_H
