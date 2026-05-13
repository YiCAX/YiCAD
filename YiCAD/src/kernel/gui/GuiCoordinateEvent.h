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

/// @file GuiCoordinateEvent.h
/// @brief 坐标事件类，封装坐标输入事件

#ifndef GUICOORDINATEEVENT_H
#define GUICOORDINATEEVENT_H

#include "DmVector.h"

/// @brief 坐标事件
class GuiCoordinateEvent
{
public:
    /// @brief 构造坐标事件
    /// @param pos 坐标位置
    GuiCoordinateEvent(const DmVector& pos) : pos(pos) {}

    /// @brief 获取事件中的实际文档坐标
    /// @return 事件发生时的位置坐标
    DmVector getCoordinate()
    {
        return pos;
    }

protected:
    DmVector pos;
};

#endif
