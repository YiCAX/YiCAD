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

/// @file PreviewActionInterface.h
/// @brief 需要预览功能的操作类接口，管理预览的绘制和删除

#ifndef PREVIEWACTIONINTERFACE_H
#define PREVIEWACTIONINTERFACE_H

#include <memory>

#include "ActionInterface.h"

class Preview;
class DmDocument;

/// @brief 预览操作接口
/// 所有需要预览功能的操作类必须实现此接口
class PreviewActionInterface : public ActionInterface
{
public:
    PreviewActionInterface(const char* name, DmDocument* doc, GuiDocumentView* docView);
    ~PreviewActionInterface() override;

    void init(int status = 0) override;
    void finish(bool updateTB = true) override;
    void suspend() override;
    void resume() override;
    void trigger() override;
    bool isViewAction() override;

    /// @brief 绘制当前预览
    void drawPreview();

    /// @brief 从屏幕上删除预览
    void deletePreview();

protected:
    std::unique_ptr<Preview>    preview;    ///< 持有预览实体的预览对象
    bool                        hasPreview; ///< 预览是否正在使用中
    DmDocument*                 pDocument;  ///< 关联的文档指针
};

#endif
