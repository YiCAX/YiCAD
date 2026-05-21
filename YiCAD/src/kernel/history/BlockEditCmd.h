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

/// @file BlockEditCmd.h
/// @brief 块编辑括号命令，用于包裹块编辑会话的进入和退出

#ifndef BLOCKEDITCMD_H
#define BLOCKEDITCMD_H

#include "Cmd.h"
#include <QString>

class DmDocument;
class DmBlock;
class GuiDocumentView;

/// @brief 进入块编辑命令
/// @details 将文档切换到块编辑模式，getEntityTable() 返回块的实体表
class BlockEditEnterCmd : public ICmd
{
public:
    /// @brief 构造进入块编辑命令
    /// @param doc 文档指针
    /// @param blockName 要编辑的块名称
    /// @param docView 文档视图指针
    BlockEditEnterCmd(DmDocument* doc, const QString& blockName,
                      GuiDocumentView* docView);

    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    CmdType getCmdType() const override { return CmdType::BlockEditEnterCmd; }

private:
    /// @brief 更新文档中引用此块的所有块参照
    void updateBlockRefs();

private:
    DmDocument* m_pDocument;        ///< 文档指针
    QString m_blockName;            ///< 块名称
    GuiDocumentView* m_pDocView;    ///< 文档视图指针
};

/// @brief 退出块编辑命令
/// @details 退出块编辑模式，可选择是否保存（更新块参照）
class BlockEditExitCmd : public ICmd
{
public:
    /// @brief 构造退出块编辑命令
    /// @param doc 文档指针
    /// @param blockName 块名称
    /// @param docView 文档视图指针
    /// @param save 是否保存修改（更新所有引用的块参照）
    BlockEditExitCmd(DmDocument* doc, const QString& blockName,
                     GuiDocumentView* docView, bool save);

    void execute() override;
    void undo() override;
    void redo() override;
    void clear() override;
    CmdType getCmdType() const override { return CmdType::BlockEditExitCmd; }

private:
    /// @brief 更新文档中引用此块的所有块参照
    void updateBlockRefs();

private:
    DmDocument* m_pDocument;        ///< 文档指针
    QString m_blockName;            ///< 块名称
    GuiDocumentView* m_pDocView;    ///< 文档视图指针
    bool m_save;                    ///< 是否保存修改
};

#endif // BLOCKEDITCMD_H
