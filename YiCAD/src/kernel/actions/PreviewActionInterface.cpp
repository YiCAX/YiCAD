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

/// @file PreviewActionInterface.cpp
/// @brief 预览操作接口的实现

#include "PreviewActionInterface.h"

#include "GuiDocumentView.h"
#include "DmDocument.h"
#include "Preview.h"
#include "Debug.h"

/// @brief 设置继承此接口的操作类所操作的实体容器
/// @param name 操作名称
/// @param doc 文档指针
/// @param docView 文档视图指针
PreviewActionInterface::PreviewActionInterface(const char* name, DmDocument* doc, GuiDocumentView* docView)
    : ActionInterface(name, doc, docView)
    , preview(new Preview(doc))
    , pDocument(doc)
{
    hasPreview = true;
}

PreviewActionInterface::~PreviewActionInterface()
{
    deletePreview();
}

void PreviewActionInterface::init(int status)
{
    deletePreview();
    ActionInterface::init(status);
}

void PreviewActionInterface::finish(bool updateTB)
{
    deletePreview();
    ActionInterface::finish(updateTB);
}

void PreviewActionInterface::suspend()
{
    ActionInterface::suspend();
    deletePreview();
}

void PreviewActionInterface::resume()
{
    ActionInterface::resume();
    drawPreview();
}

void PreviewActionInterface::trigger()
{
    ActionInterface::trigger();
    deletePreview();
}

bool PreviewActionInterface::isViewAction()
{
    return false;
}

void PreviewActionInterface::deletePreview()
{
    if (hasPreview)
    {
        // 避免删除空预览
        preview->clear();
        hasPreview = false;
    }

    if (!docView->isCleanUp())
    {
        //docView->getOverlayContainer(DM::ActionPreviewEntity)->clear();
        docView->disableOverlayBox();
    }
}

void PreviewActionInterface::drawPreview()
{
    docView->redraw();
    hasPreview = true;
}
