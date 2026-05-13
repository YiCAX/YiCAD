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

/// @file DmDimAligned.h
/// @brief 对齐标注类

#ifndef DMDIMALIGNED_H
#define DMDIMALIGNED_H

#include "DmDimension.h"
#include "DmEntityContainer.h"
#include "DmDimensionStyle.h"

/// @brief 对齐标注数据
struct DmDimAlignedData
{
	DmDimAlignedData();

	/// @para extensionPoint1 尺寸界线1的起点
	/// @para extensionPoint2 尺寸界线2的起点
	DmDimAlignedData(const DmVector& extensionPoint1, const DmVector& extensionPoint2);

	/// @brief 尺寸界线1的起点
	DmVector extensionPoint1;
	/// @brief 尺寸界线2的起点
	DmVector extensionPoint2;
};

/// @brief 对齐标注
class DmDimAligned : public DmDimension
{
	TYPESYSTEM_HEADER();
public:
	DmDimAligned() = default;
	DmDimAligned(DmEntityContainer* parent, const DmDimensionData& d, const DmDimAlignedData& ed);
	//DmDimAligned(DmEntityContainer* parent, const DmDimAlignedData& data);

	DmEntity* clone() const override;

	///	@return DM::EntityDimAligned
	DM::EntityType getEntityType() const override;

	/// @return Copy of data that defines the aligned dimension.
	/// @see getData()
	DmDimAlignedData const& getEData() const;

	DmVectorSolutions getRefPoints() const override;

	//void updateDim(bool autoText = false) override;

	DmVector const& getExtensionPoint1() const;

	DmVector const& getExtensionPoint2() const;

	//DmVector const& getDefinitionPoint() const;

	// Recalculate the original Dimension Point to remove Dim oblique angle.
	//void updateDimPoint();
	//void update() override;

	void move(const DmVector& offset) override;
	void rotate(const DmVector& center, const DmVector& angleVector) override;
	void scale(const DmVector& center, const DmVector& factor) override;
	void mirror(const DmVector& axisPoint1, const DmVector& axisPoint2) override;
	bool hasEndpointsWithinWindow(const DmVector& v1, const DmVector& v2) override;
	//void stretch(const DmVector& firstCorner, const DmVector& secondCorner, const DmVector& offset) override;
	void moveRef(const DmVector& ref, const DmVector& offset) override;

	virtual void updateDim(bool autoText = false) override;

	// persistent helper
	virtual void saveStream(OutputStream& wrt) const override;
	virtual void restoreStream(InputStream& reader, const std::vector<PAIR>& revs) override;
	virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;

private:
	/// @brief 绘制内部标注（除标注界线）
	/// @param extendBoundaryLine1 [out] 延伸标注界线1（水平方向在"尺寸界线上方"才可能延伸标注界线）
	/// @param extendBoundaryLine2 [out] 延伸标注界线2
	/// @param clipUnderLine [out] 是否裁剪标注界线（只有垂直方向居中才可能裁剪）
	void updateInnerDim(const DmVector& offsetVec, const DmVector& boundaryLineDir, const double boundaryLineStartOffset, const double boundaryLineEndOffset,
		bool& extendBoundaryLine1, bool& extendBoundaryLine2, bool& clipBoundLine, DmEntityContainer* pText, const DmVector& textSize);
	void updateOuterDim(const DmVector& offsetVec, const DmVector& boundaryLineDir, const double boundaryLineStartOffset, const double boundaryLineEndOffset,
		bool& extendBoundaryLine1, bool& extendBoundaryLine2, bool& clipBoundLine, DmEntityContainer* pText, const DmVector& textSize);
	
protected:
	// Extended data. 
	DmDimAlignedData edata;

};

#endif
