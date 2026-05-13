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

/// @file DocumentCmd.cpp
/// @brief 文档级操作命令实现

#include "DocumentCmd.h"
#include "DmDocument.h"

ModifyDocPenCmd::ModifyDocPenCmd(DmDocument *doc, const DmPen &newPen, const DmPen &originPen)
:m_doc(doc)
,m_newPen(newPen)
,m_originPen(originPen)
{

}

/// @brief 执行修改画笔
void ModifyDocPenCmd::execute()
{
    if (m_isExecuted)
    {
        return;
    }
    m_doc->setActivePen(m_newPen);
    ICmd::execute();
}

/// @brief 撤销修改画笔
void ModifyDocPenCmd::undo()
{
    if (!m_isExecuted)
    {
        return;
    }
    m_doc->setActivePen(m_originPen);
    ICmd::undo();
}

/// @brief 重做修改画笔
void ModifyDocPenCmd::redo()
{
    if (m_isExecuted)
    {
        return;
    }
    m_doc->setActivePen(m_newPen);
    ICmd::redo();
}
