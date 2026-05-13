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

/// @file MetaLineTypes.cpp
/// @brief 线型实体序列化容器实现文件

#include "MetaLineTypes.h"

#include "DmLineType.h"
#include "DmLineTypeTable.h"
#include "DmDocument.h"
#include "DmEntityContainer.h"

#include "Reader.h"
#include "Writer.h"

constexpr const char* LINETYPES = "LineTypes";

MetaLineTypesContainer::MetaLineTypesContainer(DmDocument* pDoc)
	: m_pDocument(pDoc)
{
}

void MetaLineTypesContainer::saveXML(Writer& wrt) const
{
	wrt.incInd();

	auto lineTypes = m_pDocument->getLineTypeTable();
	auto active = lineTypes->getActive();

	// Insert lineTypes item
	for (auto i = lineTypes->begin(); i != lineTypes->end(); ++i)
	{
		DmLineType* lineType(*i);
		wrt.Stream() << wrt.ind() << "<LineType" << " name=\"" << lineType->getLineTypeName().toStdString() << "\"" << " active=\"" << (bool)(active == lineType) << "\">" << std::endl;
		auto desp = lineType->getLineTypeDesp();
		wrt.incInd();

		// dash
		wrt.Stream() << wrt.ind() << "<LineTypeDesp" << " desp=\"" << lineType->getLineTypeDesp().toStdString() << "\"/>" << std::endl;

		// data
		auto datas = lineType->getLineTypeData();
		wrt.Stream() << wrt.ind() << "<LineTypeData" << " Count=\"" << datas.size() << "\">" << std::endl;
		wrt.incInd();
		for (auto& data : datas)
		{
			wrt.Stream() << wrt.ind() << "<data" << " value=\"" << data << "\"/>" << std::endl;
		}
		wrt.decInd();
		wrt.Stream() << wrt.ind() << "</LineTypeData>" << std::endl;

		wrt.decInd();

		wrt.Stream() << wrt.ind() << "</LineType>" << std::endl;
	}

	wrt.decInd();
}

void MetaLineTypesContainer::restoreXML(XMLReader& reader)
{
	// restore lineTypes
	auto iCount = (size_t)reader.getAttributeAsInteger("Count");
	for (size_t idx = 0; idx < iCount; idx++)
	{
		reader.readElement("LineType");

		auto name = QString::fromStdString(reader.getAttribute("name"));
		bool active = reader.hasAttribute("active");

		if ((name == "ByLayer" || name == "ByBlock" || name == "Continuous") && m_pDocument->getLineTypeTable()->find(name))
		{
			reader.readEndElement("LineType");
			continue;
		}

		reader.readElement("LineTypeDesp");
		QString desp = QString::fromStdString(reader.getAttribute("desp"));
		QString outward = desp.replace(QRegExp("[a-zA-Z0-9()]"), "");

		reader.readElement("LineTypeData");
		auto iDataCount = (size_t)reader.getAttributeAsInteger("Count");
		auto data = std::vector<double>();
		for (size_t daidx = 0; daidx < iDataCount; daidx++)
		{
			reader.readElement("data");
			data.emplace_back(reader.getAttributeAsFloat("value"));
		}
		reader.readEndElement("LineTypeData");

		reader.readEndElement("LineType");

		DmLineType* linetype = new DmLineType(name);
		linetype->setLineTypeData(data);
		linetype->setLineTypeDesp(desp);
		linetype->setLineTypeName(name);
		linetype->setLineTypeOutWard(outward.trimmed());

		m_pDocument->getLineTypeTable()->add_direct(linetype);

		if (active)
		{
			m_pDocument->getLineTypeTable()->activate_direct(linetype);
		}
	}
}

unsigned int MetaLineTypesContainer::getMemSize() const
{
	return 0;
}
