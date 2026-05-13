/*
 * Copyright (C) 2026 YiCAD Team
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

/// @file DmEntityHelper.h
/// @brief 实体工厂辅助类，通过类型名创建实体及获取实体类型名称

#ifndef DMENTITYHELPER_H
#define DMENTITYHELPER_H

#include "DmEntity.h"

class DmEntityHelper
{
public:
    /// @brief 根据实体类型名字符串创建对应的实体对象
    /// @param [in] typeName 实体类型名称，如 "Line", "Circle" 等
    /// @return 创建的实体指针，若类型名未匹配则返回 nullptr
    static DmEntity* createEntityByName(const std::string& typeName);

    /// @brief 根据实体类型枚举获取对应的名称字符串
    /// @param [in] entityType 实体类型枚举值
    /// @return 实体类型名称字符串，若未匹配则返回空字符串
    static std::string getEntityNameByType(DM::EntityType entityType);
};

#endif // DMENTITYHELPER_H
