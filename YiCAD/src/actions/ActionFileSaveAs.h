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

/// @file ActionFileSaveAs.h
/// @brief 文件另存为 Action 类，处理用户将文件导出为各种格式的事件

#ifndef ACTIONFILESAVEAS_H
#define ACTIONFILESAVEAS_H

#include "ActionInterface.h"

/// @brief 文件另存为 Action，独占模式，触发后执行另存为流程
class ActionFileSaveAs : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionFileSaveAs(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 判断是否为独占模式
    /// @return 始终返回 true
    bool isExclusive() override;

    /// @brief 初始化并立即触发另存为
    /// @param status 初始状态
    void init(int status = 0) override;

    /// @brief 触发文件另存为操作
    void trigger() override;
};

#endif // ACTIONFILESAVEAS_H
