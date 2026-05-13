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

/// @file ActionDrawCircle3p.h
/// @brief 通过三个点绘制圆的交互动作类

#ifndef ACTIONDRAWCIRCLE3P_H
#define ACTIONDRAWCIRCLE3P_H

#include "PreviewActionInterface.h"

struct DmCircleData;

/// @brief 通过给定的三个点绘制圆的交互动作
class ActionDrawCircle3P : public PreviewActionInterface
{
    Q_OBJECT
public:
    /// @brief 动作状态枚举
    enum Status
    {
        SetPoint1, ///< 设置第一个点
        SetPoint2, ///< 设置第二个点
        SetPoint3  ///< 设置第三个点
    };

public:
    ActionDrawCircle3P(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawCircle3P() override;

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
    /// @brief 已定义的圆数据点
    struct Points;
    std::unique_ptr<Points> pPoints;
};

#endif
