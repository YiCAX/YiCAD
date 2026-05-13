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

/// @file Fileio.cpp
/// @brief 文件导入导出接口类实现

#include "Fileio.h"

#include <cstddef>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>

#include "FilterOcdIO.h"
#include "filter/PluginIOInterface.h"
#include "Debug.h"

bool FileIO::fileImport(DmDocument& document, const QString& file)
{
    std::unique_ptr<FilterInterface>&& filter(getImportFilter(file));
    if (filter)
    {
        bool bImported = { filter->fileImport(document, file) };
        return bImported;
    }
    return false;
}

bool FileIO::fileExport(DmDocument& document, const QString& file, const QString& formatType)
{
    std::unique_ptr<FilterInterface>&& filter(getExportFilter(formatType));
    if (filter)
    {
        return filter->fileExport(document, file, formatType);
    }
    return false;
}

FileIO* FileIO::instance()
{
    static FileIO* uniqueInstance = nullptr;
    if (!uniqueInstance)
    {
        uniqueInstance = new FileIO();
    }
    return uniqueInstance;
}

std::unique_ptr<FilterInterface> FileIO::getImportFilter(const QString& file) const
{
    for (auto& f : getFilters())
    {
        std::unique_ptr<FilterInterface> filter(f());
        if (filter && filter->canImport(file))
        {
            return filter;
        }
    }
    return nullptr;
}

std::unique_ptr<FilterInterface> FileIO::getExportFilter(const QString& formatType) const
{
    for (auto& f : getFilters())
    {
        std::unique_ptr<FilterInterface> filter(f());
        if (filter && filter->canExport(formatType))
        {
            return filter;
        }
    }

    QMessageBox::critical(nullptr, QObject::tr("ToolTips"), QObject::tr("Unsupported file format, please use another format to export!"), QMessageBox::Cancel); // 暂不支持的文件格式，请使用其他格式导出
    return nullptr;
}

std::vector<std::function<FilterInterface* ()>> FileIO::getFilters()
{
    return
    {
        PluginIOInterface::createFilter,
        FilterOcdIO::createFilter
    };
}
