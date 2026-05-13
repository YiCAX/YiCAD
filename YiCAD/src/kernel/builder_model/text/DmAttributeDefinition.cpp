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

/// @file DmAttributeDefinition.cpp
/// @brief DmAttributeDefinition 块属性定义类的实现

#include "DmAttributeDefinition.h"
#include "Debug.h"

TYPESYSTEM_SOURCE(DmAttributeDefinition, DmEntity, 0);

DmAttributeDefinition::DmAttributeDefinition(DmEntity* parent, const TextData& textData, const AttributeDefinitionData& attrData)
	: DmText(parent, textData)
	, data(attrData)
{
}

DmEntity* DmAttributeDefinition::clone() const
{
	DmAttributeDefinition* t = new DmAttributeDefinition(*this);
	t->update();
	return t;
}

DM::EntityType DmAttributeDefinition::getEntityType() const
{
	return DM::EntityAttributeDefinition;
}

AttributeDefinitionData& DmAttributeDefinition::getAttributeDataRef()
{
	return data;
}

AttributeDefinitionData DmAttributeDefinition::getAttributeData() const
{
	return data;
}

void DmAttributeDefinition::update()
{
	clear();

	QString text = getTag().toUpper();
	addEntitiesOfText(text);
}

QString DmAttributeDefinition::getTag() const
{
	return data.getTag();
}

void DmAttributeDefinition::setTag(const QString& tag)
{
	data.setTag(tag);
}

QString DmAttributeDefinition::getPrompt() const
{
	return data.getPrompt();
}

void DmAttributeDefinition::setPrompt(const QString& prompt)
{
	data.setPrompt(prompt);
}

void DmAttributeDefinition::saveStream(OutputStream& wrt) const
{
	DmText::saveStream(wrt);

	auto tag = data.getTag().toStdString();
	auto prompt = data.getPrompt().toStdString();

	wrt << (std::string)tag << (std::string)prompt;
}

void DmAttributeDefinition::restoreStream(InputStream& reader, const std::vector<PAIR>& revs)
{
	int fileRev = getRevisionId("DmAttributeDefinition", revs);
	if (revId > fileRev)
	{
        DmText::restoreStream(reader, revs);
		// 老文件格式
		restoreStreamWithRev(reader, fileRev);
	}
	else
	{
        restoreStream(reader);
	}
}

void DmAttributeDefinition::restoreStreamWithRev(InputStream& rdr, int rev)
{
	if (rev == 0)
	{
	}
	else //big change, e.g. change supper class of DmAttributeDefinition
	{
		//step1.
		// read all legacy data one by one
	}
}

void DmAttributeDefinition::restoreStream(InputStream& rdr)
{
    DmText::restoreStream(rdr);

    std::string tag;
    std::string prompt;
    rdr >> (std::string&)tag >> (std::string&)prompt;
    data.setTag(QString::fromStdString(tag));
    data.setPrompt(QString::fromStdString(prompt));
    update();
}
