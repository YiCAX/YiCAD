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

/// @file BlockReferenceData.h
/// @brief 块参照数据结构类，定义块参照的位置、旋转、缩放等属性

#ifndef BLOCKREFERENCEDATA_H
#define BLOCKREFERENCEDATA_H

#include <list>

#include "EntityData.h"
#include "DmVector.h"

/// @brief 块参照数据结构
class BlockReferenceData : public EntityData
{
public:
    /// @brief 默认构造函数
    BlockReferenceData();

public:
    /// @brief 获取块表记录句柄
    /// @return 块表记录句柄
    int getBlockTableRecHandle() const;

    /// @brief 设置块表记录句柄
    /// @param handle 块表记录句柄
    void setBlockTableRecHHandle(const int handle);

    /// @brief 获取块名称
    /// @return 块名称（宽字符串）
    std::wstring getBlockName() const;

    /// @brief 设置块名称
    /// @param name 块名称（宽字符串）
    void setBlockName(const std::wstring name);

    /// @brief 获取法向量
    /// @return 法向量
    DmVector getNormal() const;

    /// @brief 设置法向量
    /// @param normal 法向量
    void setNormal(const DmVector& normal);

    /// @brief 获取定位点
    /// @return 定位点坐标
    DmVector getPosition() const;

    /// @brief 设置定位点
    /// @param position 定位点坐标
    void setPosition(const DmVector& position);

    /// @brief 获取旋转角度
    /// @return 旋转角度（弧度）
    double getRotationAngle() const;

    /// @brief 设置旋转角度
    /// @param angle 旋转角度（弧度）
    void setRotationAngle(const double angle);

    /// @brief 获取缩放比例
    /// @return 缩放向量
    DmVector getScale() const;

    /// @brief 设置缩放比例
    /// @param scale 缩放向量
    void setScale(const DmVector& scale);

private:
    int             m_BlockTableRecordHandle;   ///< 块表记录句柄
    std::wstring    m_strBlockName;             ///< 块名
    DmVector        m_normal;                   ///< 法向量
    DmVector        m_position;                 ///< 定位点
    double          m_dRotationAngle;           ///< 旋转角度
    DmVector        m_scale;                    ///< 放缩
};

#endif // BLOCKREFERENCEDATA_H
