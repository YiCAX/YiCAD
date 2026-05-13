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

/// @file DmTriangle.h
/// @brief 三角形实体类，支持点判断、最近点计算等

#ifndef DMTRIANGLE_H
#define DMTRIANGLE_H

#include "DmEntity.h"
#include "TriangleData.h"

/// @brief 三角形实体
class DmTriangle : public DmEntity
{
    TYPESYSTEM_HEADER();
public:
    DmTriangle() = default;
    DmTriangle(DmEntity* parent, const TriangleData& d);

    DmEntity* clone() const override;
    DM::EntityType getEntityType() const override;
    TriangleData const& getData() const;
    void setData(const TriangleData& d);
    DmVector getPointAt(int i) const;
    DmVector getNearestEndpoint(const DmVector& coord, double* dist = nullptr) const override;
    DmVector getNearestPointOnEntity(const DmVector& coord, bool onEntity = true, double* dist = nullptr, DmEntity** entity = nullptr) const override;
    DmVector getNearestCenter(const DmVector& coord, double* dist = nullptr) const override;
    DmVector getNearestMiddle(const DmVector& coord, double* dist = nullptr, int middlePoints = 1) const override;
    double getDistanceToPoint(const DmVector& coord, DmEntity** entity = nullptr, DM::ResolveLevel level = DM::ResolveNone) const override;
    void move(const DmVector& offset) override;
    void rotate(const DmVector& center, const DmVector& angleVector) override;
    void scale(const DmVector& center, const DmVector& factor) override;
    void mirror(const DmVector& axisPoint1, const DmVector& axisPoint2) override;
    void calculateBorders() override;
    std::list<DmEntity*> getSubEntities() const override;

    bool isInCrossWindow(const DmVector& v1, const DmVector& v2) const;
    bool isPointInside(const DmVector& pt) const;
    bool isContainer() const override;

    // persistent helper
    virtual void saveStream(OutputStream& wrt) const override;
    virtual void restoreStream(InputStream& reader, const std::vector<PAIR>& revs) override;
    virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;

protected:
    TriangleData data;
    bool isModify;
    constexpr static int POINT_SIZE = 3; ///< 三角形顶点个数
};

using DmTrianglePtr = std::shared_ptr<DmTriangle>;

#endif //DMTRIANGLE_H
