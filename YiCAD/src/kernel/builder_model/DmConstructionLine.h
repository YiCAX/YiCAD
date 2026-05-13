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

/// @file DmConstructionLine.h
/// @brief 构造线实体类，表示两端无限延伸的直线

#ifndef DMCONSTRUCTIONLINE_H
#define DMCONSTRUCTIONLINE_H

#include "DmAtomicEntity.h"
#include "DmVector.h"

// Holds the data that defines a construction line (a line which is not limited to both directions).
class DmConstructionLineData
{
public:
    DmConstructionLineData();
    DmConstructionLineData(const DmVector& point1, const DmVector& point2);

    friend class DmConstructionLine;

private:
    DmVector point1{false}; ///< 第一个定义点
    DmVector point2{false}; ///< 第二个定义点
};

// Class for a construction line entity.
class DmConstructionLine : public DmAtomicEntity
{
public:
    DmConstructionLine() = default;

    /// @brief 通过父实体和构造线数据构造
    /// @param parent 父实体指针
    /// @param d 构造线数据
    DmConstructionLine(DmEntity* parent, const DmConstructionLineData& d);

    virtual DmEntity* clone() const;

    virtual ~DmConstructionLine() = default;

    /// @return DM::EntityConstructionLine
    virtual DM::EntityType getEntityType() const;

    /// @brief 获取构造线的定义数据
    /// @return 构造线数据的常量引用
    DmConstructionLineData const& getData() const;

    /// @brief 获取第一个定义点
    /// @return 第一个定义点的常量引用
    DmVector const& getPoint1() const;

    /// @brief 获取第二个定义点
    /// @return 第二个定义点的常量引用
    DmVector const& getPoint2() const;

    /// @brief 获取实体起点
    /// @return 起点坐标
    DmVector getStartpoint() const override;

    /// @brief 获取实体终点
    /// @return 终点坐标
    DmVector getEndpoint() const override;
    double getDirection1(void) const override;
    double getDirection2(void) const override;

    virtual Quadratic getQuadratic() const;
    virtual DmVector getMiddlePoint(void) const;
    virtual DmVector getNearestEndpoint(const DmVector& coord, double* dist = NULL) const;
    virtual DmVector getNearestPointOnEntity(const DmVector& coord, bool onEntity = true, double* dist = NULL, DmEntity** entity = NULL) const;
    virtual DmVector getNearestCenter(const DmVector& coord, double* dist = NULL) const;
    virtual DmVector getNearestMiddle(const DmVector& coord, double* dist = NULL, int middlePoints = 1) const;
    virtual double getDistanceToPoint(const DmVector& coord, DmEntity** entity = NULL, DM::ResolveLevel level = DM::ResolveNone) const;

    virtual void move(const DmVector& offset);
    virtual void rotate(const DmVector& center, const DmVector& angleVector);
    virtual void scale(const DmVector& center, const DmVector& factor);
    virtual void mirror(const DmVector& axisPoint1, const DmVector& axisPoint2);

    virtual void calculateBorders();

    std::list<DmEntity*> getSubEntities() const override;

protected:
    DmConstructionLineData data; ///< 构造线数据
};

#endif
