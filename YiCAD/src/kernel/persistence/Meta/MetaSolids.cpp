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

/// @file MetaSolids.cpp
/// @brief 填充(Solid)实体序列化容器实现文件

#include "MetaSolids.h"

#include "DmSolid.h"
#include "DmDocument.h"

#include "Reader.h"
#include "Writer.h"

constexpr const char* SOLIDS = "Solids";
constexpr const char* SOLID_REV = "rev";
constexpr const char* SOLID_FILE = "file";
constexpr const char* SOLID_BIN = "Solids.bin";

MetaSolidsContainer::MetaSolidsContainer(DmDocument* pDoc)
    : m_pDocument(pDoc)
{
}

void MetaSolidsContainer::saveXML(Writer& wrt) const
{
    wrt.incInd();

    std::vector<PAIR> revs;
    DmSolid::getRevId(revs);

    // Insert Solids item
    wrt.Stream() << wrt.ind() << "<Solids" << " levels=\"" << revs.size() << "\"" << " Count=\"" << m_entities.size() << "\"" << " file= \"" << wrt.addFile(SOLID_BIN, this) << "\">" << std::endl;

    // write type and revision ids
    {
        wrt.incInd();
        for (auto itype : revs)
        {
            auto strtype = itype.first;

            // wrt.Stream() << wrt.ind() << "<leve name=\"" << PersistentTools::base64_encode(strtype.c_str(), itype.first.length()) << "\" id = \"" << itype.second << "\"/>" << std::endl;
            wrt.Stream() << wrt.ind() << "<level name=\"" << strtype << "\" id = \"" << itype.second << "\"/>" << std::endl;
        }

        wrt.decInd();
    }
    wrt.Stream() << wrt.ind() << "</" << SOLIDS << ">" << std::endl;
    wrt.decInd();
}

void MetaSolidsContainer::restoreXML(XMLReader& reader)
{
    reader.readElement(SOLIDS);

    std::string file(reader.getAttribute(SOLID_FILE));

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

    reader.readEndElement(SOLIDS);
}

unsigned int MetaSolidsContainer::getMemSize() const
{
    return 0;
}

void MetaSolidsContainer::saveStream(OutputStream& wrt) const
{
    // give each entity a chance to add entry or a file
    for (auto& e : m_entities)
    {
        e->saveStream(wrt);
    }
}

void MetaSolidsContainer::restoreStream(InputStream& rdr)
{
    while (!rdr.end())
    {
        // read all solids
        auto iSolid = new DmSolid();
        iSolid->setDocument(m_pDocument);
        iSolid->restoreStream(rdr, m_revs);
        m_pDocument->getEntityTable()->add_direct(iSolid);
    }
}

void MetaSolidsContainer::setEntities(std::list<DmEntity*>& entities)
{
    m_entities = entities;
}
