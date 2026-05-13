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

/// @file DmIdManager.cpp
/// @brief 实体ID管理器实现，使用boost::uuid生成唯一ID

#include "DmIdManager.h"

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

DmIdManager::DmIdManager()
    : m_entitiesMap(ENTITIESIDMAP())
{
}

DmId DmIdManager::assignID(DmObject* pObj)
{
    DmId id;
    do
    {
        boost::uuids::uuid uid = boost::uuids::random_generator()();
        std::string uidStr = boost::uuids::to_string(uid);
        id = DmId(uidStr);
    }
    while (isExistGUID(id));

    m_entitiesMap[id] = pObj;
    pObj->setId(id);
    return id;
}

bool DmIdManager::isExistGUID(const DmId& id) const
{
    if (m_entitiesMap.find(id) != m_entitiesMap.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DmIdManager::removeID(const DmId& id)
{
    auto it = m_entitiesMap.find(id);
    if (it != m_entitiesMap.end())
    {
        m_entitiesMap.erase(it);
    }
}

DmObject* DmIdManager::getEntity(const DmId& id)
{
    auto it = m_entitiesMap.find(id);
    if (it != m_entitiesMap.end())
    {
        return m_entitiesMap[id];
    }

    return nullptr;
}

void DmIdManager::clear()
{
    m_entitiesMap.clear();
}

void DmIdManager::assignID(DmObject* pObj, const DmId& id)
{
    m_entitiesMap[id] = pObj;
    pObj->setId(id);
}
