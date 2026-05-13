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

/// @file DmChar.h
/// @brief 文字字符实体类

#ifndef DMCHAR_H
#define DMCHAR_H

#include <QString>
#include "DmEntity.h"


class DmLine;
class DmCharTemplate;

/// @brief 代表一个文字实体
class DmChar :public DmEntity
{
public:
	DmChar(DmCharTemplate* charTempl, DmEntity* parent = nullptr);
	DmEntity* clone() const override;
	~DmChar();
	friend class DmCharTemplate;
	
public:
	DM::EntityType getEntityType() const override;
	/// @brief 是否为空白符
	bool isWriteSpace() const;
	void addEntity(DmEntity* e);
	void removeEntity(DmEntity* e);

	QString getName() const;
	void setName(const QString& name);
	double getWidth() const;
	void setWidth(const double width);
	double getHeight() const;
	void setHeight(const double height);
	/// @brief 获得基线以上（非留白）高度
	double getAscender() const;
	/// @brief 获得基线以下（非留白）高度
	double getDescender() const;
	double getWidthFactor() const;
	void setWidthFactor(const double widthFactor);
	double getSlashAngle() const;
	void setSlashAngle(const double slashAngle);
	double getNominalHeight() const;
	void setNominalHeight(const double nominalHeight);
	DmVector getPosition() const;
	void setPosition(const DmVector& pos);
	bool hasOverline() const;
	DmLine* getOverline() const;
	void setOverline(DmLine* overline);
	void addOverline();
	void removeOverline();
	bool hasUnderline() const;
	DmLine* getUnderline() const;
	void setUnderline(DmLine* underline);
	void addUnderline();
	void removeUnderline();
	bool hasStrikethrough() const;
	DmLine* getStrikethrough() const;
	void setStrikethrough(DmLine* strikethrough);
	void addStrikethrough();
	void removeStrikethrough();
	bool isBold() const;
	void setBold(bool bold);
	bool isItalic() const;
	void setItalic(bool italic);

	bool isContainer() const override;
	void calculateBorders() override; 
	/// @brief 计算文字到点的距离。在包围框内，计算最近距离，在包围框外，返回无穷大
	double getDistanceToPoint(const DmVector& coord, DmEntity** entity = nullptr, DM::ResolveLevel level = DM::ResolveNone) const override;
	DmVector getNearestEndpoint(const DmVector& coord, double* dist = nullptr) const override;
	DmVector getNearestPointOnEntity(const DmVector& /*coord*/, bool onEntity = true, double* dist = nullptr, DmEntity** entity = nullptr) const override;
	DmVector getNearestCenter(const DmVector& coord, double* dist = nullptr) const override;
	DmVector getNearestMiddle(const DmVector& coord, double* dist = nullptr, int middlePoints = 1) const override;
	void setVisible(bool v) override;
	bool setSelected(bool select = true) override;
	void setHighlighted(bool highlight = true) override;

	void rotate(const DmVector& center, const DmVector& angleVector) override;
	void mirror(const DmVector& axisPoint1, const DmVector& axisPoint2) override;
	std::list<DmEntity*> getSubEntities() const override;
public:
	DmCharTemplate* getCharTemplate() const;
	void setCharTemplate(DmCharTemplate* templ);
	bool isNewLine() const;
	bool isSpace() const;
	bool isTab() const;
	/// @brief 重写基类的方法以更新宽高。所有scale的重载最终都调用这个，所以只能重写这个。
	virtual void scale(const DmVector& center, const DmVector& factor) override;
	void move(const DmVector& offset) override;
	void moveTo(const DmVector& newPos);
private:
	QString m_name;			//字符名
	DmCharTemplate* m_pCharTemplate;	//文字对应的模板
	double m_dHeight;	//不留白高度，同DmCharTemplate
	double m_dWidth;	//宽度

	double m_dWidthFactor;	//宽度系数
	double m_dSlashAngle;	//倾斜角度（弧度角）
	double m_dNominalHeight;	//名义高度（UI输入的高度）
	bool m_isBold;		//是否为粗体
	bool m_isItalic;		//是否为斜体
	bool m_hasOverline;	//是否有上划线
	bool m_hasUnderline;	//是否有下划线
	bool m_hasStrikethrough;	//是否有删除线
	DmVector m_pos;	//文字所在的位置

	// 
	DmLine* m_overline;
	DmLine* m_underline;
	DmLine* m_strikethrough;

	std::list<DmEntity*> entities;
};
#endif //!DMCHAR_H