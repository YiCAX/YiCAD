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

/// @file MetaMTexts.cpp
/// @brief 多行文字实体序列化容器实现文件

#include "MetaMTexts.h"

#include "DmMText.h"
#include "DmDocument.h"
#include "DmEntityContainer.h"

#include "Reader.h"
#include "Writer.h"

constexpr const char* MTEXTS = "MTexts";
constexpr const char* MTEXTS_REV = "rev";
constexpr const char* MTEXTS_FILE = "file";
constexpr const char* MTEXTS_BIN = "MTexts.bin";

MetaMTextsContainer::MetaMTextsContainer(DmDocument* pDoc)
	: m_pDocument(pDoc)
{
}

void MetaMTextsContainer::saveXML(Writer& wrt) const
{
	wrt.incInd();

	std::vector<PAIR> revs;
	DmMText::getRevId(revs);

	// Insert lines item
	wrt.Stream() << wrt.ind() << "<MTexts" << " levels=\"" << revs.size() << "\"" << " Count=\"" << m_entities.size() << "\"" << " file= \"" << wrt.addFile(MTEXTS_BIN, this) << "\">" << std::endl;

	// write type and revision ids
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
	wrt.Stream() << wrt.ind() << "</" << MTEXTS << ">" << std::endl;
	wrt.decInd();
}

void MetaMTextsContainer::restoreXML(XMLReader& reader)
{
	reader.readElement(MTEXTS);

	std::string file(reader.getAttribute(MTEXTS_FILE));
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

	reader.readEndElement(MTEXTS);
}

unsigned int MetaMTextsContainer::getMemSize() const
{
	return 0;
}

void MetaMTextsContainer::saveStream(OutputStream& wrt) const
{
	for (auto& e : m_entities)
	{
		e->saveStream(wrt);
	}
}

void MetaMTextsContainer::restoreStream(InputStream& rdr)
{
	while (!rdr.end())
	{
		// read all mtexts
		auto imtext = new DmMText();
		imtext->setDocument(m_pDocument);
		imtext->restoreStream(rdr, m_revs);
		m_pDocument->getEntityTable()->add_direct(imtext);
	}
}

void MetaMTextsContainer::setEntities(std::list<DmEntity*>& entities)
{
	m_entities = entities;
}
