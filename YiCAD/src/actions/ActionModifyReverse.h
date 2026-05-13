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

/// @file ActionModifyReverse.h
/// @brief 反向修改操作——将选中实体的方向反转

#ifndef ACTIONMODIFYREVERSE_H
#define ACTIONMODIFYREVERSE_H

#include "ActionInterface.h"

/// @brief 反向修改操作类，将选中实体（线、弧、椭圆、样条曲线）的方向反转
class ActionModifyReverse : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    ActionModifyReverse(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化并直接执行反向操作
    /// @param [in] status 初始状态值
    void init(int status = 0) override;

    /// @brief 执行反向操作，反转所有选中实体的方向
    void trigger() override;

    /// @brief 更新鼠标按钮提示信息
    void updateMouseButtonHints() override;

    /// @brief 禁用拷贝构造
    ActionModifyReverse(const ActionModifyReverse&) = delete;

    /// @brief 禁用拷贝赋值
    ActionModifyReverse& operator=(const ActionModifyReverse&) = delete;
};

#endif //ACTIONMODIFYREVERSE_H
