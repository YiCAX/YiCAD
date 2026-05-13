/*
 * Copyright (C) 2026 YiCAD Team
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

/// @file DmLayerTable.h
/// @brief 图层表，管理文档中所有图层对象

#ifndef DMLAYERTABLE_H
#define DMLAYERTABLE_H

#include <vector>

#include "DmLayer.h"
#include "TableBase.h"

/// @brief 图层表
/// @details 用来维护文档里的所有图层对象
class DmLayerTable : public ITable
{
public:
    using iterator = FilterIterator<std::vector<DmLayer*>::iterator>;
public:
    DmLayerTable();
    virtual ~DmLayerTable();
    void setDocument(DmDocument *pDoc) override;

    void startModify(DmObject* e) override;
    /// @brief 添加图层
    void add(DmLayer* e);
    /// @brief 通过 id 移除图层
    void remove(DmId id);
    /// @brief 移除图层
    void remove(DmLayer* e);
    /// @brief 查找图层（不能获得已删除的图层）
    DmLayer* find(const QString& name);
    /// @brief 通过 id 查找图层，采用此方法可获得已删除的（未从内存删除）图层
    DmLayer* find(const DmId& id);
    /// @brief 表中不存在该实体，直接添加
    bool add_direct(DmLayer* e);
    /// @brief 直接删除从表中实体
    bool remove_direct(DmLayer* e);

    iterator begin();
    iterator end();

    unsigned int count() const;

    /// @brief 按名称激活图层
    void activate(const QString& name);
    /// @brief 激活图层
    void activate(DmLayer* layer);
    /// @brief 直接激活图层（不产生命令）
    void activate_direct(const QString& name);
    /// @brief 直接激活图层
    void activate_direct(DmLayer* layer);
    /// @brief 获取当前激活的图层
    DmLayer* getActive();

private:
    std::unordered_map<DmId, DmLayer*>  m_layerMap;     ///< 图层字典（包含已删除但未清理的图层）
    std::vector<DmLayer*>               m_layers;       ///< 图层列表（包含已删除但未清理的图层）
    DmLayer*                            m_pActiveLayer; ///< 当前激活的图层
};

#endif
