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

/// @file ActionBlocksSave.h
/// @brief 块保存动作类头文件

#ifndef ACTIONBLOCKSSAVE_H
#define ACTIONBLOCKSSAVE_H

#include "ActionInterface.h"

class DmBlockReference;

/// @brief 保存当前激活的块到文件
class ActionBlocksSave : public ActionInterface
{
    Q_OBJECT
public:
    ActionBlocksSave(DmDocument* doc, GuiDocumentView* docView);

    void init(int status = 0) override;
    void trigger() override;

private:
    /// @brief 递归将块参照引用的块定义添加到目标文档
    void addBlock(DmBlockReference* ref, DmDocument* doc);
    /// @brief 将源文档的线型/图层/文字样式/标注样式复制到目标文档
    void copyStyles(DmDocument* src, DmDocument* dst);
};

#endif
