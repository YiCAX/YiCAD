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

/// @file ActionDrawLineFree.h
/// @brief 自由手绘线交互动作类头文件

#ifndef ACTIONDRAWLINEFREE_H
#define ACTIONDRAWLINEFREE_H

#include "PreviewActionInterface.h"

class DmPolyline;

/// @brief 自由手绘线交互动作类，处理用户鼠标事件以绘制自由手绘线。
class ActionDrawLineFree : public PreviewActionInterface
{
    Q_OBJECT

public:

    /// @brief 动作状态枚举
    enum Status
    {
        SetStartpoint, ///< 设置起点
        Dragging       ///< 拖拽中
    };

    /// @brief 构造函数
    /// @param doc 文档对象指针
    /// @param docView 文档视图指针
    ActionDrawLineFree(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 析构函数
    ~ActionDrawLineFree() override;

    /// @brief 触发动作完成，结束当前绘制并将有效的折线添加到文档
    void trigger() override;

    /// @brief 鼠标移动事件处理，拖拽时根据鼠标位置追加顶点
    /// @param e 鼠标事件指针
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 鼠标按下事件处理，开始新的自由手绘
    /// @param e 鼠标事件指针
    void mousePressEvent(QMouseEvent* e) override;

    /// @brief 鼠标释放事件处理，结束当前绘制
    /// @param e 鼠标事件指针
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标按钮提示信息
    void updateMouseButtonHints() override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

protected:
    std::unique_ptr<DmVector> vertex;     ///< 上一个点
    std::unique_ptr<DmPolyline> polyline; ///< 自由手绘折线对象
};

#endif
