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

/// @file ActionDrawCircle2p.h
/// @brief 通过两点绘制圆的交互动作类

#ifndef ACTIONDRAWCIRCLE2P_H
#define ACTIONDRAWCIRCLE2P_H

#include "PreviewActionInterface.h"

class CircleData;

/// @brief 通过给定两个点（直径端点）绘制圆的交互动作类
class ActionDrawCircle2P : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief 交互状态枚举
    enum Status
    {
        SetPoint1, /**< 设置第一个点 */
        SetPoint2  /**< 设置第二个点 */
    };

public:
    ActionDrawCircle2P(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawCircle2P() override;

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
    std::unique_ptr<CircleData> data; ///< 已定义的圆数据
    struct Points;
    std::unique_ptr<Points> pPoints;  ///< 两点数据
};

#endif
