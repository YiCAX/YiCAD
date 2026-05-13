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
/// @file MetaAttributeDefinitions.cpp
/// @brief 属性定义元数据容器实现文件

#include "MetaAttributeDefinitions.h"

#include "DmAttributeDefinition.h"
#include "DmDocument.h"

#include "Reader.h"
#include "Writer.h"

constexpr const char* ATTRIBUTEDEFINITIONS = "AttributeDefinitions";
constexpr const char* ATTRIBUTEDEFINITION_REV = "rev";
constexpr const char* ATTRIBUTEDEFINITION_FILE = "file";
constexpr const char* ATTRIBUTEDEFINITION_BIN = "AttributeDefinitions.bin";

MetaAttributeDefinitionsContainer::MetaAttributeDefinitionsContainer(DmDocument* pDoc)
	: m_pDocument(pDoc)
{
}

void MetaAttributeDefinitionsContainer::saveXML(Writer& wrt) const
{
	wrt.incInd();

	std::vector<PAIR> revs;
	DmAttributeDefinition::getRevId(revs);

	//Insert AttributeDefinitions item
	wrt.Stream() << wrt.ind() << "<AttributeDefinitions" << " levels=\"" << revs.size() << "\"" << " Count=\"" << m_entities.size() << "\"" << " file= \"" << wrt.addFile(ATTRIBUTEDEFINITION_BIN, this) << "\">" << std::endl;

	//write type and revision ids
	{
		wrt.incInd();
		for (auto itype : revs)
		{
			auto strtype = itype.first;

			//wrt.Stream() << wrt.ind() << "<leve name=\"" << PersistentTools::base64_encode(strtype.c_str(), itype.first.length()) << "\" id = \"" << itype.second << "\"/>" << std::endl;
			wrt.Stream() << wrt.ind() << "<level name=\"" << strtype << "\" id = \"" << itype.second << "\"/>" << std::endl;
		}

		wrt.decInd();
	}
	wrt.Stream() << wrt.ind() << "</" << ATTRIBUTEDEFINITIONS << ">" << std::endl;
	wrt.decInd();
}

void MetaAttributeDefinitionsContainer::restoreXML(XMLReader& reader)
{
	reader.readElement(ATTRIBUTEDEFINITIONS);

	std::string file(reader.getAttribute(ATTRIBUTEDEFINITION_FILE));
	//restore files
	if (!file.empty())
	{
		reader.addFile(file.c_str(), this);
	}

	//restore levels
	auto ilevels = (size_t)reader.getAttributeAsInteger("levels");
	for (size_t idx = 0; idx < ilevels; idx++)
	{
		reader.readElement("level");
		auto itype = reader.getAttribute("name");
		auto irev = reader.getAttributeAsInteger("id");
		m_revs.push_back(std::make_pair(itype, irev));
	}

	reader.readEndElement(ATTRIBUTEDEFINITIONS);
}

unsigned int MetaAttributeDefinitionsContainer::getMemSize() const
{
	return 0;
}

void MetaAttributeDefinitionsContainer::saveStream(OutputStream& wrt) const
{
	for (auto& e : m_entities)
	{
		e->saveStream(wrt);
	}
}

void MetaAttributeDefinitionsContainer::restoreStream(InputStream& rdr)
{
	while (!rdr.end())
	{
		// read all AttributeDefinitions
		auto iAttributeDefinition = new DmAttributeDefinition();
		iAttributeDefinition->setDocument(m_pDocument);
		iAttributeDefinition->restoreStream(rdr, m_revs);
		m_pDocument->getEntityTable()->add_direct(iAttributeDefinition);
	}
}

void MetaAttributeDefinitionsContainer::setEntities(std::list<DmEntity*>& entities)
{
	m_entities = entities;
}
