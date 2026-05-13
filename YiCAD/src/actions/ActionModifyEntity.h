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

/// @file ActionModifyEntity.h
/// @brief 修改实体属性的交互动作类声明

#ifndef ACTIONMODIFYENTITY_H
#define ACTIONMODIFYENTITY_H

#include "ActionInterface.h"

/// @brief 修改实体属性的交互动作
///
/// 处理用户点击实体进行属性修改的操作，支持多行文字等特殊实体类型的处理。
class ActionModifyEntity : public ActionInterface
{
    Q_OBJECT
public:
    /// @brief 构造函数
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    ActionModifyEntity(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 触发修改操作
    void trigger() override;

    /// @brief 鼠标释放事件处理
    /// @param [in] e 鼠标事件指针
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

    /// @brief 更新鼠标按键提示信息
    void updateMouseButtonHints() override;

private:
    DmEntity* m_currentEntity = nullptr; ///< 当前拾取的实体指针
};

#endif // ACTIONMODIFYENTITY_H
