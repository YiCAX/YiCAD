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

/// @file ActionEditCopy.h
/// @brief 编辑复制/剪切 Action 类，处理复制或剪切实体到剪贴板

#ifndef ACTIONEDITCOPY_H
#define ACTIONEDITCOPY_H

#include "ActionInterface.h"

/// @brief 处理复制/剪切到剪贴板的用户交互 Action
class ActionEditCopy : public ActionInterface
{
    Q_OBJECT

    /// @brief Action 状态枚举
    enum Status
    {
        SetReferencePoint ///< 设置参考点
    };

public:
    /// @brief 构造函数
    /// @param copy 为 true 时复制，为 false 时剪切
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionEditCopy(bool copy, DmDocument* doc, GuiDocumentView* docView);
    ~ActionEditCopy() override;

    /// @brief 初始化 Action 状态
    /// @param status 初始状态
    void init(int status) override;

    void trigger() override;

    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

    void coordinateEvent(GuiCoordinateEvent* e) override;

    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

protected:
    bool copy;                              ///< true为复制，false为剪切
    std::unique_ptr<DmVector> referencePoint; ///< 参考点
};

#endif // ACTIONEDITCOPY_H
