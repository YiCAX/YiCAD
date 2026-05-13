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

/// @file ActionDimLinear.h
/// @brief 线性标注交互操作类头文件

#ifndef ACTIONDIMLINEAR_H
#define ACTIONDIMLINEAR_H

#include "ActionDimension.h"

struct DmDimLinearData;

/// @brief 处理用户事件以绘制线性标注。
class ActionDimLinear : public ActionDimension
{
    Q_OBJECT
public:
    /**
     * Varitions of this action.
     */
    enum Variation
    {
        AnyAngle,
        Horizontal,
        Vertical
    };

    /**
     * Action States.
     */
    enum Status
    {
        SetExtPoint1, /**< Setting the 1st ext point.  */
        SetExtPoint2, /**< Setting the 2nd ext point. */
        SetDefPoint,  /**< Setting the common def point. */
        SetText,      /**< Setting the text label in the command line. */
        SetAngle      /**< Setting the angle in the command line. */
    };

public:
    /// @brief 构造函数
    /// @param [in] doc 文档对象指针
    /// @param [in] docView 文档视图指针
    /// @param [in] type 操作类型，默认为线性标注
    explicit ActionDimLinear(DmDocument* doc, GuiDocumentView* docView,
                    DM::ActionType type = DM::ActionDimLinear);
    ~ActionDimLinear() override;

    void reset() override;

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

    double getAngle() const;

    void setAngle(double a);

protected:
    std::unique_ptr<DmDimLinearData> edata;  ///< 线性标注数据。
    Status lastStatus;                        ///< 进入文字或角度输入前的状态。
};

#endif
