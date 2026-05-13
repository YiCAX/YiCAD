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

/// @file SpacialSearchTree.h
/// @brief 空间搜索树类，采用rtree实现。关于rtree与四叉树的区别，参考：https://qa.1r1g.com/sf/ask/1625138301/

#ifndef SPACIALSEARCHTREE_H
#define SPACIALSEARCHTREE_H

#include <unordered_map>
#include "DmVector.h"
#include "DmId.h"

class SpacialSearchTreePrivate;
class DmEntity;
class DmBlockReference;

/// @brief 搜索树包围框
struct SearchTreeBoundingBox
{
    DmVector min;
    DmVector max;

    /// @brief 构造函数
    /// @param [in] min 最小点
    /// @param [in] max 最大点
    SearchTreeBoundingBox(const DmVector& min, const DmVector& max);

    /// @brief 相等比较
    /// @param [in] box 另一个包围框
    /// @return 相等返回true
    bool operator==(const SearchTreeBoundingBox& box) const;
};

/// @brief 空间搜索树。采用rtree。
class SpacialSearchTree
{
public:
    SpacialSearchTree();
    ~SpacialSearchTree();

    /// @brief 插入实体
    /// @param [in] entity 要插入的实体
    void insert(DmEntity* entity);

    /// @brief 移除实体
    /// @param [in] entity 要移除的实体
    void remove(DmEntity* entity);

    /// @brief 更新实体
    /// @param [in] entity 要更新的实体
    void update(DmEntity* entity);

    /// @brief 搜索指定范围内的实体
    /// @param [in] min 搜索范围最小点
    /// @param [in] max 搜索范围最大点
    /// @param [out] ents 搜索结果实体列表
    void search(const DmVector& min, const DmVector& max, std::vector<DmEntity*>& ents);

    /// @brief 清空树
    void clear();

private:
    /// @brief 递归获取块参照中的实体
    /// @param [in] blkRef 块参照
    /// @param [out] ents 实体列表
    void getEntitiesOfBlockReferenceRecursive(DmBlockReference* blkRef, std::vector<DmEntity*>& ents);

private:
    SpacialSearchTreePrivate* m_pTreePrivate;                    ///< 空间树私有实现
    std::unordered_map<DmId, SearchTreeBoundingBox> m_searchTreeBoundingBoxes;  ///< 保存空间树的实体及包围框
};

#endif //SPACIALSEARCHTREE_H
