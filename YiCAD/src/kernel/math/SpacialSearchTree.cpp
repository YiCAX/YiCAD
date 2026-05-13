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

/// @file SpacialSearchTree.cpp
/// @brief 空间搜索树类实现

#include "SpacialSearchTree.h"
#include "RTree.h"
#include "DmEntity.h"
#include "DmBlockReference.h"

SearchTreeBoundingBox::SearchTreeBoundingBox(const DmVector& min, const DmVector& max)
    : min(min)
    , max(max)
{
}

bool SearchTreeBoundingBox::operator==(const SearchTreeBoundingBox& box) const
{
    return (min == box.min) && (max == box.max);
}

typedef RTree<DmEntity*, double, 2> SearchTree;
class SpacialSearchTreePrivate
{
public:
    SearchTree searchTree;
};

SpacialSearchTree::SpacialSearchTree()
    : m_pTreePrivate(nullptr)
{
    m_pTreePrivate = new SpacialSearchTreePrivate();
}

SpacialSearchTree::~SpacialSearchTree()
{
    if (m_pTreePrivate)
    {
        delete m_pTreePrivate;
        m_pTreePrivate = nullptr;
    }
}

void SpacialSearchTree::insert(DmEntity* entity)
{
    DmVector min = entity->getMin();
    DmVector max = entity->getMax();
    if (min.x > max.x || min.y > max.y) // 保证范围有效
    {
        return;
    }

    // 插入实体本身（包括块参照）
    double mind[2] = { min.x, min.y };
    double maxd[2] = { max.x, max.y };
    m_pTreePrivate->searchTree.Insert(mind, maxd, entity);
    if (entity->getId().isValid())
    {
        SearchTreeBoundingBox box(min, max);
        m_searchTreeBoundingBoxes.insert(std::make_pair(entity->getId(), box));
    }
}

void SpacialSearchTree::remove(DmEntity* entity)
{
    DmVector min = entity->getMin();
    DmVector max = entity->getMax();

    // 移除实体本身（包括块参照）
    double mind[2] = { min.x, min.y };
    double maxd[2] = { max.x, max.y };
    m_pTreePrivate->searchTree.Remove(mind, maxd, entity);
    if (entity->getId().isValid())
    {
        m_searchTreeBoundingBoxes.erase(entity->getId());
    }

    // 如果是块参照，也移除子实体
    auto entType = entity->getEntityType();
    if (entType == DM::EntityBlockReference)
    {
        DmBlockReference* blkRef = static_cast<DmBlockReference*>(entity);
        std::vector<DmEntity*> subEnts;
        getEntitiesOfBlockReferenceRecursive(blkRef, subEnts);
        for (auto e : subEnts)
        {
            remove(e);
        }
    }
}

void SpacialSearchTree::update(DmEntity* entity)
{
    if (!entity)
    {
        return;
    }
    auto id = entity->getId();
    if (!id.isValid())
    {
        return;
    }
    auto it = m_searchTreeBoundingBoxes.find(id);
    if (it == m_searchTreeBoundingBoxes.end())
    {
        return;
    }
    DmVector newMin = entity->getMin();
    DmVector newMax = entity->getMax();
    double newMinD[2] = { newMin.x, newMin.y };
    double newMaxD[2] = { newMax.x, newMax.y };
    SearchTreeBoundingBox newBox(newMin, newMax);
    if (!(it->second == newBox))
    {
        DmVector oldMin = it->second.min;
        DmVector oldMax = it->second.max;
        double oldMinD[2] = { oldMin.x, oldMin.y };
        double oldMaxD[2] = { oldMax.x, oldMax.y };
        m_pTreePrivate->searchTree.Remove(oldMinD, oldMaxD, entity);
        m_pTreePrivate->searchTree.Insert(newMinD, newMaxD, entity);
        it->second = newBox;
    }
}

void SpacialSearchTree::search(const DmVector& min, const DmVector& max, std::vector<DmEntity*>& ents)
{
    double mind[2] = { min.x, min.y };
    double maxd[2] = { max.x, max.y };
    auto callback = [&ents](DmEntity* ent)
    {
        ents.emplace_back(ent);
        return true;
    };
    m_pTreePrivate->searchTree.Search(mind, maxd, callback);
}

void SpacialSearchTree::getEntitiesOfBlockReferenceRecursive(DmBlockReference* blkRef, std::vector<DmEntity*>& ents)
{
    for (auto e : blkRef->getEntityList())
    {
        if (e->getEntityType() == DM::EntityBlockReference)
        {
            DmBlockReference* subBlkRef = static_cast<DmBlockReference*>(e);
            getEntitiesOfBlockReferenceRecursive(subBlkRef, ents);
        }
        else
        {
            ents.emplace_back(e);
        }
    }
}

void SpacialSearchTree::clear()
{
    m_pTreePrivate->searchTree.RemoveAll();
    m_searchTreeBoundingBoxes.clear();
}
