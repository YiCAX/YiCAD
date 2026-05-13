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

/// @file XMLWriterQXmlStreamWriter.cpp
/// @brief 基于QXmlStreamWriter的XML写入器类实现

#include "XMLWriterQXmlStreamWriter.h"

#include <QXmlStreamWriter>

#include "XmlWriterInterface.h"

XMLWriterQXmlStreamWriter::XMLWriterQXmlStreamWriter()
    : xmlWriter(new QXmlStreamWriter(&xml))
{
	xmlWriter->setAutoFormatting(true);
	xmlWriter->setCodec("UTF-8");
}

XMLWriterQXmlStreamWriter::~XMLWriterQXmlStreamWriter() = default;

void XMLWriterQXmlStreamWriter::createRootElement(const std::string &name, const std::string &namespace_uri)
{
    xmlWriter->writeStartDocument();
    xmlWriter->writeDefaultNamespace(QString::fromStdString(namespace_uri));
    xmlWriter->writeStartElement(QString::fromStdString(namespace_uri), QString::fromStdString(name));
}

void XMLWriterQXmlStreamWriter::addElement(const std::string &name, const std::string &namespace_uri)
{
    xmlWriter->writeStartElement(QString::fromStdString(namespace_uri), QString::fromStdString(name));
}

void XMLWriterQXmlStreamWriter::addAttribute(const std::string &name, const std::string &value, const std::string &namespace_uri)
{
    xmlWriter->writeAttribute(QString::fromStdString(namespace_uri), QString::fromStdString(name), QString::fromStdString(value));
}

void XMLWriterQXmlStreamWriter::addNamespaceDeclaration(const std::string &prefix, const std::string &namespace_uri)
{
    xmlWriter->writeNamespace(QString::fromStdString(namespace_uri), QString::fromStdString(prefix));
}

void XMLWriterQXmlStreamWriter::closeElement()
{
    xmlWriter->writeEndElement();
}

std::string XMLWriterQXmlStreamWriter::documentAsString()
{
    xmlWriter->writeEndDocument();

    return xml.toStdString();
}
