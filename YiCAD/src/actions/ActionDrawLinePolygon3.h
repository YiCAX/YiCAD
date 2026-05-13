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

/// @file ActionDrawLinePolygon3.h
/// @brief 中心-切线法绘制正多边形 Action 类头文件

#ifndef ACTIONDRAWLINEPOLYGON3_H
#define ACTIONDRAWLINEPOLYGON3_H

#include "PreviewActionInterface.h"

class DmLine;

/// @brief 中心-切线法绘制正多边形
///
/// 通过指定中心点、切点和边数来创建正多边形（线串）。
/// 支持命令行输入边数和坐标。
class ActionDrawLinePolygonCenTan : public PreviewActionInterface
{
    Q_OBJECT

    /// 交互状态枚举
    enum Status
    {
        SetCenter,  ///< 设置中心点
        SetTangent, ///< 设置切点
        SetNumber   ///< 命令行输入边数
    };

public:
    /// @brief 构造函数
    /// @param [in] doc 文档对象指针
    /// @param [in] docView 文档视图指针
    ActionDrawLinePolygonCenTan(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 析构函数
    ~ActionDrawLinePolygonCenTan() override;

    /// @brief 触发动作执行
    void trigger() override;

    /// @brief 鼠标移动事件处理
    /// @param [in] e 鼠标事件
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 鼠标释放事件处理
    /// @param [in] e 鼠标事件
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标按钮提示
    void updateMouseButtonHints() override;

    /// @brief 坐标事件处理
    /// @param [in] e 坐标事件
    void coordinateEvent(GuiCoordinateEvent* e) override;

    /// @brief 命令事件处理
    /// @param [in] e 命令事件
    void commandEvent(GuiCommandEvent* e) override;

    /// @brief 获取可用命令列表
    /// @return 可用命令字符串列表
    QStringList getAvailableCommands() override;

    /// @brief 隐藏选项面板
    void hideOptions() override;

    /// @brief 显示选项面板
    void showOptions() override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

    /// @brief 获取多边形边数
    /// @return 当前设置的边数
    int getNumber() const
    {
        return number;
    }

    /// @brief 设置多边形边数
    /// @param [in] sideCount 边数
    void setNumber(int sideCount)
    {
        number = sideCount;
    }

    /// @brief 创建正多边形线串
    /// @param [in] container 实体容器指针
    /// @param [in] center 中心点坐标
    /// @param [in] tangent 切点坐标
    /// @param [in] number 边数
    /// @return 创建的线段列表
    std::vector<DmLine*> createPolygon3(DmEntityContainer* container,
                                        const DmVector& center,
                                        const DmVector& tangent,
                                        int number);

private:
    struct Points;

    std::unique_ptr<Points> pPoints; ///< 多边形点数据（Pimpl 惯用法）
    int number;                      ///< 边数
    Status lastStatus;               ///< 进入文本输入前的状态

};

#endif  // ACTIONDRAWLINEPOLYGON3_H
