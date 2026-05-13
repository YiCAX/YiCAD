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

/// @file ActionSetSnapRestriction.h
/// @brief 设置附加捕捉约束模式的Action

#ifndef ACTIONSETSNAPRESTRICTION_H
#define ACTIONSETSNAPRESTRICTION_H

#include "ActionInterface.h"

/// @brief 切换当前附加捕捉约束模式的Action
class ActionSetSnapRestriction : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    /// @param [in] snapRes 要设置的捕捉约束模式
    ActionSetSnapRestriction(DmDocument* doc, GuiDocumentView* docView,
                             DM::SnapRestriction snapRes);

    /// @brief 初始化操作状态
    /// @param [in] status 初始状态值，默认为0
    void init(int status = 0) override;

    /// @brief 执行捕捉约束切换
    void trigger() override;

protected:
    DM::SnapRestriction snapRes; ///< 要设置的捕捉约束模式
};

#endif // ACTIONSETSNAPRESTRICTION_H
