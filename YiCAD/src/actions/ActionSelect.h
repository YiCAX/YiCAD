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

/// @file ActionSelect.h
/// @brief 实体选择操作，作为选择动作的入口，将实际选择委托给子Action（单选或框选）

#ifndef ACTIONSELECT_H
#define ACTIONSELECT_H

#include <set>

#include "ActionInterface.h"
#include "UIActionHandler.h"

/// @brief 实体选择操作，作为选择动作的入口，将实际选择委托给子Action（单选或框选）
class ActionSelect : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] a_handler UI动作处理器指针
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    /// @param [in] nextAction 选择完成后要执行的后续动作类型
    /// @param [in] entityTypeList 允许选择的实体类型列表，为空则允许所有类型
    ActionSelect(
        UIActionHandler* a_handler, DmDocument* doc, GuiDocumentView* docView,
        DM::ActionType nextAction,
        std::list<DM::EntityType> const& entityTypeList = std::list<DM::EntityType>());

    /// @brief 初始化操作状态
    /// @param [in] status 初始状态值
    void init(int status) override;

    /// @brief 恢复操作
    void resume() override;

    /// @brief 处理鼠标释放事件
    /// @param [in] e 鼠标事件指针
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标按钮提示文本
    void updateMouseButtonHints() override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

    /// @brief 获取已选中的实体数量
    /// @return 已选中的实体数量；若为0则显示提示消息
    int countSelected();

    /// @brief 处理键盘按键事件
    /// @param [in] e 键盘事件指针
    void keyPressEvent(QKeyEvent* e) override;

private:
    std::list<DM::EntityType> const entityTypeList;     ///< 允许选择的实体类型列表
    DM::ActionType nextAction;                          ///< 选择完成后要执行的动作类型
    UIActionHandler* action_handler;                    ///< UI动作处理器指针
};

#endif
