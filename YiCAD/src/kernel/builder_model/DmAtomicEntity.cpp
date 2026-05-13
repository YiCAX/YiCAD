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

/// @file DmAtomicEntity.cpp
/// @brief 原子实体基类的默认实现

#include "DmAtomicEntity.h"

TYPESYSTEM_SOURCE_ABSTRACT(DmAtomicEntity, DmEntity, 0)

DmAtomicEntity::DmAtomicEntity(DmEntity* parent)
    : DmEntity(parent)
{
}

bool DmAtomicEntity::isContainer() const
{
    return false;
}

DmVector DmAtomicEntity::getEndpoint() const
{
    return DmVector(false);
}

DmVector DmAtomicEntity::getStartpoint() const
{
    return DmVector(false);
}

double DmAtomicEntity::getDirection1() const
{
    return 0.0;
}

double DmAtomicEntity::getDirection2() const
{
    return 0.0;
}

DmVector DmAtomicEntity::getCenter() const
{
    return DmVector(false);
}

double DmAtomicEntity::getRadius() const
{
    return 0.;
}

DmVector DmAtomicEntity::getNearestCenter(const DmVector& /*coord*/, double* /*dist*/) const
{
    return DmVector(false);
}

void DmAtomicEntity::setStartpointSelected(bool select)
{
    if (select)
    {
        setFlag(DM::FlagSelected1);
    }
    else
    {
        delFlag(DM::FlagSelected1);
    }
}

void DmAtomicEntity::setEndpointSelected(bool select)
{
    if (select)
    {
        setFlag(DM::FlagSelected2);
    }
    else
    {
        delFlag(DM::FlagSelected2);
    }
}

bool DmAtomicEntity::isTangent(const CircleData& /* circleData */) const
{
    return false;
}

bool DmAtomicEntity::isStartpointSelected() const
{
    return getFlag(DM::FlagSelected1);
}

bool DmAtomicEntity::isEndpointSelected() const
{
    return getFlag(DM::FlagSelected2);
}

bool DmAtomicEntity::offset(const DmVector& /*position*/, const double& /*distance*/)
{
    return false;
}

void DmAtomicEntity::moveStartpoint(const DmVector& /*pos*/)
{
}

void DmAtomicEntity::moveEndpoint(const DmVector& /*pos*/)
{
}

void DmAtomicEntity::trimStartpoint(const DmVector& pos)
{
    moveStartpoint(pos);
}

void DmAtomicEntity::trimEndpoint(const DmVector& pos)
{
    moveEndpoint(pos);
}

DM::Ending DmAtomicEntity::getTrimPoint(const DmVector& /*coord*/, const DmVector& /*trimPoint*/)
{
    return DM::EndingNone;
}

void DmAtomicEntity::moveSelectedRef(const DmVector& ref, const DmVector& offset)
{
    if (isSelected())
    {
        moveRef(ref, offset);
    }
}

void DmAtomicEntity::saveStream(OutputStream& wrt) const
{
    DmEntity::saveStream(wrt);
}

void DmAtomicEntity::restoreStream(InputStream& reader, const std::vector<PAIR>& revs)
{
    int fileRev = getRevisionId("DmAtomicEntity", revs);
    if (revId > fileRev)
    {
        DmEntity::restoreStream(reader, revs);
        // 老文件格式
        restoreStreamWithRev(reader, fileRev);
    }
    else
    {
        // 新版本格式，在restoreStream中处理
    }
}

void DmAtomicEntity::restoreStreamWithRev(InputStream& rdr, int rev)
{
    DmEntity::restoreStreamWithRev(rdr, rev);
}

void DmAtomicEntity::restoreStream(InputStream& rdr)
{
    DmEntity::restoreStream(rdr);
}
