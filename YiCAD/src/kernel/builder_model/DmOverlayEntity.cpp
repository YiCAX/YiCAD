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

/// @file DmOverlayEntity.cpp
/// @brief 前景层覆盖实体实现：线、圆、点

#include "DmOverlayEntity.h"
#include "Tools.h"

DmOverlayLine::DmOverlayLine(DmEntity* parent, const LineData& d)
    : DmLine(parent, d)
{
}

DM::EntityType DmOverlayLine::getEntityType() const
{
    return DM::EntityOverlayLine;
}

std::list<DmEntity*> DmOverlayLine::getSubEntities() const
{
    return std::list<DmEntity*>();
}

DmOverlayCircle::DmOverlayCircle(DmEntity* parent, const CircleData& d)
    : DmCircle(parent, d)
{
}

DM::EntityType DmOverlayCircle::getEntityType() const
{
    return DM::EntityOverlayCircle;
}

std::list<DmEntity*> DmOverlayCircle::getSubEntities() const
{
    return std::list<DmEntity*>();
}

DmOverlayPoint::DmOverlayPoint(DmEntity* parent, const PointData& d)
    : DmPoint(parent, d)
{
}

DM::EntityType DmOverlayPoint::getEntityType() const
{
    return DM::EntityOverlayPoint;
}

std::list<DmEntity*> DmOverlayPoint::getSubEntities() const
{
    return std::list<DmEntity*>();
}
