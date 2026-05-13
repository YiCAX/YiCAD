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

/// @file ActionDrawCircleTan2.h
/// @brief 绘制与两个给定圆相切且具有指定半径的圆的交互动作类

#ifndef ACTIONDRAWCIRCLETAN2_H
#define ACTIONDRAWCIRCLETAN2_H

#include "PreviewActionInterface.h"

class DmAtomicEntity;
struct DmCircleData;

/// @brief 绘制与两个给定圆/线相切且具有指定半径的圆
class ActionDrawCircleTan2 : public PreviewActionInterface
{
    Q_OBJECT
public:
    /// @brief 动作状态枚举
    enum Status
    {
        SetCircle1, ///< 选择第一个圆/线
        SetCircle2, ///< 选择第二个圆/线
        SetCenter   ///< 选择最近的相切圆圆心
    };

public:
    ActionDrawCircleTan2(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawCircleTan2() override;

    void init(int status = 0) override;

    void trigger() override;
    bool getCenters();
    bool preparePreview() const;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    QStringList getAvailableCommands() override;
    void finish(bool updateTB = true) override;
    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

    void showOptions() override;
    void hideOptions() override;
    void setRadius(const double& r);
    double getRadius() const;

protected:
    DmEntity* catchCircle(QMouseEvent* e);

private:
    struct Points;
    std::unique_ptr<Points> pPoints;
};

#endif
