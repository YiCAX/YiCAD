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

/// @file RegionData.cpp
/// @brief 面域数据类实现

#include "RegionData.h"

#include <utility>

RegionData::RegionData()
{
    m_boundary = std::make_shared<DmEntityContainer>(nullptr);
    setEntityType(EEntityType::eRegion);
}

RegionData::RegionData(DmEntityContainerPtr boundary, const std::vector<DmEntityContainerPtr>& holes)
    : m_boundary(boundary)
    , m_holes(holes)
{
    setEntityType(EEntityType::eRegion);
}

DmEntityContainerPtr RegionData::getBoundary() const
{
    return m_boundary;
}

void RegionData::setBoundary(DmEntityContainerPtr b)
{
    m_boundary = b;
}

std::vector<DmEntityContainerPtr> RegionData::getHoles() const
{
    return m_holes;
}

void RegionData::setHoles(const std::vector<DmEntityContainerPtr>& holes)
{
    m_holes = holes;
}