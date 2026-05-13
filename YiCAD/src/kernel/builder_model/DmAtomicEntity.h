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

/// @file DmAtomicEntity.h
/// @brief 原子实体基类，所有不可分解的实体（线、弧、圆等）的抽象基类

#ifndef DMATOMICENTITY_H
#define DMATOMICENTITY_H

#include "DmEntity.h"

class CircleData;

class DmAtomicEntity : public DmEntity
{
    TYPESYSTEM_HEADER();

public:
    DmAtomicEntity(DmEntity* parent = nullptr);

    /// @brief 实体是否为容器
    bool isContainer() const override;

    DmVector getEndpoint() const override;
    DmVector getStartpoint() const override;

    double getDirection1() const override;
    double getDirection2() const override;

    DmVector getCenter() const override;
    double getRadius() const override;

    DmVector getNearestCenter(const DmVector& /*coord*/, double* /*dist*/) const override;

    virtual void setStartpointSelected(bool select);
    virtual void setEndpointSelected(bool select);
    virtual bool isTangent(const CircleData& /* circleData */) const;

    bool isStartpointSelected() const;
    bool isEndpointSelected() const;

    bool offset(const DmVector& /*position*/, const double& /*distance*/) override;

    virtual void moveStartpoint(const DmVector& /*pos*/);
    virtual void moveEndpoint(const DmVector& /*pos*/);

    virtual void trimStartpoint(const DmVector& pos);
    virtual void trimEndpoint(const DmVector& pos);

    virtual DM::Ending getTrimPoint(const DmVector& /*coord*/, const DmVector& /*trimPoint*/);

    void moveSelectedRef(const DmVector& ref, const DmVector& offset) override;

    virtual void saveStream(OutputStream& wrt) const override;
    virtual void restoreStream(InputStream& rdr, const std::vector<PAIR>& revs) override;
    virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;
};

#endif
