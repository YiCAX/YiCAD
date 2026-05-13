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

/// @file ActionDrawXline.h
/// @brief 构造线绘制 Action 类，处理用户事件以绘制构造线

#ifndef ACTIONDRAWXLINE_H
#define ACTIONDRAWXLINE_H

#include "PreviewActionInterface.h"
#include "DmXline.h"

/// @brief 处理构造线绘制的用户交互 Action
class ActionDrawXline : public PreviewActionInterface
{
public:
    /// @brief Action 状态枚举
    enum Status
    {
        SetBasePoint,  ///< 设置基点
        SetDir         ///< 设置方向
    };

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionDrawXline(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawXline() override;

    /// @brief 重置数据
    void reset();

    /// @brief 初始化 Action 状态
    /// @param status 初始状态
    void init(int status = 0) override;
    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;
    void commandEvent(GuiCommandEvent* e) override;
    QStringList getAvailableCommands() override;

    void showOptions() override;
    void hideOptions() override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

protected:
    std::unique_ptr<XLineData> data;
};

#endif // ACTIONDRAWXLINE_H
