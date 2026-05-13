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

/// @file ActionInfoDist.h
/// @brief 距离测量 Action 类，处理用户测量两点之间距离的交互事件

#ifndef ACTIONINFODIST_H
#define ACTIONINFODIST_H

#include "PreviewActionInterface.h"

/// @brief 距离测量 Action，通过选择两个点来计算距离
class ActionInfoDist : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief Action 状态枚举
    enum Status
    {
        SetPoint1, ///< 设置第一个点
        SetPoint2  ///< 设置第二个点
    };

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionInfoDist(DmDocument* doc, GuiDocumentView* docView);
    ~ActionInfoDist() override;

    /// @brief 初始化 Action 状态
    /// @param status 初始状态
    void init(int status = 0) override;
    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

private:
    struct Points;
    std::unique_ptr<Points> pPoints; ///< 两点数据
};

#endif // ACTIONINFODIST_H
