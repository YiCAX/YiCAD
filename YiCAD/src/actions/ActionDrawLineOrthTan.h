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

/// @file ActionDrawLineOrthTan.h
/// @brief 绘制与直线正交的切线交互动作类

#ifndef ACTIONDRAWLINEORTHTAN_H
#define ACTIONDRAWLINEORTHTAN_H

#include "PreviewActionInterface.h"

class DmLine;

/// @brief 处理用户事件以绘制与直线正交的切线
class ActionDrawLineOrthTan : public PreviewActionInterface
{
    Q_OBJECT
private:
    enum Status
    {
        SetLine,  ///< 选择与切线正交的直线
        SetCircle ///< 选择圆弧/圆/椭圆以创建其切线
    };

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionDrawLineOrthTan(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 触发当前动作
    void trigger() override;

    /// @brief 完成动作，清理状态
    /// @param updateTB 是否更新工具栏
    void finish(bool updateTB = true) override;

    /// @brief 处理鼠标移动事件
    /// @param e 鼠标事件指针
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 处理鼠标释放事件
    /// @param e 鼠标事件指针
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标按钮提示文本
    void updateMouseButtonHints() override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

    /// @brief 创建一条与给定法线正交的切线
    /// @param coord 参考坐标点
    /// @param normal 法线实体指针
    /// @param circle 圆弧/圆/椭圆实体指针
    /// @return 创建的切线，失败返回 nullptr
    DmLine* createLineOrthTan(const DmVector& coord, DmLine* normal, DmEntity* circle);

private:
    /// @brief 清除选中的直线和预览
    void clearLines();

    DmLine* normal = nullptr;  ///< 选中的法线
    DmLine* tangent = nullptr; ///< 用于预览的切线
    DmEntity* circle = nullptr; ///< 用于生成切线的圆弧/圆/椭圆
};

#endif
