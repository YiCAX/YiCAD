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

/// @file ActionBlocksSaveAs.h
/// @brief 块另存为动作类头文件

#ifndef ACTIONBLOCKSSAVEAS_H
#define ACTIONBLOCKSSAVEAS_H

#include "ActionInterface.h"

class UIBlockSaveAs;

/// @brief 处理将当前激活块另存为文件的用户事件
class ActionBlocksSaveAs : public ActionInterface
{
    Q_OBJECT
public:
    /// @brief 构造函数
    /// @param[in] doc 文档指针
    /// @param[in] docView 文档视图指针
    ActionBlocksSaveAs(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 析构函数
    ~ActionBlocksSaveAs() override;

    /// @brief 判断是否为独占动作
    /// @return true 表示独占
    bool isExclusive() override;

    /// @brief 初始化动作
    /// @param[in] status 初始状态
    void init(int status = 0) override;

    /// @brief 触发动作执行
    void trigger() override;

private:
    UIBlockSaveAs* m_pBlockSaveAs; /**< 块另存为对话框指针 */
};

#endif
