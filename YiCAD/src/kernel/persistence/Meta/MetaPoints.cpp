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

/// @file MetaPoints.cpp
/// @brief 点实体序列化容器实现文件

#include "MetaPoints.h"

#include "DmPoint.h"
#include "DmDocument.h"

#include "Reader.h"
#include "Writer.h"

constexpr const char* POINTS     = "Points";
constexpr const char* POINT_REV  = "rev";
constexpr const char* POINT_FILE = "file";
constexpr const char* POINT_BIN  = "Points.bin";

MetaPointsContainer::MetaPointsContainer(DmDocument* pDoc)
	: m_pDocument(pDoc)
{
}

void MetaPointsContainer::saveXML(Writer& wrt) const
{
	wrt.incInd();

	std::vector<PAIR> revs;
	DmPoint::getRevId(revs);

	// Insert Points item
	wrt.Stream() << wrt.ind() << "<Points" << " levels=\"" << revs.size() << "\"" << " Count=\"" << m_entities.size() << "\"" << " file= \"" << wrt.addFile(POINT_BIN, this) << "\">" << std::endl;

	// write type and revision ids
	{
		wrt.incInd();

		for (auto& itype : revs)
		{
			auto strtype = itype.first;

			// wrt.Stream() << wrt.ind() << "<leve name=\"" << PersistentTools::base64_encode(strtype.c_str(), itype.first.length()) << "\" id = \"" << itype.second << "\"/>" << std::endl;
			wrt.Stream() << wrt.ind() << "<level name=\"" << strtype << "\" id = \"" << itype.second << "\"/>" << std::endl;
		}

		wrt.decInd();
	}

	wrt.Stream() << wrt.ind() << "</" << POINTS << ">" << std::endl;
	wrt.decInd();
}

void MetaPointsContainer::restoreXML(XMLReader& reader)
{
	reader.readElement(POINTS);

	std::string file(reader.getAttribute(POINT_FILE));

	// restore files
	if (!file.empty())
	{
		reader.addFile(file.c_str(), this);
	}

	// restore levels
	auto ilevels = (size_t)reader.getAttributeAsInteger("levels");

	for (size_t idx = 0; idx < ilevels; idx++)
	{
		reader.readElement("level");
		auto itype = reader.getAttribute("name");
		auto irev = reader.getAttributeAsInteger("id");
		m_revs.push_back(std::make_pair(itype, irev));
	}

	reader.readEndElement(POINTS);
}

unsigned int MetaPointsContainer::getMemSize() const
{
	return 0;
}

void MetaPointsContainer::saveStream(OutputStream& wrt) const
{
	for (auto& e : m_entities)
	{
		e->saveStream(wrt);
	}
}

void MetaPointsContainer::restoreStream(InputStream& rdr)
{
	while (!rdr.end())
	{
		// read all points
		auto iPoint = new DmPoint();
		iPoint->setDocument(m_pDocument);
		iPoint->restoreStream(rdr, m_revs);
		m_pDocument->getEntityTable()->add_direct(iPoint);
	}
}

void MetaPointsContainer::setEntities(std::list<DmEntity*>& entities)
{
	m_entities = entities;
}
