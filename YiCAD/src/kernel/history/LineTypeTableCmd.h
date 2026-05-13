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

/// @file LineTypeTableCmd.h
/// @brief 线型表命令，包含添加、修改、移除、激活线型的 Undo/Redo 命令

#ifndef LINETYPETABLECMD_H
#define LINETYPETABLECMD_H

#include "TableBase.h"
#include "DmLineType.h"

class DmLineTypeTable;
class DmLineType;

/// @brief 添加线型命令
class LineTypeTableAddCmd :public ICmd
{
public:
    /// @brief 构造添加线型命令
    /// @param table 线型表
    /// @param addedLineType 要添加的线型
    LineTypeTableAddCmd(DmLineTypeTable* table, DmLineType* addedLineType);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    DmObject* getObject() override {return m_addedLineType;}
    CmdType getCmdType() const override{return CmdType::LineTypeTableAddCmd;}

protected:
    DmLineTypeTable* m_table;           ///< 线型表
    DmLineType* m_addedLineType;        ///< 被添加的线型
    bool m_isOwnByCommand;               ///< 线型是否由该命令所有
};

/// @brief 修改线型命令
class LineTypeTableModifyCmd :public ICmd
{
public:
    LineTypeTableModifyCmd():m_table(nullptr),m_modifiedLineType(nullptr){}
    /// @brief 构造修改线型命令
    /// @param table 线型表
    /// @param modifiedLineType 要修改的线型
    LineTypeTableModifyCmd(DmLineTypeTable* table, DmLineType* modifiedLineType);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    DmObject* getObject() override {return m_modifiedLineType;}
    CmdType getCmdType() const override{return CmdType::LineTypeTableModifyCmd;}

protected:
    std::string m_originData;           ///< 原始序列化数据
    std::string m_newData;              ///< 新序列化数据
    DmLineType* m_modifiedLineType;     ///< 被修改的线型
    DmLineTypeTable* m_table;           ///< 线型表
};

/// @brief 移除线型命令
class LineTypeTableRemoveCmd :public ICmd
{
public:
    /// @brief 构造移除线型命令
    /// @param table 线型表
    /// @param removedLineType 要移除的线型
    LineTypeTableRemoveCmd(DmLineTypeTable* table, DmLineType* removedLineType);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    DmObject* getObject() override {return m_removedLineType;}
    CmdType getCmdType() const override{return CmdType::LineTypeTableRemoveCmd;}

protected:
    DmLineTypeTable* m_table;           ///< 线型表
    DmLineType* m_removedLineType;      ///< 被移除的线型
};

/// @brief 激活线型命令
class LineTypeTableActivateCmd :public ICmd
{
public:
    /// @brief 构造激活线型命令
    /// @param table 线型表
    /// @param activatedLineType 要激活的线型
    LineTypeTableActivateCmd(DmLineTypeTable* table, DmLineType* activatedLineType);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    CmdType getCmdType() const override{return CmdType::LineTypeTableActivateCmd;}

protected:
    DmLineTypeTable* m_table;               ///< 线型表
    DmLineType* m_activatedLineType;        ///< 被激活的线型
    DmLineType* m_originActiveLineType;     ///< 原来激活的线型
};

#endif //LINETYPETABLECMD_H
