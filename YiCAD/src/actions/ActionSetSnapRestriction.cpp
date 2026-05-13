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

/// @file ActionSetSnapRestriction.cpp
/// @brief 设置附加捕捉约束模式的Action实现

#include "ActionSetSnapRestriction.h"

#include "GuiDocumentView.h"

/// @brief 构造函数，设置新的附加捕捉约束模式
/// @param [in] doc 文档指针
/// @param [in] docView 文档视图指针
/// @param [in] snapRes 新的捕捉约束模式
ActionSetSnapRestriction::ActionSetSnapRestriction(DmDocument* doc,
                                                   GuiDocumentView* docView,
                                                   DM::SnapRestriction snapRes) :
    ActionInterface("Set Additional Snap Mode", doc, docView)
{
    this->snapRes = snapRes;
}

/// @brief 初始化操作状态，立即触发捕捉约束切换
/// @param [in] status 初始状态值
void ActionSetSnapRestriction::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 执行捕捉约束切换
void ActionSetSnapRestriction::trigger()
{
    docView->setSnapRestriction(snapRes);

    finish(false);
}
