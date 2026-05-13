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

/// @file DimensionStyleTableCmd.h
/// @brief 标注样式表命令，包含添加、修改、移除、激活标注样式的 Undo/Redo 命令

#ifndef DIMENSIONSTYLETABLECMD_H
#define DIMENSIONSTYLETABLECMD_H

#include "Cmd.h"
#include "DmDimensionStyle.h"
#include <string>

class DmDimensionStyleTable;

/// @brief 添加标注样式命令
class DimensionStyleTableAddCmd :public ICmd
{
public:
    /// @brief 构造添加标注样式命令
    /// @param table 标注样式表
    /// @param addedDimStyle 要添加的标注样式
    DimensionStyleTableAddCmd(DmDimensionStyleTable* table, DmDimensionStyle* addedDimStyle);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    virtual DmObject* getObject() {return m_addedDimStyle;}
    CmdType getCmdType() const override{return CmdType::DimensionStyleTableAddCmd;}

protected:
    DmDimensionStyleTable* m_table;             ///< 标注样式表
    DmDimensionStyle* m_addedDimStyle;           ///< 被添加的标注样式
    bool m_isOwnByCommand;                       ///< 是否由该命令所有
};

/// @brief 修改标注样式命令
class DimensionStyleTableModifyCmd :public ICmd
{
public:
    DimensionStyleTableModifyCmd():m_table(nullptr),m_modifiedDimStyle(nullptr){}
    /// @brief 构造修改标注样式命令
    /// @param table 标注样式表
    /// @param modifiedDimStyle 要修改的标注样式
    DimensionStyleTableModifyCmd(DmDimensionStyleTable* table, DmDimensionStyle* modifiedDimStyle);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    virtual DmObject* getObject() {return m_modifiedDimStyle;}
    CmdType getCmdType() const override{return CmdType::DimensionStyleTableModifyCmd;}

protected:
    std::string m_originData;                   ///< 原始序列化数据
    std::string m_newData;                      ///< 新序列化数据
    DmDimensionStyle* m_modifiedDimStyle;       ///< 被修改的标注样式
    DmDimensionStyleTable* m_table;             ///< 标注样式表
};

/// @brief 移除标注样式命令
class DimensionStyleTableRemoveCmd :public ICmd
{
public:
    /// @brief 构造移除标注样式命令
    /// @param table 标注样式表
    /// @param removedDimStyle 要移除的标注样式
    DimensionStyleTableRemoveCmd(DmDimensionStyleTable* table, DmDimensionStyle* removedDimStyle);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    virtual DmObject* getObject() {return m_removedDimStyle;}
    CmdType getCmdType() const override{return CmdType::DimensionStyleTableRemoveCmd;}

protected:
    DmDimensionStyleTable* m_table;             ///< 标注样式表
    DmDimensionStyle* m_removedDimStyle;        ///< 被移除的标注样式
};

/// @brief 激活标注样式命令
class DimensionStyleTableActivateCmd :public ICmd
{
public:
    /// @brief 构造激活标注样式命令
    /// @param table 标注样式表
    /// @param activatedDimStyle 要激活的标注样式
    DimensionStyleTableActivateCmd(DmDimensionStyleTable* table, DmDimensionStyle* activatedDimStyle);
    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    CmdType getCmdType() const override{return CmdType::DimensionStyleTableActivateCmd;}

protected:
    DmDimensionStyleTable* m_table;             ///< 标注样式表
    DmDimensionStyle* m_activatedDimStyle;      ///< 被激活的标注样式
    DmDimensionStyle* m_originActiveDimStyle;   ///< 原来激活的标注样式
};

#endif //DIMENSIONSTYLETABLECMD_H
