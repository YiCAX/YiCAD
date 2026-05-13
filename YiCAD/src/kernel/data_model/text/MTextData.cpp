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

/// @file MTextData.cpp
/// @brief 多行文字数据类实现

#include "MTextData.h"
#include "TextConsts.h"

MTextData::MTextData()
    : EntityData()
    , m_strTextString("")
    , m_dAngle(0)
    , m_ptPosition(DmVector(0, 0, 0))
    , m_dLineSpacingFactor(1.0)
    , m_dLineSpace(3.5)
    , m_dCharHeight(2.5)
    , m_pTextStyle(nullptr)
    , m_dDefineWidth(1.0)
    , m_dDefineHeight(0.0)
    , m_eTextVertMode(EMTextVertMode::kTextBottom)
    , m_eTextHorzMode(EMTextHorzMode::kTextLeft)
    , m_updateMode(EUpdateMode::Update)
{
    setEntityType(EEntityType::eMText);
}

MTextData::MTextData(const DmVector& insertionPoint, double charHeight, EMTextVertMode valign, EMTextHorzMode halign
    , double lineSpace, double defineWidth, const QString& text, DmTextStyle* style, double angle, EUpdateMode updateMode)
    : EntityData()
    , m_strTextString(text)
    , m_dAngle(angle)
    , m_ptPosition(insertionPoint)
    , m_dLineSpacingFactor(1.0)
    , m_dLineSpace(lineSpace)
    , m_dCharHeight(charHeight)
    , m_pTextStyle(style)
    , m_dDefineWidth(defineWidth)
    , m_dDefineHeight(0.0)
    , m_eTextVertMode(valign)
    , m_eTextHorzMode(halign)
    , m_updateMode(updateMode)
{
    setEntityType(EEntityType::eMText);
}

QString MTextData::getTextString() const
{
    return m_strTextString;
}

void MTextData::setTextString(const QString& strTextString)
{
    m_strTextString = strTextString;
}

DmTextStyle* MTextData::getTextStyle() const
{
    return m_pTextStyle;
}

void MTextData::setTextStyle(DmTextStyle* pStyle)
{
    m_pTextStyle = pStyle;
}

double MTextData::getAngle() const
{
    return m_dAngle;
}

void MTextData::setAngle(const double& dAngle)
{
    m_dAngle = dAngle;
}

DmVector MTextData::getPosition() const
{
    return m_ptPosition;
}

void MTextData::setPosition(const DmVector& pt)
{
    m_ptPosition = pt;
}

EMTextHorzMode MTextData::getTextHorzMode() const
{
    return m_eTextHorzMode;
}

void MTextData::setTextHorzMode(EMTextHorzMode eTextHorzMode)
{
    m_eTextHorzMode = eTextHorzMode;
}

EMTextVertMode MTextData::getTextVertMode() const
{
    return m_eTextVertMode;
}

void MTextData::setTextVertMode(EMTextVertMode eTextVertMode)
{
    m_eTextVertMode = eTextVertMode;
}

double MTextData::getLineSpacingFactor() const
{
    return m_dLineSpacingFactor;
}

void MTextData::setLineSpacingFactor(const double& dLineSpacingFactor)
{
    m_dLineSpacingFactor = dLineSpacingFactor;
    m_dLineSpace = m_dLineSpacingFactor * m_dCharHeight * LINE_HEIGHT_PER_CHAR_HEIGHT;
}

double MTextData::getLineSpace() const
{
    return m_dLineSpace;
}

void MTextData::setLineSpace(const double& dLineSpace)
{
    m_dLineSpace = dLineSpace;
    m_dLineSpacingFactor = m_dLineSpace / (m_dCharHeight * LINE_HEIGHT_PER_CHAR_HEIGHT);
}

double MTextData::getCharHeight() const
{
    return m_dCharHeight;
}

void MTextData::setCharHeight(double height)
{
    m_dCharHeight = height;
    m_dLineSpace = m_dLineSpacingFactor * m_dCharHeight * LINE_HEIGHT_PER_CHAR_HEIGHT;
}

EMTextMode MTextData::getJustification() const
{
    return m_eJustification;
}

void MTextData::setJustification(EMTextMode justification)
{
    m_eJustification = justification;
}

double MTextData::getDefineWidth() const
{
    return m_dDefineWidth;
}

void MTextData::setDefineWidth(double width)
{
    m_dDefineWidth = width;
}

double MTextData::getDefineHeight() const
{
    return m_dDefineHeight;
}

void MTextData::setDefineHeight(double height)
{
    m_dDefineHeight = height;
}

EUpdateMode MTextData::getUpdateMode() const
{
    return m_updateMode;
}

void MTextData::setUpdateMode(EUpdateMode updateMode)
{
    m_updateMode = updateMode;
}
