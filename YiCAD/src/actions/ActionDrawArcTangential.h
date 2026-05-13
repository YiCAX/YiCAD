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

/// @file ActionDrawArcTangential.h
/// @brief 通过基实体和终止角度绘制切向圆弧的交互动作类

#ifndef ACTIONDRAWARCTANGENTIAL_H
#define ACTIONDRAWARCTANGENTIAL_H

#include "PreviewActionInterface.h"

class DmAtomicEntity;
class DmArc;

/// @brief 通过选择基实体并指定终止点来绘制切向圆弧的交互动作类
class ActionDrawArcTangential : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief 交互状态枚举
    enum Status
    {
        SetBaseEntity, /**< 设置基实体 */
        SetEndAngle    /**< 设置终止角度 */
    };

public:
    ActionDrawArcTangential(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawArcTangential() override;

    void reset();

    void init(int status = 0) override;

    void trigger() override;
    void preparePreview();

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;
    void commandEvent(GuiCommandEvent* e) override;
    QStringList getAvailableCommands() override;

    void hideOptions() override;
    void showOptions() override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;
    void updatePreview();

    double getRadius() const;
    double getAngle() const;

    bool isLockRadius() const;
    void setIsLockRadius(bool lock);
    double lockRadius() const;
    void setLockRadius(double radius);

    bool isLockAngle() const;
    void setIsLockAngle(bool lock);
    double lockAngle() const;
    void setLockAngle(double angle);

protected:
    DmAtomicEntity* baseEntity;      ///< 基实体指针
    bool isStartPoint;               ///< 是否点击了基实体的起点
    std::unique_ptr<DmVector> point; ///< 决定终止角度的点
    std::unique_ptr<DmArc> tempArc;  ///< 计算得到的圆弧数据

private:
    double m_dLockRadius; ///< 锁定半径值
    bool m_bLockRadius;   ///< 是否锁定半径
    double m_dLockAngle;  ///< 锁定角度值
    bool m_bLockAngle;    ///< 是否锁定角度
};

#endif
