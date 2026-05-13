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

/// @file ActionDrawEllipseAxis.h
/// @brief 通过中心和轴端点绘制椭圆/椭圆弧的交互动作类

#ifndef ACTIONDRAWELLIPSEAXIS_H
#define ACTIONDRAWELLIPSEAXIS_H

#include "PreviewActionInterface.h"

/// @brief 通过指定中心点、长轴端点和短轴比绘制椭圆或椭圆弧
class ActionDrawEllipseAxis : public PreviewActionInterface
{
    Q_OBJECT
public:
    /// @brief 动作状态枚举
    enum Status
    {
        SetCenter, ///< 设置中心点
        SetMajor,  ///< 设置长轴端点
        SetMinor,  ///< 设置短轴比
        SetAngle1, ///< 设置起始角度
        SetAngle2  ///< 设置结束角度
    };

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    /// @param isArc 为 true 创建椭圆弧，为 false 创建完整椭圆
    ActionDrawEllipseAxis(DmDocument* doc, GuiDocumentView* docView, bool isArc);
    ~ActionDrawEllipseAxis() override;

    void init(int status = 0) override;
    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;
    void commandEvent(GuiCommandEvent* e) override;
    QStringList getAvailableCommands() override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

protected:
    struct Points;
    std::unique_ptr<Points> pPoints;
};

#endif
