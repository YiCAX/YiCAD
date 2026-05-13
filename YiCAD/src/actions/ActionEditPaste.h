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

/// @file ActionEditPaste.h
/// @brief 粘贴 Action 类，处理将剪贴板实体粘贴到当前文档的用户事件

#ifndef ACTIONEDITPASTE_H
#define ACTIONEDITPASTE_H

#include "PreviewActionInterface.h"

/// @brief 处理粘贴操作的用户交互 Action
class ActionEditPaste : public PreviewActionInterface
{
    Q_OBJECT

public:
    /// @brief Action 状态枚举
    enum Status
    {
        SetTargetPoint ///< 设置目标参考点
    };

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionEditPaste(DmDocument* doc, GuiDocumentView* docView);
    ~ActionEditPaste() override;

    /// @brief 初始化 Action 状态
    /// @param status 初始状态
    void init(int status = 0) override;

    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

private:
    /// @brief 将剪贴板中的图层复制到当前文档
    void pasteLayers(DmDocument* source);

protected:
    std::unique_ptr<DmVector> targetPoint; ///< 粘贴目标参考点
};

#endif // ACTIONEDITPASTE_H
