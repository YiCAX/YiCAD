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

/// @file DmDimensionStyle.h
/// @brief 标注样式类

#ifndef DMDIMENSIONSTYLE_H
#define DMDIMENSIONSTYLE_H

#include <vector>
#include <QString> 
#include "Datamodel.h"
#include "DimVars.h"
#include "DmColor.h"
#include "TextConsts.h"
#include "DmObject.h"

#include "DmLineTypeTable.h"

class DmVector;
class DmTextStyle;
class DmEntityContainer;
class DmDimension;
class DmDocument;
class DmDimensionStyleTable;
struct DmDimensionData;
class Writer;
class Reader;
class OutputStream;
class InputStream;

class DmDimensionStyleData
{
public:
	QString name{""};											//标注样式名
	DmDimensionStyleData() = default;
	DmDimensionStyleData(const DmDimensionData* dimData);
public:
	DIM_FUNCS_DECLARE()
private:
	DIM_MEMERS_DECLARE()
};

class DmDimensionStyle : public DmObject
{
	TYPESYSTEM_HEADER();

public:
	DmDimensionStyle() = default;
	explicit DmDimensionStyle(const QString& name , DmTextStyle* pTextStyle);
	explicit DmDimensionStyle(const DmDimensionStyleData& data);
	explicit DmDimensionStyle(const DmDimensionStyle& style);
	explicit DmDimensionStyle(const DmDimensionStyle& temp, const QString& name);
	QString getName() const;
	void setName(const QString& name) { m_styleData.name = name; }
	void updateData(const DmDimensionStyleData& data);
	DmDimensionStyleData& getDataRef() { return m_styleData; }
	const DmDimensionStyleData& getDataConstRef() const { return m_styleData; }
	bool canArrow1Mirror() const;
	bool canArrow2Mirror() const;
	double getArrow1CutDistance() const;
	double getArrow2CutDistance() const;
	double getLeaderArrowCutDistance() const;
	static double getArrowCutDistance(DM::ArrowType arrowType);
	static QString getArrowBlockName(DM::ArrowType arrowType);
	/// @brief 获得有效的高度（不会为0）
	double getValidTextHeight() const;
	/// @brief 获得指定值的文字，文字会按标注样式格式生成
	DmEntityContainer* getText(const double& value, DmDimension* parent, DM::EntityType entityType) const;
	static DmEntityContainer* getText(const DmDimensionStyleData& styleData,const double& value, DmDimension* parent, DM::EntityType entityType);
	/// @brief 获得指定文本的文字，用于生成标注的“替代文字”
	DmEntityContainer* getText(const QString& label, DmDimension* parent) const;
	static DmEntityContainer* getText(const DmDimensionStyleData& styleData,const QString& label, DmDimension* parent);

	QString getFormatString(const double value, DM::EntityType entityType) const;
	static QString getFormatString(const DmDimensionStyleData& styleData, const double value, DM::EntityType entityType);

	void getPreview(DmEntityContainer* previewContainer) const;

	DM::ArrowType stringToArrow(const std::string& arrowName);

	// persistent helper
	virtual void saveStream(OutputStream& wrt) const override;
	virtual void restoreStream(InputStream& rdr, const std::vector<PAIR>& revs) override;
	virtual void restoreStreamWithRev(InputStream& rdr, int rev) override;
    virtual void restoreStream(InputStream& rdr) override;

private:
	static DmEntityContainer* getTextForLinear(const DmDimensionStyleData& styleData,const double value, DmDimension* parent, DM::EntityType entityType);
	static DmEntityContainer* getTextForAngular(const DmDimensionStyleData& styleData, const double value, DmDimension* parent, DM::EntityType entityType);
	static std::vector<QString> getFormatStrsForAngular(const DmDimensionStyleData& styleData, const double value, DM::EntityType entityType);
	static std::vector<QString> getFormatStrsForLinear(const DmDimensionStyleData& styleData, const double value, DM::EntityType entityType);

	//添加前缀，后缀
	static void addPrePostfix(const DmDimensionStyleData& styleData, std::vector<QString>& strs, DM::EntityType entityType);

	/// @brief 根据精度获得角度的"度分秒"表达
	/// @param [in] 弧度角
	/// @retrun "度分秒"表达
	static QString getStrForDMS(const DmDimensionStyleData& styleData, const double rad);

	static std::vector<QString> getTextStringOfFraction(const DmDimensionStyleData& styleData, const double value);
	/// @brief 数值字符串去掉尾部的0
	static QString getTrimEndZero(const QString& valueStr);
	/// @brief 根据精度获得分母
	static int getPrecisionDenominator(const DmDimensionStyleData& styleData);
	/// @brief 获得小数分隔符
	static QString getDecimalSaparatorStr(const DmDimensionStyleData& styleData);
	static void createTextForStrs(const DmDimensionStyleData& styleData, DmEntityContainer* contaner, const std::vector<QString>& strs);
	static bool canArrowMirror(DM::ArrowType arrowType);

private:
	DmDimensionStyleData	m_styleData;
};
#endif
