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

/// @file DmDimAngular.h
/// @brief 角度标注类

#ifndef DMDIMANGULAR_H
#define DMDIMANGULAR_H

#include "DmDimension.h"
#include "DmDimensionStyle.h"

/// @brief 角度标注的数据
struct DmDimAngularData
{
	DmDimAngularData() = default;
	DmDimAngularData(const DmVector& line1StartPt, const DmVector& line1EndPt, 
		const DmVector& line2StartPt, const DmVector& line2EndPt, const DmVector& ptOnArc);

	DmVector line1StartPt;	//所标注的直线1起点
	DmVector line1EndPt;	//所标注的直线1终点
	DmVector line2StartPt;	//所标注的直线2起点
	DmVector line2EndPt;	//所标注的直线2终点
	DmVector ptOnArc;	//标注圆弧上的一个点（用来定位标注的圆弧所在象限）
};

/// @brief 角度标注
class DmDimAngular : public DmDimension
{
	TYPESYSTEM_HEADER();
public:
	DmDimAngular() = default;
	DmDimAngular(DmEntityContainer* parent, const DmDimensionData& d, const DmDimAngularData& ed);

	DmEntity* clone() const override;
	DM::EntityType getEntityType() const override;
	DmDimAngularData getEData() const;
	DmVector getCenter() const override;
	DmVectorSolutions getRefPoints() const override;

	void updateDim(bool autoText = false) override;

	void move(const DmVector& offset) override;
	void rotate(const DmVector& center, const DmVector& angleVector) override;
	void scale(const DmVector& center, const DmVector& factor) override;
	void mirror(const DmVector& axisPoint1, const DmVector& axisPoint2) override;
	void moveRef(const DmVector& ref, const DmVector& offset) override;

	// persistent helper
	virtual void saveStream(OutputStream& wrt) const override;
	virtual void restoreStream(InputStream& reader, const std::vector<PAIR>& revs) override;
	virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;

private:
	/// @brief 计算一些后续需要的参数
	void calculateData();
	/// @brief 计算标注界线一些参数
	void getBoundaryLineInfos(bool& needBoundLine1, DmVector& boundaryLineDir1, bool& needBoundLine2, DmVector& boundaryLineDir2,
		double& boundaryLine1StartOffset, double& boundaryLine1EndOffset, double& boundaryLine2StartOffset, double& boundaryLine2EndOffset) const;
	/// @brief 绘制内部标注（除标注界线）
	/// @param extendBoundaryLine1 [out] 延伸标注界线1（水平方向在"尺寸界线上方"才可能延伸标注界线）
	/// @param extendBoundaryLine2 [out] 延伸标注界线2
	/// @param clipUnderLine [out] 是否裁剪标注界线（只有垂直方向居中才可能裁剪）
	void updateInnerDim(const DmVector& boundaryLineDir1, const DmVector& boundaryLineDir2, const double boundaryLine1StartOffset, const double boundaryLine1EndOffset,
		const double boundaryLine2StartOffset, const double boundaryLine2EndOffset,
		bool& extendBoundaryLine1, bool& extendBoundaryLine2, bool& clipBoundLine, DmEntityContainer* pText, const DmVector& textSize);
	void updateOuterDim(const DmVector& boundaryLineDir1, const DmVector& boundaryLineDir2, const double boundaryLine1StartOffset, const double boundaryLine1EndOffset,
		const double boundaryLine2StartOffset, const double boundaryLine2EndOffset,
		bool& extendBoundaryLine1, bool& extendBoundaryLine2, bool& clipBoundLine, DmEntityContainer* pText, const DmVector& textSize);

protected:
	DmDimAngularData edata;

private:
	DmVector m_center;		//圆弧中心
	double m_radius;		//圆弧半径
	DmVector m_arcEndPt1;	//圆弧与标注线1交点
	DmVector m_arcEndPt2;	//圆弧与标注线2交点
	DmVector m_arcEndPt1Dir;//圆心到m_arcEndPt1方向向量
	DmVector m_arcEndPt2Dir;
	double m_arcStartAngle;	//圆弧起始角度
	double m_arcEndAngle;	//圆弧终止角度
};

#endif
