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

/// @file EntityData.h
/// @brief 实体数据基类，所有CAD实体数据结构的基类，包含实体类型标识

#ifndef ENTITYDATA_H
#define ENTITYDATA_H

#include <string>
#include <vector>

#include "EntityDataDef.h"

/// @brief 实体数据基类
class EntityData
{
public:
    /// @brief 默认构造函数
    EntityData();

    /// @brief 虚析构函数
    virtual ~EntityData() = default;

public:
    /// @brief 获取实体类型
    /// @return 实体类型枚举
    EEntityType getEntityType() const;

    /// @brief 设置实体类型
    /// @param entType 实体类型枚举
    void setEntityType(const EEntityType& entType);

private:
    EEntityType     m_eType;    ///< 实体类型
};

#endif // ENTITYDATA_H
