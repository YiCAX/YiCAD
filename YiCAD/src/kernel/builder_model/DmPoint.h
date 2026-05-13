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

/// @file DmPoint.h
/// @brief 点实体类，表示二维点

#ifndef DMPOINT_H
#define DMPOINT_H

#include "DmAtomicEntity.h"
#include "PointData.h"

/// @brief 点实体，表示二维空间中的一个点
class DmPoint : public DmAtomicEntity
{
    TYPESYSTEM_HEADER();

public:
    DmPoint() = default;

    /// @brief 构造函数
    /// @param parent 父实体指针
    /// @param d 点几何数据
    DmPoint(DmEntity* parent, const PointData& d);

    DmEntity* clone() const override;

    /// @brief 获取实体类型
    /// @return 点实体类型
    DM::EntityType getEntityType() const override;

    DmVector getStartpoint() const override;
    DmVector getEndpoint() const override;

    void moveStartpoint(const DmVector& pos) override;

    /// @brief 获取点几何数据
    /// @return 点数据
    PointData getData() const;

    DmVectorSolutions getRefPoints() const override;

    /// @brief 获取点位置
    /// @return 位置向量
    DmVector getPos() const;

    /// @brief 设置点位置
    /// @param pos 新位置
    void setPos(const DmVector& pos);

    DmVector getCenter() const override;
    double getRadius() const override;
    bool isTangent(const CircleData& circleData) const override;

    DmVector getMiddlePoint(void) const override;
    DmVector getNearestEndpoint(const DmVector& coord,
                                double* dist = nullptr) const override;
    DmVector getNearestPointOnEntity(const DmVector& coord,
                                     bool onEntity = true,
                                     double* dist = nullptr,
                                     DmEntity** entity = nullptr) const override;
    DmVector getNearestCenter(const DmVector& coord,
                              double* dist = nullptr) const override;
    DmVector getNearestMiddle(const DmVector& coord,
                              double* dist = nullptr,
                              int middlePoints = 1) const override;
    double getDistanceToPoint(const DmVector& coord,
                              DmEntity** entity = nullptr,
                              DM::ResolveLevel level = DM::ResolveNone)
        const override;

    void move(const DmVector& offset) override;
    void rotate(const DmVector& center,
                const DmVector& angleVector) override;
    void scale(const DmVector& center,
               const DmVector& factor) override;
    void mirror(const DmVector& axisPoint1,
                const DmVector& axisPoint2) override;

    /// @brief 重新计算实体包围盒
    void calculateBorders() override;

    std::list<DmEntity*> getSubEntities() const override;

    // persistent helper
    virtual void saveStream(OutputStream& wrt) const override;
    virtual void restoreStream(InputStream& reader,
                               const std::vector<PAIR>& revs) override;
    virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;

protected:
    PointData data;         ///< 点几何数据

private:
    bool isModify;          ///< 是否已修改标记
};

#endif
