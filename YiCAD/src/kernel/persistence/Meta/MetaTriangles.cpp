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

/// @file MetaTriangles.cpp
/// @brief 三角形(Triangle)实体序列化容器实现文件

#include "MetaTriangles.h"

#include "DmTriangle.h"
#include "DmDocument.h"

#include "Reader.h"
#include "Writer.h"

constexpr const char* TRIANGLES = "Triangles";
constexpr const char* TRIANGLE_REV = "rev";
constexpr const char* TRIANGLE_FILE = "file";
constexpr const char* TRIANGLE_BIN = "Triangles.bin";

MetaTrianglesContainer::MetaTrianglesContainer(DmDocument* pDoc)
    : m_pDocument(pDoc)
{
}

void MetaTrianglesContainer::saveXML(Writer& wrt) const
{
    wrt.incInd();

    std::vector<PAIR> revs;
    DmTriangle::getRevId(revs);

    // Insert Triangles item
    wrt.Stream() << wrt.ind() << "<Triangles" << " levels=\"" << revs.size() << "\"" << " Count=\"" << m_entities.size() << "\"" << " file= \"" << wrt.addFile(TRIANGLE_BIN, this) << "\">" << std::endl;

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
    wrt.Stream() << wrt.ind() << "</" << TRIANGLES << ">" << std::endl;
    wrt.decInd();
}

void MetaTrianglesContainer::restoreXML(XMLReader& reader)
{
    reader.readElement(TRIANGLES);

    std::string file(reader.getAttribute(TRIANGLE_FILE));

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

    reader.readEndElement(TRIANGLES);
}

unsigned int MetaTrianglesContainer::getMemSize() const
{
    return 0;
}

void MetaTrianglesContainer::saveStream(OutputStream& wrt) const
{
    // give each entity a chance to add entry or a file
    for (auto& e : m_entities)
    {
        e->saveStream(wrt);
    }
}

void MetaTrianglesContainer::restoreStream(InputStream& rdr)
{
    while (!rdr.end())
    {
        // read all triangles
        auto iTriangle = new DmTriangle();
        iTriangle->setDocument(m_pDocument);
        iTriangle->restoreStream(rdr, m_revs);
        m_pDocument->getEntityTable()->add_direct(iTriangle);
    }
}

void MetaTrianglesContainer::setEntities(std::list<DmEntity*>& entities)
{
    m_entities = entities;
}
