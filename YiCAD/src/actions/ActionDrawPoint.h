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

/// @file ActionDrawPoint.h
/// @brief 绘制点动作类，处理用户鼠标事件以在文档中绘制点实体

#ifndef ACTIONDRAWPOINT_H
#define ACTIONDRAWPOINT_H

#include "PreviewActionInterface.h"

/// @brief 绘制点动作类
///
/// 继承自 PreviewActionInterface，处理鼠标、键盘和坐标事件，
/// 在文档中创建 DmPoint 点实体。
class ActionDrawPoint : public PreviewActionInterface
{
    Q_OBJECT
public:
    /// @brief 构造函数
    /// @param doc 文档对象指针
    /// @param docView 文档视图对象指针
    ActionDrawPoint(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 析构函数
    ~ActionDrawPoint() override;

    /// @brief 触发绘制操作，在文档中创建点实体
    void trigger() override;

    /// @brief 鼠标移动事件处理
    /// @param e 鼠标事件对象指针
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 鼠标释放事件处理
    /// @param e 鼠标事件对象指针
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 坐标输入事件处理
    /// @param e 坐标事件对象指针
    void coordinateEvent(GuiCoordinateEvent* e) override;

    /// @brief 命令输入事件处理
    /// @param e 命令事件对象指针
    void commandEvent(GuiCommandEvent* e) override;

    /// @brief 获取当前动作可用的命令列表
    /// @return 可用命令字符串列表
    QStringList getAvailableCommands() override;

    /// @brief 更新鼠标按钮提示文本
    void updateMouseButtonHints() override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

private:
    std::unique_ptr<DmVector> m_pt; ///< 绘制点坐标向量，管理点位置数据
};

#endif
