/*
 * Copyright (C) 2026 YiCAD
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

/// @file DbPluginInterface.cpp
/// @brief 插件数据库接口类实现，管理导入导出格式类型注册与文件I/O调度

#include "DbPluginInterface.h"

#include "DmSystem.h"
#include "Fileio.h"
#include "filter/PluginIOInterface.h"
#include "GuiDialogFactory.h"

Database_plugin_interface::Database_plugin_interface()
    : m_pImportFormatTypes(new std::list<QString>())
    , m_pExportFormatTypes(new std::list<QString>())
    , m_pluginIoInterface(new PluginIOInterface())
{
}

void Database_plugin_interface::addImportType(const QString& type, const QString& formatType)
{
    m_pImportFormatTypes->emplace_back(formatType);
    DMSYSTEM->addImportFormatType(type, formatType);
}

void Database_plugin_interface::addExportType(const QString& type, const QString& formatType)
{
    m_pExportFormatTypes->emplace_back(formatType);
    DMSYSTEM->addExportFormatType(type, formatType);
}

bool Database_plugin_interface::canImport(const QString& file) const
{
    return m_pluginIoInterface->canImport(file);
}

bool Database_plugin_interface::canExport(const QString& formatType) const
{
    return m_pluginIoInterface->canExport(formatType);
}

bool Database_plugin_interface::fileImport(const nlohmann::json& json)
{
    return false;
}

bool Database_plugin_interface::fileExport(const nlohmann::json& json, const QString& file, const QString& formatType)
{
    return false;
}

void Database_plugin_interface::setCurrentType(const QString& type)
{
    DMSYSTEM->setCurrentFormatType(type);
}

void Database_plugin_interface::outputCmdMessage(const QString& msg)
{
    GUIDIALOGFACTORY->commandMessage(msg);
}
