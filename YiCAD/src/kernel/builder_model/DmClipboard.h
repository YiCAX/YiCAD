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

/// @file DmClipboard.h
/// @brief YiCAD 内部剪贴板（单例模式），不使用系统剪贴板以保证可移植性

#ifndef DMCLIPBOARD_H
#define DMCLIPBOARD_H

#include <iosfwd>

#include "DmDocument.h"

#define DMCLIPBOARD DmClipboard::instance()

class DmBlock;
class DmLayer;
class DmEntity;

// YiCAD internal clipboard. We don't use the system clipboard for better portaility.
// Implemented as singleton.
class DmClipboard
{
protected:
    DmClipboard();

public:
    /// @brief 获取唯一剪贴板实例（单例模式）
    /// @return 剪贴板实例指针
    static DmClipboard* instance();

    /// @brief 清空剪贴板
    void clear();

    /// @brief 添加块到剪贴板
    /// @param b 块指针
    void addBlock(DmBlock* b);

    /// @brief 检查剪贴板是否包含指定名称的块
    /// @param name 块名称
    /// @return 如果包含则返回true
    bool hasBlock(const QString& name);

    /// @brief 获取剪贴板中的块数量
    /// @return 块数量
    int countBlocks();

    /// @brief 添加图层到剪贴板
    /// @param l 图层指针
    void addLayer(DmLayer* l);

    /// @brief 检查剪贴板是否包含指定名称的图层
    /// @param name 图层名称
    /// @return 如果包含则返回true
    bool hasLayer(const QString& name);

    /// @brief 添加实体到剪贴板
    /// @param e 实体指针
    void addEntity(DmEntity* e);

    /// @brief 获取剪贴板中实体数量
    /// @return 实体数量
    unsigned count();

    /// @brief 获取剪贴板文档
    /// @return 文档指针
    DmDocument* getDocument();

protected:
    static DmClipboard* uniqueInstance; ///< 单例实例

    DmDocument pDocument; ///< 剪贴板文档
};

#endif
