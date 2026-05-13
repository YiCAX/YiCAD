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

/// @file ActionFileNew.h
/// @brief 新建文件 Action 类，处理用户创建新空白文档的事件

#ifndef ACTIONFILENEW_H
#define ACTIONFILENEW_H

#include "ActionInterface.h"

/// @brief 新建文件 Action，独占模式，触发后创建新文档
class ActionFileNew : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionFileNew(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 判断是否为独占模式
    /// @return 始终返回 true
    bool isExclusive() override;

    /// @brief 初始化并立即触发新建
    /// @param status 初始状态
    void init(int status = 0) override;

    /// @brief 触发新建文件操作
    void trigger() override;

    /// @brief 获取文件名
    /// @return 文件全路径
    QString getFileName() const;

    /// @brief 设置文件名
    /// @param fileName 文件全路径，为空时自动设置
    void setFileName(const QString& fileName);

protected:
    QString m_strFileName; ///< 文件全路径，或者为空（新建时自动设置）
};

#endif // ACTIONFILENEW_H
