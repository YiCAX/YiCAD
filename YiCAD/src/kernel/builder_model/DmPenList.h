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

/// @file DmPenList.h
/// @brief 画笔列表，单例模式管理画笔的创建与ID映射

#ifndef DMPENLIST_H
#define DMPENLIST_H

#include <unordered_map>
#include "DmPen.h"

#define DMPENLIST DmPenList::instance()

/// @brief 画笔列表（单例模式），管理画笔实例与ID的双向映射
class DmPenList
{
public:
    /// @brief 获取全局唯一实例
    /// @return 单例指针
    static DmPenList* instance();

    /// @brief 按属性请求画笔，不存在则创建
    /// @param color 颜色
    /// @param lineWidth 线宽
    /// @param lineType 线型指针
    /// @return 画笔指针
    DmPen* request(const DmColor& color, DM::LineWidth lineWidth,
                   DmLineType* lineType);

    /// @brief 按ID请求画笔
    /// @param penId 画笔ID
    /// @return 画笔指针，未找到返回 nullptr
    DmPen* request(int penId) const;

    /// @brief 获取画笔对应的ID
    /// @param pen 画笔引用
    /// @return 画笔ID，未找到返回 -1
    int getPenId(const DmPen& pen);

private:
    DmPenList() = default;

private:
    std::unordered_map<int, DmPen*> m_idPenMap;     ///< ID到画笔的映射
    std::unordered_map<DmPen, int> m_penIdMap;      ///< 画笔到ID的映射
};

#endif // DMPENLIST_H
