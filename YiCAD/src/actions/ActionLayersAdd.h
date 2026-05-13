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

/// @file ActionLayersAdd.h
/// @brief 图层添加 Action 类，处理用户添加新图层的交互

#ifndef ACTIONLAYERSADD_H
#define ACTIONLAYERSADD_H

#include "ActionInterface.h"

/// @brief 图层添加 Action，弹出新建图层对话框并添加图层
class ActionLayersAdd : public ActionInterface
{
    Q_OBJECT
public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionLayersAdd(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化并立即触发
    /// @param status 初始状态
    void init(int status = 0) override;

    /// @brief 触发图层添加操作
    void trigger() override;

private:
};

#endif
