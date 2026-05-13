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

/// @file DimensionData.cpp
/// @brief 标注基类数据实现

#include "DimensionData.h"

DimensionData::DimensionData()
    : EntityData()
    , m_ptTextPoint(DmVector(0, 0, 0))
    , m_strTextString(L"")
    , m_strDimStyle(L"")
    , m_dTextLineFactor(1.)
    , m_eTextAlign(EAttachmentPoint::kBaseAlign)
{
    setEntityType(EEntityType::eDimension);
}

DmVector DimensionData::getTextPoint() const
{
    return m_ptTextPoint;
}

void DimensionData::setTextPoint(const DmVector& textPoint)
{
    m_ptTextPoint = textPoint;
}

std::wstring DimensionData::getTextString() const
{
    return m_strTextString;
}

void DimensionData::setTextString(const std::wstring& textString)
{
    m_strTextString = textString;
}

std::wstring DimensionData::getDimStyle() const
{
    return m_strDimStyle;
}

void DimensionData::setDimStyle(const std::wstring& strStyle)
{
    m_strDimStyle = strStyle;
}

double DimensionData::getTextLineFactor() const
{
    return m_dTextLineFactor;
}

void DimensionData::setTextLineFactor(const double& dTextLineFactor)
{
    m_dTextLineFactor = dTextLineFactor;
}

EAttachmentPoint DimensionData::getTextAlign() const
{
    return m_eTextAlign;
}

void DimensionData::setTextAlign(const EAttachmentPoint& align)
{
    m_eTextAlign = align;
}
