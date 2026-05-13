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

/// @file DmLeader.h
/// @brief 引线标注类

#ifndef DMLEADER_H
#define DMLEADER_H

#include "DmEntity.h"
#include "DmDimensionStyle.h"
#include "DimVars.h"

class DmLeaderData
{
public:
	DmLeaderData() = default;
	DmLeaderData(DmDimensionStyle* style, const std::vector<DmVector>& _vertexes);
	DmDimensionStyle* pStyle;
	std::vector<DmVector> vertextes;

	///////以下为替代属性
public:
	DIM_FUNCS_DECLARE()
private:
	DIM_MEMERS_DECLARE()
};

/// @brief 引线标注的引线
class DmLeader : public DmEntity
{
	TYPESYSTEM_HEADER();
public:
	DmLeader(DmEntity* parent = NULL);
	DmLeader(DmEntity* parent, const DmLeaderData& d);
	~DmLeader();
	DmEntity* clone() const override;

	DM::EntityType getEntityType() const override;
	void update() override;
	DmLeaderData getData() const;
	DmLeaderData& getDataRef();
	DmVectorSolutions getRefPoints() const override;

	void addEntity(DmEntity* e);
	void clear();

	DmEntity* addVertex(const DmVector& v);

	bool isContainer() const override;
	void calculateBorders() override;
	double getDistanceToPoint(const DmVector& coord, DmEntity** entity = nullptr, DM::ResolveLevel level = DM::ResolveNone) const override;
	DmVector getNearestEndpoint(const DmVector& coord, double* dist = nullptr) const override;
	DmVector getNearestPointOnEntity(const DmVector& coord, bool onEntity = true, double* dist = nullptr, DmEntity** entity = nullptr) const override;
	DmVector getNearestCenter(const DmVector& coord, double* dist = nullptr) const override;
	DmVector getNearestMiddle(const DmVector& coord, double* dist = nullptr, int middlePoints = 1) const override;
	void setVisible(bool v) override;
	bool setSelected(bool select = true) override;
	void setHighlighted(bool highlight = true) override;

	void move(const DmVector& offset) override;
	void rotate(const DmVector& center, const DmVector& angleVector) override;
	void scale(const DmVector& center, const DmVector& factor) override;
	void mirror(const DmVector& axisPoint1, const DmVector& axisPoint2) override;
	void moveRef(const DmVector& ref, const DmVector& offset) override;

	std::list<DmEntity*> getSubEntities() const override;

	static std::string arrowToString(DM::ArrowType arrowType);
	static DM::ArrowType stringToArrow(const std::string& arrowName);

	// persistent helper
	virtual void saveStream(OutputStream& wrt) const override;
	virtual void restoreStream(InputStream& reader, const std::vector<PAIR>& revs) override;
	virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;

protected:
	DmLeaderData	data;
	bool			empty;
	DmEntityContainer* container;
};

#endif
