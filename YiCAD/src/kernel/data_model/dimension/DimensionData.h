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

/// @file DimensionData.h
/// @brief 标注基类数据

#ifndef DIMENSIONDATA_H
#define DIMENSIONDATA_H

#include "EntityData.h"
#include "DmVector.h"

/// @brief 标注基类
class DimensionData : public EntityData
{
public:
    DimensionData();

    /// @brief 获取文字定位点
    /// @return 文字定位点坐标
    DmVector getTextPoint() const;

    /// @brief 设置文字定位点
    /// @param [in] textPoint 文字定位点坐标
    void setTextPoint(const DmVector& textPoint);

    /// @brief 获取文字内容
    /// @return 文字内容
    std::wstring getTextString() const;

    /// @brief 设置文字内容
    /// @param [in] textString 文字内容
    void setTextString(const std::wstring& textString);

    /// @brief 获取标注样式名称
    /// @return 标注样式名称
    std::wstring getDimStyle() const;

    /// @brief 设置标注样式名称
    /// @param [in] strStyle 标注样式名称
    void setDimStyle(const std::wstring& strStyle);

    /// @brief 获取文字行间距因子
    /// @return 文字行间距因子
    double getTextLineFactor() const;

    /// @brief 设置文字行间距因子
    /// @param [in] dTextLineFactor 文字行间距因子
    void setTextLineFactor(const double& dTextLineFactor);

    /// @brief 获取文字对齐方式
    /// @return 文字对齐方式
    EAttachmentPoint getTextAlign() const;

    /// @brief 设置文字对齐方式
    /// @param [in] align 文字对齐方式
    void setTextAlign(const EAttachmentPoint& align);

private:
    DmVector         m_ptTextPoint;     ///< 文字定位点
    std::wstring     m_strTextString;   ///< 文字内容
    std::wstring     m_strDimStyle;     ///< 标注样式
    double           m_dTextLineFactor; ///< 文字放缩因子
    EAttachmentPoint m_eTextAlign;      ///< 文字对齐
};

#endif // DIMENSIONDATA_H
