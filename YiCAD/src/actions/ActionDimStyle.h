/*
 * Copyright (C) 2026 YiCAD Team
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

/// @file ActionDimStyle.h
/// @brief 标注样式管理操作类头文件

#ifndef ACTIONDIMSTYLE_H
#define ACTIONDIMSTYLE_H

#include "ActionInterface.h"

/// @brief 打开标注样式管理器对话框。
class ActionDimStyle : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] doc 文档对象指针
    /// @param [in] docView 文档视图指针
    explicit ActionDimStyle(DmDocument* doc, GuiDocumentView* docView);

    void init(int status = 0) override;
    void trigger() override;
};

#endif  // ACTIONDIMSTYLE_H

