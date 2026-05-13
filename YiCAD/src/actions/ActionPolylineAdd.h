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

/// @file ActionPolylineAdd.h
/// @brief 多段线添加节点操作，支持在多段线的指定段上插入新节点

#ifndef ACTIONPOLYLINEADD_H
#define ACTIONPOLYLINEADD_H

#include <memory>

#include "PreviewActionInterface.h"

class DmPolyline;
class DmVector;

/// @brief 多段线添加节点操作，支持在多段线的指定段上插入新节点
class ActionPolylineAdd : public PreviewActionInterface
{
    Q_OBJECT

    /// @brief 操作状态枚举
    enum Status
    {
        ChooseSegment,  ///< 选择现有多段线中要添加节点的段
        SetAddCoord,    ///< 设置添加节点的参考点
        SetPointPos     ///< 设置节点插入位置
    };

public:
    /// @brief 构造函数
    /// @param [in] doc 文档指针
    /// @param [in] docView 文档视图指针
    ActionPolylineAdd(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 析构函数
    ~ActionPolylineAdd() override;

    /// @brief 初始化操作状态
    /// @param [in] status 初始状态值，默认为0（ChooseSegment）
    void init(int status = 0) override;

    /// @brief 执行添加节点操作
    void trigger() override;

    /// @brief 处理鼠标移动事件
    /// @param [in] e 鼠标事件指针
    void mouseMoveEvent(QMouseEvent* e) override;

    /// @brief 处理鼠标释放事件
    /// @param [in] e 鼠标事件指针
    void mouseReleaseEvent(QMouseEvent* e) override;

    /// @brief 更新鼠标按钮提示文本
    void updateMouseButtonHints() override;

    /// @brief 更新鼠标光标样式
    void updateMouseCursor() override;

    /// @brief 完成操作，清理状态
    /// @param [in] updateTB 是否更新工具栏
    void finish(bool updateTB) override;

private:
    /// @brief 获取待移除子实体的凸度和线宽信息
    /// @param [out] bulgeIdx 凸度索引
    /// @param [out] bulge 凸度值
    /// @param [out] startLineWeight 起始线宽
    /// @param [out] endLineWeight 结束线宽
    void getRemovedPartInfo(int& bulgeIdx, double& bulge, double& startLineWeight, double& endLineWeight);

private:
    DmPolyline* addPoly = nullptr;              ///< 待修改的多段线
    int addVertextIdx = -1;                     ///< 选择的顶点索引
    std::unique_ptr<DmVector> addCoord;         ///< 新插入点坐标
};

#endif
