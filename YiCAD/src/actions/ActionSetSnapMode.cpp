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

/// @file ActionSetSnapMode.cpp
/// @brief 设置捕捉模式的Action实现

#include "ActionSetSnapMode.h"

#include "GuiDocumentView.h"

/// @brief 构造函数，设置新的捕捉模式
/// @param [in] doc 文档指针
/// @param [in] docView 文档视图指针
/// @param [in] snapMode 新的捕捉模式
ActionSetSnapMode::ActionSetSnapMode(DmDocument* doc,
                                     GuiDocumentView* docView,
                                     DM::SnapMode snapMode) :
    ActionInterface("Set Snap Mode", doc, docView)
{
    this->snapMode = snapMode;
}

/// @brief 初始化操作状态，立即触发捕捉模式切换
/// @param [in] status 初始状态值
void ActionSetSnapMode::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 执行捕捉模式切换，根据当前模式更新捕捉设置
void ActionSetSnapMode::trigger()
{
    SnapMode s = docView->getDefaultSnapMode();

    switch (snapMode)
    {
        case DM::SnapFree:
            s.clear();
            break;

        case DM::SnapCenter:
            s.snapCenter = !s.snapCenter;
            break;

        case DM::SnapEndpoint:
            s.snapEndpoint = !s.snapEndpoint;
            break;

        case DM::SnapGrid:
            s.snapGrid = !s.snapGrid;
            break;

        case DM::SnapOnEntity:
            s.snapOnEntity = !s.snapOnEntity;
            break;

        case DM::SnapMiddle:
            s.snapMiddle = !s.snapMiddle;
            break;

        case DM::SnapIntersection:
            s.snapIntersection = !s.snapIntersection;
            break;

        default:
            break;
    }

    docView->setDefaultSnapMode(s);

    finish(false);
}
