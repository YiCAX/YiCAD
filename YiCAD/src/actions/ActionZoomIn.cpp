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

/// @file ActionZoomIn.cpp
/// @brief 缩放Action实现

#include "ActionZoomIn.h"

#include <QAction>

#include "GuiDocumentView.h"

/// @brief 构造函数，初始化缩放参数
/// @param [in] doc 文档指针
/// @param [in] docView 文档视图指针
/// @param [in] direction 缩放方向（In放大/Out缩小）
/// @param [in] axis 受影响的轴向
/// @param [in] pCenter 缩放中心点
/// @param [in] factor 缩放因子
ActionZoomIn::ActionZoomIn(DmDocument* doc, GuiDocumentView* docView,
                           DM::ZoomDirection direction,
                           DM::Axis axis, DmVector const* pCenter,
                           double factor) :
    ActionInterface("Zoom in", doc, docView),
    zoom_factor(factor), direction(direction), axis(axis),
    center(pCenter ? new DmVector{*pCenter} : new DmVector{})
{
}

ActionZoomIn::~ActionZoomIn() = default;

/// @brief 初始化操作状态，立即执行缩放
/// @param [in] status 初始状态值
void ActionZoomIn::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 执行缩放操作，根据轴向和方向调用相应的缩放方法
void ActionZoomIn::trigger()
{
    switch (axis)
    {
        case DM::Both:
            if (direction == DM::In)
            {
                docView->zoomIn(zoom_factor, *center);
            }
            else
            {
                docView->zoomOut(zoom_factor, *center);
            }
            break;

        default:
            break;
    }
    finish(false);
}
