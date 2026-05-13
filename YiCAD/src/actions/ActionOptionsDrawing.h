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

/// @file ActionOptionsDrawing.h
/// @brief 绘图选项交互命令头文件

#ifndef ACTIONOPTIONSDRAWING_H
#define ACTIONOPTIONSDRAWING_H

#include "ActionInterface.h"

/// @brief 绘图选项交互命令
///
/// 打开绘图选项设置对话框，用户可在此修改与绘图相关的配置参数。
class ActionOptionsDrawing : public ActionInterface
{
    Q_OBJECT

public:
    ActionOptionsDrawing(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化并立即触发命令
    /// @param [in] status 初始状态，默认为0
    void init(int status = 0) override;

    /// @brief 触发绘图选项对话框显示
    void trigger() override;
};

#endif
