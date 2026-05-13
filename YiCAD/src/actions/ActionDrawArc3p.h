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

/// @file ActionDrawArc3p.h
/// @brief 通过三点绘制圆弧的交互动作类

#ifndef ACTIONDRAWARC3P_H
#define ACTIONDRAWARC3P_H

#include "PreviewActionInterface.h"

/// @brief 通过三个给定点绘制圆弧的交互动作类
class ActionDrawArc3P : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief 交互状态枚举
    enum Status
    {
        SetPoint1, /**< 设置第一个点 */
        SetPoint2, /**< 设置第二个点 */
        SetPoint3  /**< 设置第三个点 */
    };

public:
    ActionDrawArc3P(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawArc3P() override;

    void reset();

    void init(int status = 0) override;

    void trigger() override;
    void preparePreview();

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;
    void commandEvent(GuiCommandEvent* e) override;
    QStringList getAvailableCommands() override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

protected:
    /// @brief 定义圆弧的三点数据
    struct Points;
    std::unique_ptr<Points> pPoints; ///< 三点数据指针
};

#endif
