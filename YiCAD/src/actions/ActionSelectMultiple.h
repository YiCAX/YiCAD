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

/// @file ActionSelectMultiple.h
/// @brief 支持单选与框选的选择Action，支持Shift组合键切换选择/取消选择模式

#ifndef ACTIONSELECTMULTIPLE_H
#define ACTIONSELECTMULTIPLE_H

#include <memory>

#include "PreviewActionInterface.h"

/// @brief 支持单选与框选的选择Action（主要参考ActionDefault）
class ActionSelectMultiple : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief 操作状态枚举
    enum Status
    {
        Neutral,    ///< 初始状态，等待用户操作
        Dragging,   ///< 正在拖拽（拖拽实体或选择窗口第一角）
        SetCorner2, ///< 设置选择窗口的第二角
    };

    /// @brief 选择窗口的两个角点
    struct Points
    {
        DmVector v1;    ///< 第一角点坐标
        DmVector v2;    ///< 第二角点坐标
    };

public:
    /// @brief 构造函数
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    /// @param [in] actionSelect 调用此选择动作的父Action指针
    /// @param [in] entityTypeList 允许选择的实体类型列表，为空则允许所有类型
    ActionSelectMultiple(DmDocument* doc, GuiDocumentView* docView,
                         ActionInterface* actionSelect = nullptr,
                         std::list<DM::EntityType> const& entityTypeList =
                             std::list<DM::EntityType>{});

    /// @brief 初始化操作状态
    /// @param [in] status 初始状态值，默认为0
    void init(int status = 0) override;

    /// @brief 执行选择操作（窗口选择或结束操作）
    void trigger() override;

    /// @brief 处理键盘按键事件
    /// @param [in] e 键盘事件指针
    void keyPressEvent(QKeyEvent* e) override;

    /// @brief 处理鼠标移动事件
    /// @param [in] e 鼠标事件指针
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 处理鼠标按下事件
    /// @param [in] e 鼠标事件指针
    void mousePressEvent(QMouseEvent* e) override;

    /// @brief 处理鼠标释放事件
    /// @param [in] e 鼠标事件指针
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标按钮提示文本
    void updateMouseButtonHints() override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

private:
    std::list<DM::EntityType> const entityTypeList;     ///< 允许选择的实体类型列表
    ActionInterface* actionSelect = nullptr;            ///< 调用此Action的父Action指针
    bool select = true;                                 ///< 选择模式（true为选择，false为取消选择）
    std::unique_ptr<Points> pPoints;                    ///< 选择窗口角点数据
};

#endif // ACTIONSELECTMULTIPLE_H
