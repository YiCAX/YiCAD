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

/// @file Fileio.h
/// @brief 文件导入导出接口类头文件

#ifndef FILEIO_H
#define FILEIO_H

#include <vector>
#include <functional>
#include <memory>
#include "FilterInterface.h"

// TODO destructor for clear filterList
/// @class FileIO
/// @brief 文件导入导出的接口类
class FileIO
{
private:
    // singleton
    FileIO() = default;
    FileIO(FileIO const&) = delete;
    FileIO& operator = (FileIO const&) = delete;
    FileIO(FileIO&&) = delete;
    FileIO& operator = (FileIO&&) = delete;

public:
    /// @brief 实例化文件读写(单例)
    static FileIO* instance();

    /// @brief 获取导入的操作类指针
    std::unique_ptr<FilterInterface> getImportFilter(const QString& file) const;

    /// @brief 获取导出的操作类指针
    std::unique_ptr<FilterInterface> getExportFilter(const QString& formatType) const;

    /// @brief 文件导入
    bool fileImport(DmDocument& document, const QString& file);

    /// @brief 文件导出
    bool fileExport(DmDocument& document, const QString& file, const QString& formatType);

private:
    /// @brief 获取指向创建文件转换器的静态函数的指针列表
    static std::vector<std::function<FilterInterface* ()>> getFilters();
};

#endif