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

/// @file DmPenList.cpp
/// @brief 画笔列表实现：单例、画笔创建与ID双向映射

#include "DmPenList.h"
#include <memory>

/// @brief 获取全局唯一画笔列表实例
/// @return 单例指针
DmPenList* DmPenList::instance()
{
    static DmPenList* inst = nullptr;
    if (inst == nullptr)
    {
        inst = new DmPenList();
    }

    return inst;
}

/// @brief 按属性请求画笔，不存在则创建并分配ID
/// @param color 颜色
/// @param lineWidth 线宽
/// @param lineType 线型指针
/// @return 画笔指针
DmPen* DmPenList::request(const DmColor& color, DM::LineWidth lineWidth,
                          DmLineType* lineType)
{
    DmPen pen(color, lineWidth, lineType);
    auto it = m_penIdMap.find(pen);
    if (it == m_penIdMap.end())
    {
        static int id = 0;
        id++;
        m_penIdMap[pen] = id;
        DmPen* thePen = new DmPen(pen);
        m_idPenMap[id] = thePen;
        return thePen;
    }
    else
    {
        return m_idPenMap[it->second];
    }
}

/// @brief 按ID请求画笔
/// @param penId 画笔ID
/// @return 画笔指针，未找到返回 nullptr
DmPen* DmPenList::request(int penId) const
{
    auto it = m_idPenMap.find(penId);
    if (it == m_idPenMap.end())
    {
        return nullptr;
    }

    return it->second;
}

/// @brief 获取画笔对应的ID
/// @param pen 画笔引用
/// @return 画笔ID，未找到返回 -1
int DmPenList::getPenId(const DmPen& pen)
{
    auto it = m_penIdMap.find(pen);
    if (it != m_penIdMap.end())
    {
        return it->second;
    }

    return -1;
}
