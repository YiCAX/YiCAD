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

/// @file DmAttribute.cpp
/// @brief DmAttribute 块属性实体类的实现

#include "DmAttribute.h"

TYPESYSTEM_SOURCE(DmAttribute, DmEntity, 0);

DmAttribute::DmAttribute(DmEntity* parent, const TextData& textData, const AttributeData& attrData)
	:DmText(parent, textData), data(attrData)
{
}

DmEntity* DmAttribute::clone() const
{
	DmAttribute* t = new DmAttribute(*this);
	t->update();
	return t;
}

DM::EntityType DmAttribute::getEntityType() const
{
	return DM::EntityAttribute;
}

AttributeData& DmAttribute::getAttributeDataRef()
{
	return data;
}

AttributeData DmAttribute::getAttributeData() const
{
	return data;
}

QString DmAttribute::getTag() const
{
	return data.getTag();
}

void DmAttribute::setTag(const QString& tag)
{
	data.setTag(tag);
}

void DmAttribute::saveStream(OutputStream& wrt) const
{
	DmText::saveStream(wrt);

	auto tag = data.getTag().toStdString();

	wrt << (std::string)tag;
}

void DmAttribute::restoreStream(InputStream& reader, const std::vector<PAIR>& revs)
{
	DmText::restoreStream(reader, revs);

	int fileRev = getRevisionId("DmAttribute", revs);
	if (revId > fileRev)
	{
		// 老文件格式
		restoreStreamWithRev(reader, fileRev);
	}
	else
	{
		std::string tag;
		reader >> (std::string&)tag;

		data.setTag(QString::fromStdString(tag));
		update();
	}
}

void DmAttribute::restoreStreamWithRev(InputStream& rdr, int rev)
{
	if (rev == 0)
	{
	}
	else //big change, e.g. change supper class of DmAttribute
	{
		//step1.
		// read all legacy data one by one
	}
}
