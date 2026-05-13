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

/// @file ActionZoomIn.h
/// @brief 缩放Action，支持不同方向和轴向的缩放操作

#ifndef ACTIONZOOMIN_H
#define ACTIONZOOMIN_H

#include "ActionInterface.h"

/// @brief 默认缩放因子
constexpr double DEFAULT_ZOOM_FACTOR = 1.25;

/// @brief 缩放操作Action，支持放大/缩小及轴向控制
class ActionZoomIn : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    /// @param [in] direction 缩放方向（放大/缩小）
    /// @param [in] axis 受影响的轴向
    /// @param [in] pCenter 缩放中心点，为空则使用当前视图中心
    /// @param [in] factor 缩放因子
    ActionZoomIn(DmDocument* doc, GuiDocumentView* docView,
                 DM::ZoomDirection direction = DM::In,
                 DM::Axis axis = DM::Both,
                 DmVector const* pCenter = nullptr,
                 double factor = DEFAULT_ZOOM_FACTOR);

    /// @brief 析构函数
    ~ActionZoomIn() override;

    /// @brief 初始化操作状态
    /// @param [in] status 初始状态值，默认为0
    void init(int status = 0) override;

    /// @brief 执行缩放操作
    void trigger() override;

    /// @brief 是否为视图操作命令
    /// @return true，缩放属于视图操作
    bool isViewAction() override { return true; }

protected:
    double zoom_factor = DEFAULT_ZOOM_FACTOR;         ///< 缩放因子
    DM::ZoomDirection direction = DM::In;             ///< 缩放方向
    DM::Axis axis = DM::Both;                         ///< 受影响的轴向
    std::unique_ptr<DmVector> center;                 ///< 缩放中心点
};

#endif // ACTIONZOOMIN_H
