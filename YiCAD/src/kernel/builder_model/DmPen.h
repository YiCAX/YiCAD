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

/// @file DmPen.h
/// @brief 画笔类，封装颜色、线宽、线型等绘制属性

#ifndef DMPEN_H
#define DMPEN_H

#include "Datamodel.h"
#include "DmColor.h"
#include "DmFlags.h"

#include "DmLineType.h"
#include "DmLineTypeTable.h"

/// @brief 画笔类，存储线宽、线型和颜色等绘制属性
class DmPen : public DmFlags
{
public:
    /// @brief 创建默认画笔（黑色、实线、线宽0）
    DmPen();

    /// @brief 创建指定属性的画笔
    /// @param c 颜色
    /// @param w 线宽
    /// @param t 线型指针
    DmPen(const DmColor& c, DM::LineWidth w, DmLineType* t);

    /// @brief 创建带标志位的默认画笔，通常用于创建无效画笔
    /// @param f 标志位
    DmPen(unsigned int f);

    virtual ~DmPen();

    /// @brief 获取线型
    /// @return 线型指针
    DmLineType* getLineType() const;

    /// @brief 设置线型
    /// @param t 线型指针
    void setLineType(DmLineType* t);

    /// @brief 获取线宽
    /// @return 线宽枚举值
    DM::LineWidth getWidth() const;

    /// @brief 设置线宽
    /// @param w 线宽枚举值
    void setWidth(DM::LineWidth w);

    /// @brief 获取颜色
    /// @return 颜色常量引用
    const DmColor& getColor() const;

    /// @brief 设置颜色
    /// @param c 颜色
    void setColor(const DmColor& c);

    /// @brief 计算颜色哈希值
    /// @return 哈希值
    size_t hashColor() const;

    /// @brief 判断画笔是否有效
    /// @return 有效返回 true
    bool isValid();

    bool operator==(const DmPen& p) const;
    bool operator!=(const DmPen& p) const;

protected:
    DM::LineWidth width;    ///< 线宽
    DmColor color;          ///< 颜色

    DmLineType* lineType;   ///< 线型指针
};

inline bool DmPen::operator==(const DmPen& p) const
{
    return (lineType == p.lineType && width == p.width && color == p.color);
}

inline bool DmPen::operator!=(const DmPen& p) const
{
    return !(*this == p);
}

inline DmLineType* DmPen::getLineType() const
{
    return lineType;
}

inline void DmPen::setLineType(DmLineType* t)
{
    lineType = t;
}

inline DM::LineWidth DmPen::getWidth() const
{
    return width;
}

inline void DmPen::setWidth(DM::LineWidth w)
{
    width = w;
}

inline const DmColor& DmPen::getColor() const
{
    return color;
}

inline void DmPen::setColor(const DmColor& c)
{
    color = c;
}

inline bool DmPen::isValid()
{
    return !getFlag(DM::FlagInvalid);
}

inline size_t DmPen::hashColor() const
{
    return std::hash<DmColor>()(color);
}

MAKE_HASHABLE(DmPen, t.getLineType(), t.getWidth(), t.hashColor());

#endif
