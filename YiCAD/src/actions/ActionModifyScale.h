/*
 * Copyright (C) 2026 YiCAD Community
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

/// @file ActionModifyScale.h
/// @brief 缩放实体交互命令头文件

#ifndef ACTIONMODIFYSCALE_H
#define ACTIONMODIFYSCALE_H

#include "PreviewActionInterface.h"

class ScaleData;

/// @brief 缩放实体交互命令
///
/// 处理用户交互事件，实现选中实体的比例缩放功能。
/// 支持设置参考点和输入缩放比例两种操作方式。
class ActionModifyScale : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief 命令状态枚举
    enum Status
    {
        SetReferencePoint, ///< 设置参考点
        SetScale           ///< 设置缩放比例
    };

public:
    ActionModifyScale(DmDocument* doc, GuiDocumentView* docView);
    ~ActionModifyScale() override;

    /// @brief 初始化实体包围框数据，计算选中实体的宽高较大值
    void initBox();

    /// @brief 初始化命令状态
    /// @param [in] status 初始状态，默认为0（SetReferencePoint）
    void init(int status = 0) override;

    /// @brief 执行缩放操作
    void trigger() override;

    /// @brief 处理鼠标移动事件
    /// @param [in] e 鼠标事件对象
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 处理鼠标释放事件
    /// @param [in] e 鼠标事件对象
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 处理坐标输入事件
    /// @param [in] e 坐标事件对象
    void coordinateEvent(GuiCoordinateEvent* e) override;

    /// @brief 处理命令行输入事件
    /// @param [in] e 命令事件对象
    void commandEvent(GuiCommandEvent* e) override;

    /// @brief 更新鼠标按钮提示
    void updateMouseButtonHints() override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

private:
    struct Points;
    std::unique_ptr<Points> pPoints; ///< 缩放操作中使用的点数据
    double m_boxRange;               ///< 选中实体的宽高较大值
};

#endif
