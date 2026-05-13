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

/// @file ActionSelectSingle.h
/// @brief 单选实体Action，处理用户事件以选择单个实体

#ifndef ACTIONSELECTSINGLE_H
#define ACTIONSELECTSINGLE_H

#include "ActionInterface.h"

/// @brief 单选实体Action类，处理用户事件以选择单个实体
class ActionSelectSingle : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    /// @param [in] actionSelect 调用此选择动作的父Action指针
    /// @param [in] entityTypeList 允许选择的实体类型列表，为空则允许所有类型
    ActionSelectSingle(DmDocument* doc, GuiDocumentView* docView,
                       ActionInterface* actionSelect = nullptr,
                       std::list<DM::EntityType> const& entityTypeList =
                           std::list<DM::EntityType>{});

    /// @brief 执行选择操作
    void trigger() override;

    /// @brief 处理键盘按键事件
    /// @param [in] e 键盘事件指针
    void keyPressEvent(QKeyEvent* e) override;

    /// @brief 处理鼠标释放事件
    /// @param [in] e 鼠标事件指针
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

private:
    std::list<DM::EntityType> const entityTypeList; ///< 允许选择的实体类型列表

    DmEntity* en = nullptr;                ///< 当前捕获的实体指针
    ActionInterface* actionSelect = nullptr; ///< 调用此Action的父Action指针
};

#endif // ACTIONSELECTSINGLE_H
