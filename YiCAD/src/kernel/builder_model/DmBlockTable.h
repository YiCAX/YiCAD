/*
 * Copyright (C) 2026 YiCAD Team
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

/// @file DmBlockTable.h
/// @brief 块表，用来维护和管理文档中所有块模板

#ifndef DMBLOCKTABLE_H
#define DMBLOCKTABLE_H

#include <vector>
#include <unordered_map>

#include "TableBase.h"
#include "BlockTableCmd.h"
#include "DmId.h"

class QString;
class DmBlock;
class DmDocument;

/// @class DmBlockTable
/// @brief 块表
/// @description 用来维护块模板
class DmBlockTable : public ITable
{
    friend class BlockTableAddCmd;
    friend class BlockTableRemoveCmd;
    friend class BlockTableModifyCmd;

public:
    using iterator = FilterIterator<std::vector<DmBlock*>::iterator>;

public:
    DmBlockTable();
    virtual ~DmBlockTable() = default;

    void setDocument(DmDocument* pDoc) override;
    void startModify(DmObject* e) override;

    void clear();
    /// @return 当前未删除的块数量
    unsigned int count() const;

    /// @brief 用于 range-based for 遍历
    iterator begin();
    iterator end();

    void activate(const QString& name);
    void activate(DmBlock* block);
    void activate_direct(const QString& name);
    void activate_direct(DmBlock* block);
    /// @return 当前激活的块；若没有激活块则返回 nullptr
    DmBlock* getActive();

    virtual bool add(DmBlock* block, bool notify = true);
    virtual void remove(DmBlock* block);

    /// @brief 表中不存在该块，直接添加（供命令类使用）
    void add_direct(DmBlock* block);
    /// @brief 直接从表中移除块（供命令类使用）
    void remove_direct(DmBlock* block);
    virtual bool rename(DmBlock* block, const QString& name);
    DmBlock* find(const QString& name);
    DmBlock* find(const DmId& id);
    QString newName(const QString& suggestion = "");
    void toggle(const QString& name);
    void toggle(DmBlock* block);

private:
    std::vector<DmBlock*>           m_blocks;           ///< 文档中的块列表
    std::unordered_map<DmId, DmBlock*> m_blockMap;      ///< 通过 ID 查找块的映射
    DmBlock*                        m_pActiveBlock = nullptr;  ///< 当前激活的块
};

#endif
