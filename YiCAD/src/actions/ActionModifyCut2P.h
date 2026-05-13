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

/// @file ActionModifyCut2P.h
/// @brief 两点裁剪 Action 类，处理用户交互以通过两点裁剪实体（直线、圆弧、圆、椭圆或多段线）

#ifndef ACTIONMODIFYCUT2P_H
#define ACTIONMODIFYCUT2P_H

#include "PreviewActionInterface.h"

/// @brief 两点裁剪 Action，通过两个裁剪点对实体进行裁剪操作
class ActionModifyCut2P : public PreviewActionInterface
{
    Q_OBJECT
public:
    /// @brief Action 状态枚举
    enum Status
    {
        ChooseCutEntity,  ///< 选择待裁剪实体
        SetCutCoord       ///< 设置裁剪坐标
    };

    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionModifyCut2P(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 析构函数
    ~ActionModifyCut2P() override;

    /// @brief 初始化 Action 状态
    /// @param status 初始状态
    void init(int status = 0) override;

    /// @brief 执行裁剪操作
    void trigger() override;

    /// @brief 鼠标移动事件处理
    /// @param e 鼠标事件指针
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 鼠标释放事件处理
    /// @param e 鼠标事件指针
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

private:
    DmEntity* cutEntity;                     ///< 待裁剪的实体指针
    std::unique_ptr<DmVector> firstCoord;    ///< 第一个裁剪点坐标
    std::unique_ptr<DmVector> secondCoord;   ///< 第二个裁剪点坐标
};

#endif // ACTIONMODIFYCUT2P_H
