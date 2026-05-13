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

/// @file EntityTable.h
/// @brief 实体表，管理文档中所有 CAD 实体

#ifndef ENTITYTABLE_H
#define ENTITYTABLE_H

#include <vector>
#include "DmId.h"
#include "DmConstructionLine.h"
#include "DmEntityContainer.h"
#include "Information.h"
#include "SpacialSearchTree.h"
#include "TableBase.h"
#include <unordered_map>

class DmDocument;

/// @brief 实体表
class EntityTable : public ITable
{
public:
    using iterator = FilterIterator<std::vector<DmEntity*>::iterator>;
    using const_iterator = FilterIterator<std::vector<DmEntity*>::const_iterator>;
public:
    EntityTable();
    ~EntityTable();

    /// @brief 添加实体
    void add(DmEntity* e);
    /// @brief 通过 id 移除实体
    void remove(DmId id);
    /// @brief 移除实体
    void remove(DmEntity* e);
    /// @brief 通过 id 查找实体
    DmEntity* find(DmId id);
    /// @brief 表中不存在该实体，直接添加
    bool add_direct(DmEntity* e);
    /// @brief 直接删除从表中实体
    bool remove_direct(DmEntity* e);
    /// @brief 直接清空所有实体
    void clear_direct();
    void startModify(DmObject* e) override;
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    /// @brief 检查是否有选中的实体
    bool hasSelect() const;
    /// @brief 获取选中实体数量
    int countSelect() const;
    /// @brief 获得实体数（不含已删除）
    int count() const;
    /// @brief 获得选中实体中最近的拖拽点
    DmVector getNearestSelectedRef(const DmVector& coord, double* dist = nullptr) const;

    /// @brief 查找从指定坐标沿指定角度方向延伸的虚拟构造线与最近实体的交点
    DmVector getNearestVirtualIntersection(const DmVector& coord, const double& angle, double* dist);

    /// @brief 更新实体容器（用于渲染）
    void updateContainer();
    /// @brief 获取实体容器
    DmEntityContainer* getEntityContainer() {return &m_entContainer;}
    /// @brief 搜索包围框与指定区域有重叠的实体
    void searchEntities(const DmVector& min, const DmVector& max, std::vector<DmEntity*>& ents, bool onlyVisible = true, bool searchSubEnts = true);
    /// @brief 通知实体已修改，更新空间搜索树中的包围盒
    void notifyEntityModified(DmEntity* e) { m_searchTree.update(e); }
private:
    /// @brief 获得第一个未被删除的索引，没有则返回 -1
    int getFirstValidIndex() const;
    friend class EntityTableAddCmd;
    friend class EntityTableRemoveCmd;
    friend class EntityTableModifyCmd;
protected:
    DmEntityContainer m_entContainer;           ///< 用于渲染的实体容器（对实体没有控制权）
    std::unordered_map<DmId, DmEntity*> m_entMap;    ///< 所有实体字典（含已删除）
    std::vector<DmEntity*> m_ents;                  ///< 所有实体列表
private:
    SpacialSearchTree m_searchTree;                 ///< 空间搜索树
};

#endif //ENTITYTABLE_H
