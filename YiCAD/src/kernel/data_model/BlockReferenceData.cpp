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

/// @file BlockReferenceData.cpp
/// @brief 块参照数据结构类实现

#include "BlockReferenceData.h"


/// @brief 默认构造函数
BlockReferenceData::BlockReferenceData()
    : EntityData()
    , m_BlockTableRecordHandle(-1)
    , m_strBlockName(L"*U")
    , m_normal(0.0, 0.0, 1.0)
    , m_position(0.0, 0.0, 0.0)
    , m_dRotationAngle(0.0)
    , m_scale(1.0, 1.0, 1.0)
{
    setEntityType(EEntityType::eBlock);
}

int BlockReferenceData::getBlockTableRecHandle() const
{
    return m_BlockTableRecordHandle;
}

void BlockReferenceData::setBlockTableRecHHandle(const int handle)
{
    m_BlockTableRecordHandle = handle;
}

std::wstring BlockReferenceData::getBlockName() const
{
    return m_strBlockName;
}

void BlockReferenceData::setBlockName(const std::wstring name)
{
    m_strBlockName = name;
}

DmVector BlockReferenceData::getNormal() const
{
    return m_normal;
}

void BlockReferenceData::setNormal(const DmVector& normal)
{
    m_normal = normal;
}

DmVector BlockReferenceData::getPosition() const
{
    return m_position;
}

void BlockReferenceData::setPosition(const DmVector& position)
{
    m_position = position;
}

double BlockReferenceData::getRotationAngle() const
{
    return m_dRotationAngle;
}

void BlockReferenceData::setRotationAngle(const double angle)
{
    m_dRotationAngle = angle;
}

DmVector BlockReferenceData::getScale() const
{
    return m_scale;
}

void BlockReferenceData::setScale(const DmVector& scale)
{
    m_scale = scale;
}
