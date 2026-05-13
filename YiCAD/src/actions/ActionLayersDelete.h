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

/// @file ActionLayersDelete.h
/// @brief 图层删除 Action 类，处理用户删除图层的交互

#ifndef ACTIONLAYERSDELETE_H
#define ACTIONLAYERSDELETE_H

#include "ActionInterface.h"

class DmLayer;

/// @brief 图层删除 Action，检查图层是否可删除并执行删除操作
class ActionLayersDelete : public ActionInterface
{
    Q_OBJECT
public:
    /// @brief 构造函数
    /// @param sender 发送者对象指针
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionLayersDelete(QObject* sender, DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化并立即触发
    /// @param status 初始状态
    void init(int status = 0) override;

    /// @brief 触发图层删除操作
    void trigger() override;

    /// @brief 根据图层名称设置图层
    /// @param layerName 图层名称
    void setLayer(const QString& layerName);

private:
    /// @brief 检查图层是否可删除
    /// @param layer 图层指针
    /// @return 可删除返回 true，否则返回 false
    bool canLayerRemove(DmLayer* layer);

protected:
    DmLayer* layer;     ///< 目标图层指针
    QObject* theButton; ///< 触发按钮指针
};

#endif
