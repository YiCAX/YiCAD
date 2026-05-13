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

/// @file ActionLayersLockAll.h
/// @brief 图层全部锁定/解锁 Action 类，处理锁定或解锁所有图层的用户交互

#ifndef ACTIONLAYERSLOCKALL_H
#define ACTIONLAYERSLOCKALL_H

#include "ActionInterface.h"

/// @brief 图层全部锁定/解锁 Action，处理锁定或解锁所有图层的操作
class ActionLayersLockAll : public ActionInterface
{
    Q_OBJECT
public:
    /// @brief 构造函数
    /// @param lock true 表示锁定所有图层，false 表示解锁所有图层
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionLayersLockAll(const bool lock, DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化并立即触发
    /// @param status 初始状态
    void init(int status = 0) override;

    /// @brief 触发全部锁定/解锁图层操作
    void trigger() override;

private:
    bool lock; ///< true 锁定所有图层，false 解锁所有图层
};

#endif //!ACTIONLAYERSLOCKALL_H
