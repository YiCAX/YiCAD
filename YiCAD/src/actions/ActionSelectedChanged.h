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

/// @file ActionSelectedChanged.h
/// @brief 选择变更Action，处理选中实体后的UI更新与特殊动作触发

#ifndef ACTIONSELECTEDCHANGED_H
#define ACTIONSELECTEDCHANGED_H

#include "ActionInterface.h"

class DmEntity;

/// @brief 选择变更Action，当选中实体发生变化时更新UI并触发相应动作
class ActionSelectedChanged : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    ActionSelectedChanged(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化操作状态
    /// @param [in] status 初始状态值，默认为0
    void init(int status = 0) override;

    /// @brief 执行选择变更处理：更新UI并触发特殊处理
    void trigger() override;

private:
    /// @brief 更新选择信息到UI
    void updateToUI();

    /// @brief 触发单选实体的特殊处理（如多行文字编辑）
    void triggleSingleSelectedAction();

private:
    /// @brief 选择数量类型：0=无选择，1=单个图层选择，2=多个图层选择
    int m_selectedNumType = 0;

    /// @brief 单选时捕获的实体指针
    DmEntity* m_singleSelectedEnt = nullptr;
};

#endif // ACTIONSELECTEDCHANGED_H
