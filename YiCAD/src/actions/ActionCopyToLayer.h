/*
 * Copyright (C) 2024 YiCAD Team
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

/// @file ActionCopyToLayer.h
/// @brief 复制实体到图层动作类头文件

#ifndef ACTIONCOPYLAYER_H
#define ACTIONCOPYLAYER_H

#include "PreviewActionInterface.h"
#include "DmLayer.h"

/// @brief 复制到图层操作的辅助数据
struct Points
{
    DmVector v1;            /**< 基点 */
    DmVector v2;            /**< 终点 */
    DmVector previewPos;    /**< 预览的位置 */
    DmLayer* entityLayer;   /**< 目标图层 */
};

/// @brief 处理将选中实体复制到指定图层的用户事件
class ActionCopyToLayer : public PreviewActionInterface
{
    Q_OBJECT
public:
    /// @brief 动作状态枚举
    enum Status
    {
        SetLayer,       /**< 选择目标图层 */
        SetBasePoint,   /**< 设置基点 */
        SetEndPoint     /**< 设置终点 */
    };

public:
    /// @brief 构造函数
    /// @param[in] doc 文档指针
    /// @param[in] docView 文档视图指针
    ActionCopyToLayer(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 析构函数
    ~ActionCopyToLayer() override;

    /// @brief 初始化动作
    /// @param[in] status 初始状态
    void init(int status = 0) override;

    /// @brief 触发动作执行
    void trigger() override;

    /// @brief 鼠标移动事件处理
    /// @param[in] e 鼠标事件指针
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 鼠标释放事件处理
    /// @param[in] e 鼠标事件指针
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标按钮提示
    void updateMouseButtonHints() override;

    /// @brief 坐标事件处理
    /// @param[in] e 坐标事件指针
    void coordinateEvent(GuiCoordinateEvent* e) override;

private:
    /// @brief 准备预览图形
    void preparePreview();

private:
    std::unique_ptr<Points> pPoints; /**< 操作辅助数据 */
};

#endif // ACTIONCOPYLAYER_H
