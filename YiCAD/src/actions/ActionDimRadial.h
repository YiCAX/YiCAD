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

/// @file ActionDimRadial.h
/// @brief 径向/直径标注交互操作类头文件

#ifndef ACTIONDIMRADIAL_H
#define ACTIONDIMRADIAL_H

#include "ActionDimension.h"

struct DmDimRadialData;

/// @brief 处理用户事件以绘制径向/直径标注。
class ActionDimRadial : public ActionDimension
{
    Q_OBJECT
private:
    enum Status
    {
        SetEntity, /**< Choose entity. */
        SetPos,    /**< Choose point. */
        SetText    /**< Setting text label in the command line. */
    };

public:
    /// @brief 构造函数
    /// @param [in] doc 文档对象指针
    /// @param [in] docView 文档视图指针
    explicit ActionDimRadial(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDimRadial() override;

    /// @brief 重置操作状态
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

private:
    DmEntity* entity = nullptr;                   ///< 已选中的实体（圆弧/圆）。
    std::unique_ptr<DmVector> pos;                 ///< 拾取圆后鼠标移动时的位置。
    std::unique_ptr<DmDimRadialData> edata;        ///< 新标注数据。
    Status lastStatus;                             ///< 进入文字输入前的状态。
};

#endif
