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

/// @file ActionInfoTotalLength.h
/// @brief 总长度测量 Action 类，计算并显示选中实体的总长度

#ifndef ACTIONINFOTOTALLENGTH_H
#define ACTIONINFOTOTALLENGTH_H

#include "ActionInterface.h"

/// @brief 总长度测量 Action，计算选中实体的总长度
class ActionInfoTotalLength : public ActionInterface
{
    Q_OBJECT
public:
    /// @brief Action 状态枚举
    enum Status
    {
        Acknowledge ///< 确认或取消
    };

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionInfoTotalLength(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化并立即触发
    /// @param status 初始状态
    void init(int status = 0) override;

    /// @brief 触发总长度计算和显示
    void trigger() override;
};

#endif
