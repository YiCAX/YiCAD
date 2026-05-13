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

/// @file DmXline.h
/// @brief 无限长构造线实体类

#ifndef DMXLINE_H
#define DMXLINE_H

#include "DmAtomicEntity.h"
#include "XLineData.h"
#include "Quadratic.h"

/// @brief 无限长构造线
class DmXline : public DmAtomicEntity
{
	TYPESYSTEM_HEADER();

public:
	DmXline() = default;
	DmXline(DmEntity* parent, const XLineData& d);
	~DmXline();

public:
	DmEntity* clone() const override;

	/// @brief 获取实体类型
	DM::EntityType getEntityType() const override;

	/// @brief 获取线段几何数据
	XLineData getData() const;
	double getAngle() const;

	DmVector getBasePoint();
	void setBasePoint(const DmVector& pt);

	DmVector getDirecion();
	void setDirection(const DmVector& vec);

	DmVectorSolutions getRefPoints() const override;
	DmVector getNearestEndpoint(const DmVector& coord, double* dist = nullptr) const override;
	DmVector getNearestPointOnEntity(const DmVector& coord, bool onEntity = true, double* dist = nullptr, DmEntity** entity = nullptr) const override;
	DmVector getNearestMiddle(const DmVector& coord, double* dist = nullptr, int middlePoints = 1) const override;

	void move(const DmVector& offset) override;
	void rotate(const double& angle);
	void rotate(const DmVector& center, const DmVector& angleVector) override;
	void scale(const DmVector& factor) override;
	void scale(const DmVector& center, const DmVector& factor) override;
	void mirror(const DmVector& axisPoint1, const DmVector& axisPoint2) override;
	void moveRef(const DmVector& ref, const DmVector& offset) override;

	void calculateBorders() override;

	Quadratic getQuadratic() const;

	std::list<DmEntity*> getSubEntities() const override;

	// persistent helper
	virtual void saveStream(OutputStream& wrt) const override;
	virtual void restoreStream(InputStream& reader, const std::vector<PAIR>& revs) override;
	virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;

protected:
	XLineData data;

private:
	bool isModify;
};

#endif // !DMXLINE_H

