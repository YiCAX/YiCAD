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

/// @file ActionLayersFreezeAll.h
/// @brief 冻结/解冻所有图层的交互动作类
#ifndef ACTIONLAYERSFREEZEALL_H
#define ACTIONLAYERSFREEZEALL_H

#include "ActionInterface.h"

/// @brief 冻结/解冻所有图层的动作类
///
/// 处理用户冻结或解冻文档中所有图层的交互动作。
/// 通过 @p freeze 参数控制是冻结还是解冻操作。
class ActionLayersFreezeAll : public ActionInterface
{
    Q_OBJECT

public:

    /// @brief 构造函数
    /// @param freeze 是否冻结所有图层（true 为冻结，false 为解冻）
    /// @param doc CAD 文档指针
    /// @param docView 文档视图指针
    ActionLayersFreezeAll(const bool freeze, DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化动作并立即触发
    /// @param status 初始状态（默认为 0）
    void init(int status = 0) override;

    /// @brief 执行冻结/解冻所有图层的操作
    void trigger() override;

private:
    bool freeze; ///< 是否冻结（true 为冻结，false 为解冻）
};

#endif // ACTIONLAYERSFREEZEALL_H
