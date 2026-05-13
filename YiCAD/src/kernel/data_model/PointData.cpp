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

/// @file PointData.cpp
/// @brief 点数据结构类实现

#include "PointData.h"

/// @brief 默认构造函数
PointData::PointData()
    : EntityData()
    , m_pPosition(0, 0, 0)
{
    setEntityType(EEntityType::ePoint);
}

/// @brief 带位置参数的构造函数
/// @param pos 点坐标
PointData::PointData(const DmVector& pos)
    : EntityData()
    , m_pPosition(pos)
{
    // TODO:  - 此构造函数未设置实体类型，应添加 setEntityType(EEntityType::ePoint)
}

DmVector PointData::getPosition() const
{
    return m_pPosition;
}

void PointData::setPosition(const DmVector& pt)
{
    m_pPosition = pt;
}
