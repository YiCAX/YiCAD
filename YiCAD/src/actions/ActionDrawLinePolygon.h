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

/// @file ActionDrawLinePolygon.h
/// @brief 中心-角点方式绘制正多边形 Action 类

#ifndef ACTIONDRAWLINEPOLYGON_H
#define ACTIONDRAWLINEPOLYGON_H

#include "PreviewActionInterface.h"

class DmLine;

/// @brief 中心-角点方式绘制正多边形交互动作
///
/// 通过指定中心点和角点来绘制正多边形，支持命令行设置边数。
class ActionDrawLinePolygonCenCor : public PreviewActionInterface
{
    Q_OBJECT

    /// 交互状态枚举
    enum Status
    {
        SetCenter, ///< 设置中心点
        SetCorner, ///< 设置角点
        SetNumber  ///< 命令行输入边数
    };

public:
    /// @brief 构造函数
    /// @param doc 文档对象指针
    /// @param docView 文档视图指针
    ActionDrawLinePolygonCenCor(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 析构函数
    ~ActionDrawLinePolygonCenCor() override;

    /// @brief 触发多边形创建
    void trigger() override;

    /// @brief 鼠标移动事件处理
    /// @param e 鼠标事件
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 鼠标释放事件处理
    /// @param e 鼠标事件
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标按钮提示
    void updateMouseButtonHints() override;

    /// @brief 坐标事件处理
    /// @param e 坐标事件
    void coordinateEvent(GuiCoordinateEvent* e) override;

    /// @brief 命令事件处理
    /// @param e 命令事件
    void commandEvent(GuiCommandEvent* e) override;

    /// @brief 获取可用命令列表
    /// @return 可用命令字符串列表
    QStringList getAvailableCommands() override;

    /// @brief 隐藏选项面板
    void hideOptions() override;

    /// @brief 显示选项面板
    void showOptions() override;

    /// @brief 更新鼠标光标
    void updateMouseCursor() override;

    /// @brief 获取多边形边数
    /// @return 当前设置的边数
    int getNumber() const
    {
        return number;
    }

    /// @brief 设置多边形边数
    /// @param n 边数
    void setNumber(int n)
    {
        number = n;
    }

    /// @brief 根据中心点和角点创建正多边形的线段实体
    /// @param container 实体容器，可为 nullptr
    /// @param center 多边形中心点
    /// @param corner 多边形角点
    /// @param number 边数
    /// @return 创建的线段实体列表
    std::vector<DmLine*> createPolygon(DmEntityContainer* container, const DmVector& center, const DmVector& corner, int number);

private:
    struct Points;

    std::unique_ptr<Points> pPoints; ///< 多边形点数据

    static constexpr int MIN_POLYGON_EDGES = 3;       ///< 多边形最少边数
    static constexpr int MAX_POLYGON_EDGES = 10000;   ///< 命令输入边数上限（不含）

    int number = MIN_POLYGON_EDGES;        ///< 多边形边数

    Status lastStatus = Status::SetCenter; ///< 进入文本输入前的状态
};

#endif
