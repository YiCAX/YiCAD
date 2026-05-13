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

/// @file ActionModifySingleOffset.h
/// @brief 单个实体偏移交互命令头文件

#ifndef ACTIONMODIFYSINGLEOFFSET_H
#define ACTIONMODIFYSINGLEOFFSET_H

#include "PreviewActionInterface.h"

class OffsetData;
class DmPolyline;

/// @brief 单个实体偏移交互命令
///
/// 实现对选中实体的单次偏移操作。
/// 先选择原始实体，再指定偏移方向和距离。
class ActionModifySingleOffset : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief 命令状态枚举
    enum Status
    {
        ChooseEntity ///< 选择偏移实体
    };

public:
    ActionModifySingleOffset(DmDocument* doc, GuiDocumentView* docView);
    ~ActionModifySingleOffset() override;

    /// @brief 初始化命令状态
    /// @param [in] status 初始状态，默认为0（ChooseEntity）
    void init(int status = 0) override;

    /// @brief 执行偏移操作
    void trigger() override;

    /// @brief 处理鼠标移动事件，预览偏移效果
    /// @param [in] e 鼠标事件对象
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 处理鼠标释放事件
    /// @param [in] e 鼠标事件对象
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

    /// @brief 显示选项面板
    void showOptions() override;

    /// @brief 隐藏选项面板
    void hideOptions() override;

    /// @brief 设置偏移距离
    /// @param [in] d 偏移距离
    void setDist(const double& d);

    /// @brief 获取当前偏移距离
    /// @return 偏移距离
    double getDist() const;

    /// @brief 设置偏移数量
    /// @param [in] n 偏移数量
    void setNumber(unsigned n);

    /// @brief 获取当前偏移数量
    /// @return 偏移数量
    int getNumber() const;

private:
    void unhighlightEntity();

    DmEntity* m_pOriginalEntity = nullptr; ///< 原始实体
    std::unique_ptr<OffsetData> m_pData;   ///< 偏移参数数据
    DmEntity* prevHighlighted = nullptr;   ///< 上次高亮的实体
};

#endif
