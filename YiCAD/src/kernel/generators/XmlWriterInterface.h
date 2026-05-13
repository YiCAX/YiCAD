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

/// @file XmlWriterInterface.h
/// @brief XML写入器抽象接口类头文件

#ifndef XMLWRITERINTERFACE_H
#define XMLWRITERINTERFACE_H

#include <string>

class XMLWriterInterface
{
public:
    virtual void createRootElement(const std::string& name, const std::string& default_namespace_uri = "") = 0;

    virtual void addElement(const std::string& name, const std::string& namespace_uri = "") = 0;

    virtual void addAttribute(const std::string& name, const std::string& value, const std::string& namespace_uri = "") = 0;

    virtual void addNamespaceDeclaration(const std::string& prefix, const std::string& namespace_uri) = 0;

    virtual void closeElement() = 0;

    virtual std::string documentAsString() = 0;

    XMLWriterInterface() = default;
    virtual ~XMLWriterInterface() = default;
};

#endif
