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

/// @file DmOverlayEntity.h
/// @brief 前景层覆盖实体：线、圆、点的叠加显示类型

#ifndef DMOVERLAYENTITY_H
#define DMOVERLAYENTITY_H

#include "DmLine.h"
#include "DmCircle.h"
#include "DmPoint.h"

/// @brief 前景层画线
class DmOverlayLine : public DmLine
{
public:
    /// @brief 构造函数
    /// @param parent 父实体指针
    /// @param d 线段几何数据
    DmOverlayLine(DmEntity* parent, const LineData& d);

    /// @brief 获取实体类型
    /// @return 实体类型枚举值
    DM::EntityType getEntityType() const override;

    /// @brief 获取子实体列表
    /// @return 子实体列表
    std::list<DmEntity*> getSubEntities() const override;
};

/// @brief 前景层画圆
class DmOverlayCircle : public DmCircle
{
public:
    /// @brief 构造函数
    /// @param parent 父实体指针
    /// @param d 圆几何数据
    DmOverlayCircle(DmEntity* parent, const CircleData& d);

    /// @brief 获取实体类型
    /// @return 实体类型枚举值
    DM::EntityType getEntityType() const override;

    /// @brief 获取子实体列表
    /// @return 子实体列表
    std::list<DmEntity*> getSubEntities() const override;
};

/// @brief 前景层画点
class DmOverlayPoint : public DmPoint
{
public:
    /// @brief 构造函数
    /// @param parent 父实体指针
    /// @param d 点几何数据
    DmOverlayPoint(DmEntity* parent, const PointData& d);

    /// @brief 获取实体类型
    /// @return 实体类型枚举值
    DM::EntityType getEntityType() const override;

    /// @brief 获取子实体列表
    /// @return 子实体列表
    std::list<DmEntity*> getSubEntities() const override;
};

#endif
