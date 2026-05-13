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

/// @file ActionSetSnapMode.h
/// @brief 设置捕捉模式的Action

#ifndef ACTIONSETSNAPMODE_H
#define ACTIONSETSNAPMODE_H

#include "ActionInterface.h"

/// @brief 切换当前捕捉模式的Action
class ActionSetSnapMode : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    /// @param [in] snapMode 要切换的捕捉模式
    ActionSetSnapMode(DmDocument* doc, GuiDocumentView* docView,
                      DM::SnapMode snapMode);

    /// @brief 初始化操作状态
    /// @param [in] status 初始状态值，默认为0
    void init(int status = 0) override;

    /// @brief 执行捕捉模式切换
    void trigger() override;

protected:
    DM::SnapMode snapMode; ///< 要切换的捕捉模式
};

#endif // ACTIONSETSNAPMODE_H
