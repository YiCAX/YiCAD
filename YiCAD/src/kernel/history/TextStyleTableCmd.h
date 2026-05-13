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

/// @file TextStyleTableCmd.h
/// @brief 文字样式表相关的 Undo/Redo 命令类

#ifndef TEXTSTYLETABLECMD_H
#define TEXTSTYLETABLECMD_H

#include "Cmd.h"
#include "DmTextStyle.h"
#include <string>

class DmTextStyleTable;
class DmTextStyle;

/// @brief 文字样式表添加命令
class TextStyleTableAddCmd : public ICmd
{
public:
    /// @brief 构造函数
    /// @param [in] table 文字样式表指针
    /// @param [in] addedTextStyle 被添加的文字样式指针
    TextStyleTableAddCmd(DmTextStyleTable* table, DmTextStyle* addedTextStyle);

    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;

    DmObject* getObject() override
    {
        return m_addedTextStyle;
    }

    CmdType getCmdType() const override
    {
        return CmdType::TextStyleTableAddCmd;
    }

protected:
    DmTextStyleTable* m_table = nullptr;          ///< 文字样式表指针
    DmTextStyle* m_addedTextStyle = nullptr;      ///< 被添加的文字样式指针
    bool m_isOwnByCommand = false;                ///< 是否由该命令所有
};


/// @brief 文字样式表修改命令
class TextStyleTableModifyCmd : public ICmd
{
public:
    TextStyleTableModifyCmd()
        : m_table(nullptr)
        , m_modifiedTextStyle(nullptr)
    {
    }

    /// @brief 构造函数
    /// @param [in] table 文字样式表指针
    /// @param [in] modifiedTextStyle 被修改的文字样式指针
    TextStyleTableModifyCmd(DmTextStyleTable* table, DmTextStyle* modifiedTextStyle);

    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;

    DmObject* getObject() override
    {
        return m_modifiedTextStyle;
    }

    CmdType getCmdType() const override
    {
        return CmdType::TextStyleTableModifyCmd;
    }

protected:
    std::string m_originData;                     ///< 原始序列化数据
    std::string m_newData;                        ///< 新序列化数据
    DmTextStyle* m_modifiedTextStyle = nullptr;   ///< 被修改的文字样式指针
    DmTextStyleTable* m_table = nullptr;          ///< 文字样式表指针
};


/// @brief 文字样式表移除命令
class TextStyleTableRemoveCmd : public ICmd
{
public:
    /// @brief 构造函数
    /// @param [in] table 文字样式表指针
    /// @param [in] removedTextStyle 被移除的文字样式指针
    TextStyleTableRemoveCmd(DmTextStyleTable* table, DmTextStyle* removedTextStyle);

    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;

    DmObject* getObject() override
    {
        return m_removedTextStyle;
    }

    CmdType getCmdType() const override
    {
        return CmdType::TextStyleTableRemoveCmd;
    }

protected:
    DmTextStyleTable* m_table = nullptr;          ///< 文字样式表指针
    DmTextStyle* m_removedTextStyle = nullptr;    ///< 被移除的文字样式指针
};


/// @brief 文字样式表激活命令
class TextStyleTableActivateCmd : public ICmd
{
public:
    /// @brief 构造函数
    /// @param [in] table 文字样式表指针
    /// @param [in] activatedTextStyle 被激活的文字样式指针
    TextStyleTableActivateCmd(DmTextStyleTable* table, DmTextStyle* activatedTextStyle);

    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;

    CmdType getCmdType() const override
    {
        return CmdType::TextStyleTableActivateCmd;
    }

protected:
    DmTextStyleTable* m_table = nullptr;              ///< 文字样式表指针
    DmTextStyle* m_activatedTextStyle = nullptr;      ///< 被激活的文字样式指针
    DmTextStyle* m_originActiveTextStyle = nullptr;   ///< 原始激活的文字样式指针
};

#endif //TEXTSTYLETABLECMD_H
