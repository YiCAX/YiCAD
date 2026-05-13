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

/// @file DmPoint.cpp
/// @brief 点实体类实现：几何变换、最近点计算、持久化

#include "DmPoint.h"

#include <iostream>
#include <cmath>

#include "DmCircle.h"
#include "GuiDocumentView.h"

#include "Writer.h"
#include "Reader.h"
#include "Stream.h"

TYPESYSTEM_SOURCE(DmPoint, DmAtomicEntity, 0)

DmPoint::DmPoint(DmEntity* parent, const PointData& d)
    : DmAtomicEntity(parent)
    , data(d)
    , isModify(true)
{
    calculateBorders();
}

DmEntity* DmPoint::clone() const
{
    DmPoint* p = new DmPoint(*this);
    p->m_ulID = DmId();
    p->setSelected(false);
    p->setHighlighted(false);
    return p;
}

DM::EntityType DmPoint::getEntityType() const
{
    return DM::EntityPoint;
}

void DmPoint::calculateBorders()
{
    minV = maxV = data.getPosition();
}

std::list<DmEntity*> DmPoint::getSubEntities() const
{
    return std::list<DmEntity*>();
}

DmVectorSolutions DmPoint::getRefPoints() const
{
    return DmVectorSolutions{ data.getPosition() };
}

DmVector DmPoint::getStartpoint() const
{
    return data.getPosition();
}

DmVector DmPoint::getEndpoint() const
{
    return data.getPosition();
}

PointData DmPoint::getData() const
{
    return data;
}

DmVector DmPoint::getPos() const
{
    return data.getPosition();
}

DmVector DmPoint::getCenter() const
{
    return data.getPosition();
}

double DmPoint::getRadius() const
{
    return 0.0;
}

bool DmPoint::isTangent(const CircleData& circleData) const
{
    double const dist =
        data.getPosition().distanceTo(circleData.getCenter());
    return fabs(dist - fabs(circleData.getRadius()))
           < 50.0 * DM_TOLERANCE;
}

void DmPoint::setPos(const DmVector& pos)
{
    data.setPosition(pos);
}

DmVector DmPoint::getNearestEndpoint(const DmVector& coord,
                                     double* dist) const
{
    if (dist)
    {
        *dist = data.getPosition().distanceTo(coord);
    }

    return data.getPosition();
}

DmVector DmPoint::getNearestPointOnEntity(const DmVector& coord,
                                          bool /*onEntity*/,
                                          double* dist,
                                          DmEntity** entity) const
{
    if (dist)
    {
        *dist = data.getPosition().distanceTo(coord);
    }

    if (entity)
    {
        *entity = const_cast<DmPoint*>(this);
    }

    return data.getPosition();
}

DmVector DmPoint::getNearestCenter(const DmVector& coord,
                                   double* dist) const
{
    if (dist)
    {
        *dist = data.getPosition().distanceTo(coord);
    }

    return data.getPosition();
}

DmVector DmPoint::getMiddlePoint() const
{
    return data.getPosition();
}

DmVector DmPoint::getNearestMiddle(const DmVector& coord,
                                   double* dist,
                                   const int /*middlePoints*/) const
{
    if (dist)
    {
        *dist = data.getPosition().distanceTo(coord);
    }

    return data.getPosition();
}

double DmPoint::getDistanceToPoint(const DmVector& coord,
                                   DmEntity** entity,
                                   DM::ResolveLevel /*level*/) const
{
    if (entity)
    {
        *entity = const_cast<DmPoint*>(this);
    }

    return data.getPosition().distanceTo(coord);
}

void DmPoint::moveStartpoint(const DmVector& pos)
{
    data.setPosition(pos);
    isModify = true;
    calculateBorders();
}

void DmPoint::move(const DmVector& offset)
{
    data.setPosition(getPos().move(offset));
    isModify = true;
    calculateBorders();
}

void DmPoint::rotate(const DmVector& center, const DmVector& angleVector)
{
    data.setPosition(getPos().rotate(center, angleVector));
    isModify = true;
    calculateBorders();
}

void DmPoint::scale(const DmVector& center, const DmVector& factor)
{
    data.setPosition(getPos().scale(center, factor));
    isModify = true;
    calculateBorders();
}

void DmPoint::mirror(const DmVector& axisPoint1,
                     const DmVector& axisPoint2)
{
    data.setPosition(getPos().mirror(axisPoint1, axisPoint2));
    isModify = true;
    calculateBorders();
}

void DmPoint::saveStream(OutputStream& wrt) const
{
    DmAtomicEntity::saveStream(wrt);

    auto p = getPos();

    wrt << (double)p.x << (double)p.y;
}

void DmPoint::restoreStream(InputStream& reader,
                            const std::vector<PAIR>& revs)
{
    DmAtomicEntity::restoreStream(reader, revs);

    int fileRev = getRevisionId("DmPoint", revs);
    if (revId > fileRev)
    {
        // 老文件格式
        restoreStreamWithRev(reader, fileRev);
    }
    else
    {
        DmVector p(true);
        reader >> (double&)p.x >> (double&)p.y;

        setPos(p);
        isModify = true;
    }
}

void DmPoint::restoreStreamWithRev(InputStream& rdr, int rev)
{
    if (rev == 0)
    {
    }
    else // big change, e.g. change supper class of DmPoint
    {
        // step1.
        // read all legacy data one by one
    }
}
