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

/// @file ActionDrawCloudLinePolygon.h
/// @brief 通过控制点绘制多边形修订云线的交互动作类

#ifndef ACTIONDRAWCLOUDLINEPOLYGON_H
#define ACTIONDRAWCLOUDLINEPOLYGON_H

#include "PreviewActionInterface.h"
#include <QList>

class DmPolyline;
class DmVector;

/// @brief 通过控制点生成多边形修订云线
class ActionDrawCloudLinePolygon : public PreviewActionInterface
{
    Q_OBJECT
public:
    /// @brief 动作状态枚举
    enum Status
    {
        SetStartPoint, ///< 设置起点
        SetEndPoint    ///< 设置后续控制点
    };

public:
    ActionDrawCloudLinePolygon(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawCloudLinePolygon() override;

    void reset();

    /// @brief 根据控制点创建云线
    /// @param container[in] 放置云线的容器
    /// @param polyPts[in] 控制点。如果控制点为2个，云线不闭合；如果控制点为3个及以上，云线自动闭合
    /// @return 云线创建成功返回云线，否则返回nullptr
    DmPolyline* calculateCloudPoly(DmEntityContainer* container, const std::vector<DmVector>& polyPts);

    void init(int status = 0) override;
    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void keyPressEvent(QKeyEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;
    void commandEvent(GuiCommandEvent* e) override;
    QStringList getAvailableCommands() override;

    void showOptions() override;
    void hideOptions() override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

    void setMaxLength(double maxLength);
    void setMinLength(double minLength);
    double getMaxLength();
    double getMinLength();
    void undo();

    /// @brief 绘制云线预览
    /// @param container[in] 云线所在容器
    void drawPoly(DmEntityContainer* container);

protected:
    double m_minArcLen; ///< 最小弧长
    double m_maxArcLen; ///< 最大弧长

    struct Points;
    std::unique_ptr<Points> pPoints;
};

#endif
