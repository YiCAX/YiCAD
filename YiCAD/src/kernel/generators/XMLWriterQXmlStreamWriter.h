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

/// @file XMLWriterQXmlStreamWriter.h
/// @brief 基于QXmlStreamWriter的XML写入器类头文件

#ifndef XMLWRITERQXMLSTREAMWRITER_H
#define XMLWRITERQXMLSTREAMWRITER_H

#include <QString>
#include <memory>

#include "XmlWriterInterface.h"

class QXmlStreamWriter;

class XMLWriterQXmlStreamWriter : public XMLWriterInterface
{
public:
    XMLWriterQXmlStreamWriter();

    ~XMLWriterQXmlStreamWriter();

    void createRootElement(const std::string& name, const std::string& namespace_uri = "");

    void addElement(const std::string& name, const std::string& namespace_uri = "");

    void addAttribute(const std::string& name, const std::string& value, const std::string& namespace_uri = "");

    void addNamespaceDeclaration(const std::string& prefix, const std::string& namespace_uri);

    void closeElement();

    std::string documentAsString();

private:
    std::unique_ptr<QXmlStreamWriter> xmlWriter;

    QString xml;
};

#endif
