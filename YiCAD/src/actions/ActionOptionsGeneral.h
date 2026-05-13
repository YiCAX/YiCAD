/*
 * Copyright (C) 2026 YiCAD Community
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

/// @file ActionOptionsGeneral.h
/// @brief 通用选项交互命令头文件

#ifndef ACTIONOPTIONSGENERAL_H
#define ACTIONOPTIONSGENERAL_H

#include "ActionInterface.h"

/// @brief 通用选项交互命令
///
/// 打开通用选项设置对话框，用户可在此修改全局应用配置（如颜色方案等）。
class ActionOptionsGeneral : public ActionInterface
{
    Q_OBJECT

public:
    ActionOptionsGeneral(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化并立即触发命令
    /// @param [in] status 初始状态，默认为0
    void init(int status = 0) override;

    /// @brief 触发通用选项对话框显示，并应用颜色配置到所有打开的视图
    void trigger() override;
};

#endif //!ACTIONOPTIONSGENERAL_H
