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

/// @file ActionLayersActivate.h
/// @brief 图层激活 Action 类，处理设置当前激活图层的用户交互

#ifndef ACTIONLAYERSACTIVATE_H
#define ACTIONLAYERSACTIVATE_H

#include "ActionInterface.h"

class DmLayer;
struct ComboBoxData;

/// @brief 图层激活 Action，将指定图层设为当前活动图层
class ActionLayersActivate : public ActionInterface
{
    Q_OBJECT
public:
    /// @brief 构造函数
    /// @param sender 发送者对象指针
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionLayersActivate(QObject* sender, DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化并立即触发
    /// @param status 初始状态
    void init(int status = 0) override;

    /// @brief 触发图层激活操作
    void trigger() override;

    /// @brief 获取当前图层
    /// @return 图层指针
    DmLayer* getLayer() const;

    /// @brief 设置图层
    /// @param layer 图层指针
    void setLayer(DmLayer* layer);

    /// @brief 根据图层名称设置图层
    /// @param layerName 图层名称
    void setLayer(const QString& layerName);

    /// @brief 设置下拉框数据
    /// @param data 下拉框数据指针
    void setComboBoxData(ComboBoxData* data);

protected:
    DmLayer* layer;            ///< 目标图层指针
    ComboBoxData* cbxData;     ///< 下拉框数据指针
    QObject* theButton;        ///< 触发按钮指针
};

#endif
