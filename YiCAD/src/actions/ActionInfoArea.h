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

/// @file ActionInfoArea.h
/// @brief 面积测量 Action 类，处理用户通过多边形测量面积和周长

#ifndef ACTIONINFOAREA_H
#define ACTIONINFOAREA_H

#include "PreviewActionInterface.h"

class InfoArea;

/// @brief 面积测量 Action，通过定义多边形来测量封闭区域面积和周长
class ActionInfoArea : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief Action 状态枚举
    enum Status
    {
        SetFirstPoint, ///< 设置多边形第一个点
        SetNextPoint   ///< 设置多边形下一个点
    };

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionInfoArea(DmDocument* doc, GuiDocumentView* docView);
    ~ActionInfoArea() override;

    /// @brief 初始化 Action 状态
    /// @param status 初始状态
    void init(int status = 0) override;
    void trigger() override;
    void display();

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

private:
    std::unique_ptr<InfoArea> ia; ///< 面积信息对象
};

#endif // ACTIONINFOAREA_H
