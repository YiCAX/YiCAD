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

/// @file ActionDimLeader.h
/// @brief 引线标注交互操作类头文件

#ifndef ACTIONDIMLEADER_H
#define ACTIONDIMLEADER_H

#include <memory>

#include "PreviewActionInterface.h"

class DmLeader;

/// @brief 处理用户事件以绘制引线标注（箭头）。
class ActionDimLeader : public PreviewActionInterface
{
    Q_OBJECT
public:
    /**
     * Action States.
     */
    enum Status
    {
        SetStartpoint, /**< Setting the startpoint.  */
        SetEndpoint    /**< Setting the endpoint. */
    };

public:
    /// @brief 构造函数
    /// @param [in] doc 文档对象指针
    /// @param [in] docView 文档视图指针
    explicit ActionDimLeader(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDimLeader() override;

    /// @brief 重置操作状态
    void reset();

    void init(int status = 0) override;
    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;
    void commandEvent(GuiCommandEvent* e) override;
    QStringList getAvailableCommands() override;
    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

protected:
    DmLeader* leader = nullptr;          ///< 引线标注实体。

    struct Points;
    std::unique_ptr<Points> pPoints;     ///< 已设置的坐标点。
};

#endif
