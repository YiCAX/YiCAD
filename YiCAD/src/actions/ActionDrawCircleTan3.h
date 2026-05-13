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

/// @file ActionDrawCircleTan3.h
/// @brief 绘制三个给定圆的公切圆（阿波罗尼奥斯问题）的交互动作类

#ifndef ACTIONDRAWCIRCLETAN3_H
#define ACTIONDRAWCIRCLETAN3_H

#include <vector>

#include "PreviewActionInterface.h"

struct DmCircleData;
class DmAtomicEntity;

/// @brief 绘制三个给定圆/线的公切圆（阿波罗尼奥斯问题的解）
class ActionDrawCircleTan3 : public PreviewActionInterface
{
    Q_OBJECT
public:
    /// @brief 动作状态枚举
    enum Status
    {
        SetCircle1, ///< 选择第一个圆/线
        SetCircle2, ///< 选择第二个圆/线
        SetCircle3, ///< 选择第三个圆/线
        SetCenter   ///< 选择最近的公切圆圆心
    };

public:
    ActionDrawCircleTan3(DmDocument* doc, GuiDocumentView* docView);
    ~ActionDrawCircleTan3() override;

    void init(int status = 0) override;

    void trigger() override;
    bool getData();
    bool preparePreview();

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    QStringList getAvailableCommands() override;
    void finish(bool updateTB = true) override;
    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

private:
    /// @brief 验证圆心是否有效
    std::vector<double> verifyCenter(const DmVector& center) const;
    /// @brief 获取实体到圆心的半径列表
    std::vector<double> getRadii(DmAtomicEntity* entity, const DmVector& center) const;
    DmEntity* catchCircle(QMouseEvent* e);

    struct Points;
    std::unique_ptr<Points> pPoints;
};

#endif
