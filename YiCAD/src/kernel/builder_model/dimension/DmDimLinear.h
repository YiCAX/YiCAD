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

/// @file DmDimLinear.h
/// @brief 线性标注类

#ifndef DMDIMLINEAR_H
#define DMDIMLINEAR_H

#include "DmDimension.h"
#include "DmDimensionStyle.h"

struct DmDimLinearData
{
	DmDimLinearData();
	DmDimLinearData(const DmVector& extensionPoint1, const DmVector& extensionPoint2);

	/// @brief 尺寸界线1的起点
	DmVector extensionPoint1;
	/// @brief 尺寸界线2的起点
	DmVector extensionPoint2;
};

/// @brief 线性标注
class DmDimLinear : public DmDimension
{
	TYPESYSTEM_HEADER();
public:
	DmDimLinear() = default;
	DmDimLinear(DmEntityContainer* parent, const DmDimensionData& d, const DmDimLinearData& ed);
	virtual ~DmDimLinear() = default;

	virtual DmEntity* clone() const;

	/// @return DM::EntityDimLinear
	virtual DM::EntityType getEntityType() const;

	DmDimLinearData getEData() const;

	virtual DmVectorSolutions getRefPoints() const;

	virtual void updateDim(bool autoText = false);

	DmVector getExtensionPoint1() const;

	DmVector getExtensionPoint2() const;

	double getAngle() const;

	void setAngle(double a);

	double getDistance() const;

	virtual void move(const DmVector& offset);
	virtual void rotate(const DmVector& center, const DmVector& angleVector);
	virtual void scale(const DmVector& center, const DmVector& factor);
	virtual void mirror(const DmVector& axisPoint1, const DmVector& axisPoint2);
	virtual bool hasEndpointsWithinWindow(const DmVector& v1, const DmVector& v2);
	virtual void stretch(const DmVector& firstCorner, const DmVector& secondCorner, const DmVector& offset);
	virtual void moveRef(const DmVector& ref, const DmVector& offset);

	// persistent helper
	virtual void saveStream(OutputStream& wrt) const override;
	virtual void restoreStream(InputStream& reader, const std::vector<PAIR>& revs) override;
	virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;

private:
	//内部使用
	void getBoundaryLineInfos(DmVector& dimLineStartPt, DmVector& dimLineEndPt, DmVector& boundaryLineDir1, DmVector& boundaryLineDir2,
		double& boundaryLine1StartOffset, double& boundaryLine1EndOffset, double& boundaryLine2StartOffset, double& boundaryLine2EndOffset) const;
	/// @brief 绘制内部标注（除标注界线）
	/// @param extendBoundaryLine1 [out] 延伸标注界线1（水平方向在"尺寸界线上方"才可能延伸标注界线）
	/// @param extendBoundaryLine2 [out] 延伸标注界线2
	/// @param clipUnderLine [out] 是否裁剪标注界线（只有垂直方向居中才可能裁剪）
	void updateInnerDim(const DmVector& dimLineStartPtTemp, const DmVector& dimLineEndPtTemp, const DmVector& boundaryLineDir1, const DmVector& boundaryLineDir2, const double boundaryLine1StartOffset, const double boundaryLine1EndOffset,
		const double boundaryLine2StartOffset, const double boundaryLine2EndOffset,
		bool& extendBoundaryLine1, bool& extendBoundaryLine2, bool& clipBoundLine, DmEntityContainer* pText, const DmVector& textSize);
	void updateOuterDim(const DmVector& dimLineStartPtTemp, const DmVector& dimLineEndPtTemp, const DmVector& boundaryLineDir1, const DmVector& boundaryLineDir2, const double boundaryLine1StartOffset, const double boundaryLine1EndOffset,
		const double boundaryLine2StartOffset, const double boundaryLine2EndOffset,
		bool& extendBoundaryLine1, bool& extendBoundaryLine2, bool& clipBoundLine, DmEntityContainer* pText, const DmVector& textSize);
protected:
	// Extended data.
	DmDimLinearData edata;
};

#endif
