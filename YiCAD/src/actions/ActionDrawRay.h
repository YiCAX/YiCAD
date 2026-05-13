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

/// @file ActionDrawRay.h
/// @brief 射线绘制动作类，处理用户交互创建射线实体

#ifndef ACTIONDRAWRAY_H
#define ACTIONDRAWRAY_H

#include "PreviewActionInterface.h"
#include "DmRay.h"

class ActionDrawRay : public PreviewActionInterface
{
public:
    enum Status
    {
        SetBasePoint, ///< 设置基点
        SetDir         ///< 设置方向
    };

    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionDrawRay(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 析构函数
    ~ActionDrawRay() override;

    /// @brief 重置射线数据
    void reset();

    /// @brief 初始化动作
    /// @param status 初始状态 
    void init(int status = 0) override;

    /// @brief 触发射线创建，将射线实体添加到文档中
    void trigger() override;

    /// @brief 鼠标移动事件处理，实时预览射线方向
    /// @param e 鼠标事件
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 鼠标释放事件处理，确认基点或方向
    /// @param e 鼠标事件
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 坐标输入事件处理
    /// @param e 坐标事件
    void coordinateEvent(GuiCoordinateEvent* e) override;

    /// @brief 命令行事件处理
    /// @param e 命令事件
    void commandEvent(GuiCommandEvent* e) override;

    /// @brief 获取可用命令列表
    /// @return 可用命令字符串列表
    QStringList getAvailableCommands() override;

    /// @brief 显示选项面板
    void showOptions() override;

    /// @brief 隐藏选项面板
    void hideOptions() override;

    /// @brief 更新鼠标按钮提示文本
    void updateMouseButtonHints() override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

protected:
    std::unique_ptr<RayData> m_data; ///< 射线数据
};

#endif // ACTIONDRAWRAY
