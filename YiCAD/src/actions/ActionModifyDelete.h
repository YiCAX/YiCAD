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

/// @file ActionModifyDelete.h
/// @brief 删除选中实体的交互动作类声明

#ifndef ACTIONMODIFYDELETE_H
#define ACTIONMODIFYDELETE_H

#include "ActionInterface.h"

/// @brief 删除选中实体的交互动作
///
/// 处理用户删除选定实体的操作，选中实体后直接执行删除逻辑。
class ActionModifyDelete : public ActionInterface
{
    Q_OBJECT
public:
    /// @brief 动作状态枚举
    enum Status
    {
        Acknowledge ///< 确认或取消
    };

public:
    /// @brief 构造函数
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    ActionModifyDelete(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化动作
    /// @param [in] status 初始状态，默认为0
    void init(int status = 0) override;

    /// @brief 触发删除操作，调用Modification::remove()执行
    void trigger() override;

    /// @brief 更新鼠标按键提示信息
    void updateMouseButtonHints() override;

    /// @brief 更新鼠标光标为删除样式
    void updateMouseCursor() override;
};

#endif // ACTIONMODIFYDELETE_H
