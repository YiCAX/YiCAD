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

/// @file ActionDrawCircle.h
/// @brief 通过圆心和半径绘制圆的交互动作类

#ifndef ACTIONDRAWCIRCLE_H
#define ACTIONDRAWCIRCLE_H

#include "PreviewActionInterface.h"

class CircleData;

/// @brief 通过给定圆心和圆上一点绘制圆的交互动作类
class ActionDrawCircle : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief 交互状态枚举
    enum Status
    {
        SetCenter, /**< 设置圆心 */
        SetRadius  /**< 设置半径 */
    };

public:
    ActionDrawCircle(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawCircle() override;

    void reset();

    void init(int status = 0) override;

    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;
    void commandEvent(GuiCommandEvent* e) override;
    QStringList getAvailableCommands() override;

    void hideOptions() override;
    void showOptions() override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

protected:
    std::unique_ptr<CircleData> data; ///< 已定义的圆数据
};

#endif
