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

/// @file DmRay.h
/// @brief 射线实体类，由基点和方向定义

#ifndef DMRAY_H
#define DMRAY_H

#include "DmAtomicEntity.h"
#include "RayData.h"
#include "Quadratic.h"

/// @brief 射线实体，从基点向指定方向无限延伸
class DmRay : public DmAtomicEntity
{
    TYPESYSTEM_HEADER();

public:
    DmRay() = default;

    /// @brief 构造函数
    /// @param parent 父实体指针
    /// @param d 射线几何数据
    DmRay(DmEntity* parent, const RayData& d);
    ~DmRay();

    DmEntity* clone() const override;

    /// @brief 获取实体类型
    /// @return 射线实体类型
    DM::EntityType getEntityType() const override;

    /// @brief 获取射线几何数据
    /// @return 射线数据
    RayData getData() const;

    /// @brief 获取射线角度
    /// @return 角度值（弧度）
    double getAngle() const;

    /// @brief 获取基点
    /// @return 基点坐标
    DmVector getBasePoint();

    /// @brief 设置基点
    /// @param pt 新基点坐标
    void setBasePoint(const DmVector& pt);

    /// @brief 获取方向向量
    /// @return 方向向量
    DmVector getDirecion();

    /// @brief 设置方向向量
    /// @param vec 新方向向量
    void setDirection(const DmVector& vec);

    DmVectorSolutions getRefPoints() const override;
    DmVector getMiddlePoint() const override;
    DmVector getNearestEndpoint(const DmVector& coord,
                                double* dist = nullptr) const override;
    DmVector getNearestPointOnEntity(const DmVector& coord,
                                     bool onEntity = true,
                                     double* dist = nullptr,
                                     DmEntity** entity = nullptr)
        const override;
    DmVector getNearestMiddle(const DmVector& coord,
                              double* dist = nullptr,
                              int middlePoints = 1) const override;

    bool offset(const DmVector& coord, const double& distance) override;
    void move(const DmVector& offset) override;

    /// @brief 绕原点旋转
    /// @param angle 旋转角度（弧度）
    void rotate(const double& angle);

    void rotate(const DmVector& center,
                const DmVector& angleVector) override;
    void scale(const DmVector& factor) override;
    void scale(const DmVector& center,
               const DmVector& factor) override;
    void mirror(const DmVector& axisPoint1,
                const DmVector& axisPoint2) override;
    void moveRef(const DmVector& ref, const DmVector& offset) override;

    void calculateBorders() override;

    /// @brief 判断点是否在多边形内
    /// @param point 待判断的点
    /// @param vec 多边形顶点列表
    /// @return 在内部返回 true
    bool pointInPolygon(const DmVector point,
                        const std::vector<DmVector> vec);

    /// @brief 获取射线对应的二次曲线方程
    /// @return 二次曲线对象
    Quadratic getQuadratic() const;

    std::list<DmEntity*> getSubEntities() const override;

    // persistent helper
    virtual void saveStream(OutputStream& wrt) const override;
    virtual void restoreStream(InputStream& reader,
                               const std::vector<PAIR>& revs) override;
    virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;

private:
    bool isModify;              ///< 是否已修改标记

protected:
    RayData data;               ///< 射线几何数据
};

#endif
