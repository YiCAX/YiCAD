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

/// @file DocumentCmd.h
/// @brief 文档级操作命令，如修改当前画笔

#ifndef DOCUMENTCMD_H
#define DOCUMENTCMD_H

#include "Cmd.h"
#include "DmPen.h"

/// @brief 修改文档当前画笔命令
class ModifyDocPenCmd : public ICmd
{
public:
    /// @brief 构造修改画笔命令
    /// @param doc 目标文档
    /// @param newPen 新画笔
    /// @param originPen 原始画笔
    ModifyDocPenCmd(DmDocument* doc, const DmPen& newPen, const DmPen& originPen);
    void execute() override;
    void undo() override;
    void redo() override;

    void clear() override{}
    virtual CmdType getCmdType() const {return CmdType::ActivePenModify;}
protected:
    DmDocument* m_doc;          ///< 目标文档
    DmPen   m_originPen;        ///< 原始画笔
    DmPen   m_newPen;           ///< 新画笔
};

#endif //DOCUMENTCMD_H
