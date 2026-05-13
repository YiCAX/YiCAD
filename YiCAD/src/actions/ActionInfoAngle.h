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

/// @file ActionInfoAngle.h
/// @brief 角度测量 Action 类，处理用户测量两条线之间角度的交互事件

#ifndef ACTIONINFOANGLE_H
#define ACTIONINFOANGLE_H

#include "PreviewActionInterface.h"

/// @brief 角度测量 Action，通过选择两条线来计算夹角
class ActionInfoAngle : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief Action 状态枚举
    enum Status
    {
        SetEntity1, ///< 选择第一条线
        SetEntity2  ///< 选择第二条线
    };

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionInfoAngle(DmDocument* doc, GuiDocumentView* docView);
    ~ActionInfoAngle() override;

    /// @brief 初始化 Action 状态
    /// @param status 初始状态
    void init(int status) override;

    void trigger() override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

private:
    DmEntity* entity1;                  ///< 第一条线实体
    DmEntity* entity2;                  ///< 第二条线实体
    DmEntity* prevHighlighted = nullptr; ///< 上次高亮的实体

    void unhighlightEntity();           ///< 取消当前高亮实体

    struct Points;
    std::unique_ptr<Points> pPoints;    ///< 交点数据
};

#endif // ACTIONINFOANGLE_H
