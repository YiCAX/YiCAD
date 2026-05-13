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

/// @file ArcData.cpp
/// @brief 圆弧数据结构类实现

#include "ArcData.h"
#include "Math2d.h"

/// @brief 默认构造函数
ArcData::ArcData()
    : EntityData()
    , m_ptCenter(DmVector(false))
    , m_dRadius(0.0)
    , m_dStartAngle(0.0)
    , m_dEndAngle(0.0)
    , m_normal(0.0, 0.0, 1.0)
{
    setEntityType(EEntityType::eArc);
}

/// @brief 带参数构造函数
/// @param center 圆心坐标
/// @param normal 圆弧平面法向量
/// @param radius 半径
/// @param startAngle 起始角度（弧度）
/// @param endAngle 终止角度（弧度）
ArcData::ArcData(const DmVector& center, const DmVector& normal, const double& radius, const double& startAngle, const double& endAngle)
    : EntityData()
    , m_ptCenter(center)
    , m_normal(normal)
    , m_dRadius(radius)
    , m_dStartAngle(startAngle)
    , m_dEndAngle(endAngle)
{
    setEntityType(EEntityType::eArc);
}

DmVector ArcData::getCenter() const
{
    return m_ptCenter;
}

void ArcData::setCenter(const DmVector& center)
{
    m_ptCenter = center;
}

double ArcData::getRadius() const
{
    return m_dRadius;
}

void ArcData::setRadius(const double& dRadius)
{
    m_dRadius = dRadius;
}

double ArcData::getStartAngle() const
{
    return m_dStartAngle;
}

void ArcData::setStartAngle(const double& dStartAngle)
{
    m_dStartAngle = dStartAngle;
}

double ArcData::getEndAngle() const
{
    return m_dEndAngle;
}

void ArcData::setEndAngle(const double& dEndAngle)
{
    m_dEndAngle = dEndAngle;
}

DmVector ArcData::getNormal() const
{
    return m_normal;
}

void ArcData::setNormal(const DmVector& normal)
{
    m_normal = normal;
}

const std::vector<float>& ArcData::getVerticesRef() const
{
    return m_vertices;
}

void ArcData::setVertices(const std::vector<float>& vs)
{
    m_vertices = vs;
}

/// @brief 判断圆弧数据是否有效
/// @return 若圆心有效且半径大于容差且角度差大于容差则返回true
bool ArcData::isValid() const
{
    return (m_ptCenter.valid && m_dRadius > DM_TOLERANCE && fabs(remainder(m_dStartAngle - m_dEndAngle, 2. * M_PI)) > DM_TOLERANCE_ANGLE);
}
