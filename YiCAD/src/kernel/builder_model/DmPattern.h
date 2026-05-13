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

/// @file DmPattern.h
/// @brief 填充图案类，管理填充图案的名称和几何数据

#ifndef DMPATTERN_H
#define DMPATTERN_H

#include <string>
#include <vector>
#include "Datamodel.h"
#include <memory>
#include "DmVector.h"

class DmPatternList;

/// @brief 填充图案类，存储图案名称和几何线段数据
class DmPattern
{
public:
    DmPattern();
    DmPattern(const DmPattern& pat);
    DmPattern(const std::wstring& fileName);
    virtual ~DmPattern() = default;

    /// @brief 获取实体类型
    /// @return 实体类型枚举值
    DM::EntityType getEntityType() const;

    /// @brief 获取图案名称
    /// @return 图案名称字符串
    std::wstring getPatternName() const;

    /// @brief 设置图案名称
    /// @param _name 新名称
    void setPatternName(std::wstring _name);

    /// @brief 获取图案几何数据
    /// @return 二维数据数组
    std::vector<std::vector<double>> getPatternData() const;

    /// @brief 设置图案几何数据
    /// @param _pats 新几何数据
    void setPatternData(std::vector<std::vector<double>> _pats);

    /// @brief 添加一条图案线段数据
    /// @param _pats 单条线段数据
    void addPattern(std::vector<double> _pats);

    /// @brief 获得图案的最大包围范围
    /// @param [out] min 最小坐标
    /// @param [out] max 最大坐标
    /// @return 成功返回 true
    bool getMaxRange(DmVector& min, DmVector& max) const;

    /// @brief 清空所有图案数据
    void clear();

    /// @brief 缩放图案
    /// @param d_scale 缩放比例
    void scale(double d_scale);

    /// @brief 旋转图案
    /// @param d_angle 旋转角度（度）
    void angle(double d_angle);

protected:
    std::wstring m_PatName;                     ///< 图案名称

    std::vector<std::vector<double>> m_pats;     ///< 图案几何线段数据
    bool loaded;                                 ///< 是否已加载到内存
};

using DmPatternPtr = std::shared_ptr<DmPattern>;

#endif
