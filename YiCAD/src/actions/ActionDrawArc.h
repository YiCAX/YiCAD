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

/// @file ActionDrawArc.h
/// @brief 通过圆心、半径和起始/终止角度绘制圆弧的交互动作类

#ifndef ACTIONDRAWARC_H
#define ACTIONDRAWARC_H

#include "PreviewActionInterface.h"

class DmArc;

/// @brief 通过圆心、半径、起始角和终止角绘制简单圆弧的交互动作类
class ActionDrawArc : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief 交互状态枚举
    enum Status
    {
        SetCenter, /**< 设置圆心 */
        SetRadius, /**< 设置半径 */
        SetAngle1, /**< 设置起始角度 */
        ArcAngle   /**< 设置圆弧角度 */
    };

public:
    ActionDrawArc(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawArc() override;

    void reset();

    void init(int status = 0) override;
    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;
    void commandEvent(GuiCommandEvent* e) override;

    void hideOptions() override;
    void showOptions() override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

    bool isClockwise() const;
    void setClockwise(bool clockwise);

private:
    /// @brief 设置起始角度
    /// @param mouse 鼠标位置
    void setStartAngle(const DmVector& mouse);

    /// @brief 设置终止角度
    /// @param mouse 鼠标位置
    void setEndAngle(const DmVector& mouse);

protected:
    std::unique_ptr<DmArc> tempArc; ///< 绘制过程中临时的圆弧，其法向根据选项可能是负的
};

#endif
