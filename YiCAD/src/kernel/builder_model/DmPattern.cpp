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

/// @file DmPattern.cpp
/// @brief 填充图案类实现：数据管理、缩放、旋转、范围计算

#include "DmPattern.h"
#include "DmVector.h"
#include "DmSystem.h"
#include "DmLayer.h"
#include "Math2d.h"
#include "Debug.h"

DmPattern::DmPattern()
    : m_PatName()
    , m_pats()
    , loaded(false)
{
}

DmPattern::DmPattern(const DmPattern& pat)
    : loaded(false)
{
    m_PatName = pat.getPatternName();
    m_pats = pat.getPatternData();
}

DmPattern::DmPattern(const std::wstring& fileName)
    : m_PatName(fileName)
    , m_pats()
    , loaded(false)
{
}

DM::EntityType DmPattern::getEntityType() const
{
    return DM::EntityPattern;
}

std::wstring DmPattern::getPatternName() const
{
    return m_PatName;
}

void DmPattern::setPatternName(std::wstring _name)
{
    m_PatName = _name;
}

std::vector<std::vector<double>> DmPattern::getPatternData() const
{
    return m_pats;
}

void DmPattern::setPatternData(std::vector<std::vector<double>> _pats)
{
    m_pats = _pats;
}

void DmPattern::addPattern(std::vector<double> _pats)
{
    m_pats.emplace_back(_pats);
}

bool DmPattern::getMaxRange(DmVector& min, DmVector& max) const
{
    if (m_pats.empty())
    {
        return false;
    }

    if (m_pats.at(0).size() < 7)
    {
        return false;
    }

    auto firstLine = m_pats.at(0);
    min = DmVector(firstLine.at(1), firstLine.at(2));
    max = DmVector(firstLine.at(3), firstLine.at(4));

    for (auto& line : m_pats)
    {
        DmVector tmpMin(line.at(1), line.at(2));
        DmVector tmpMax(line.at(3), line.at(4));
        min = DmVector::minimum(min, tmpMin);
        max = DmVector::maximum(max, tmpMax);
    }

    return true;
}

void DmPattern::clear()
{
    m_pats.clear();
}

void DmPattern::scale(double d_scale)
{
    if (fabs(d_scale) > DM_TOLERANCE)
    {
        // 变换数据
        for (int i = 0; i < m_pats.size(); i++)
        {
            // 跳过第一个数据，角度不变
            for (int j = 1; j < m_pats[i].size(); j++)
            {
                m_pats.at(i).at(j) = m_pats.at(i).at(j) * d_scale;
            }
        }
    }
}

void DmPattern::angle(double d_angle)
{
    double r_angle = Math2d::deg2rad(d_angle);

    // 变换数据
    for (int i = 0; i < m_pats.size(); i++)
    {
        auto& line = m_pats.at(i);
        line.at(0) += r_angle;
        double x = line.at(1);
        double y = line.at(2);
        DmVector pt(x, y);
        pt.rotate(r_angle);
        line.at(1) = pt.x;
        line.at(2) = pt.y;
    }
}
