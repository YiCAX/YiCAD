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

/// @file DmBlockTableListener.h
/// @brief 块表监听器接口，用于更新通知和刷新UI

#ifndef DMBLOCKTABLELISTENER_H
#define DMBLOCKTABLELISTENER_H

#include "DmBlock.h"

/// @brief 块表监听器
/// 用于更新通知、刷新UI等
class DmBlockTableListener
{
public:
    DmBlockTableListener() {}
    virtual ~DmBlockTableListener() {}

    /// @brief 当激活的块发生变化时调用
    virtual void blockActivated(DmBlock*) {}

    /// @brief 当新块添加到列表时调用
    virtual void blockAdded(DmBlock*) {}

    /// @brief 当块从列表中移除时调用
    virtual void blockRemoved(DmBlock*) {}

    /// @brief 当块的属性被修改时调用
    virtual void blockEdited(DmBlock*) {}

    /// @brief 当块的可见性被切换时调用
    virtual void blockToggled(DmBlock*) {}

    /// @brief 当块列表被修改时调用
    virtual void blockListModified(bool) {}
};

#endif
