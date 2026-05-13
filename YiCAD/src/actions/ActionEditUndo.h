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

/// @file ActionEditUndo.h
/// @brief 撤销/重做 Action 类，处理用户撤销和重做的事件

#ifndef ACTIONEDITUNDO_H
#define ACTIONEDITUNDO_H

#include "ActionInterface.h"

/// @brief 处理撤销和重做用户事件的 Action 类
class ActionEditUndo : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param undo true 为撤销，false 为重做
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionEditUndo(bool undo, DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化并立即执行
    /// @param status 初始状态
    void init(int status = 0) override;

    /// @brief 触发撤销或重做操作
    void trigger() override;

protected:
    const bool undo; ///< Undo (true) 或 redo (false)
};

#endif // ACTIONEDITUNDO_H
