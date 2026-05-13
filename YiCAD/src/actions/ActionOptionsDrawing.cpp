/*
 * Copyright (C) 2026 YiCAD Community
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

/// @file ActionOptionsDrawing.cpp
/// @brief 绘图选项交互命令实现

#include "ActionOptionsDrawing.h"

#include <QAction>

#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"

ActionOptionsDrawing::ActionOptionsDrawing(DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Drawing Options", doc, docView)
{
    actionType = DM::ActionOptionsDrawing;
}

void ActionOptionsDrawing::init(int status)
{
    ActionInterface::init(status);

    trigger();
}

/// @brief 触发绘图选项对话框显示
///
/// 打开绘图选项对话框，并重置坐标显示控件。
void ActionOptionsDrawing::trigger()
{
    if (pDocument)
    {
        GUIDIALOGFACTORY->requestOptionsDrawingDialog(*pDocument);
        GUIDIALOGFACTORY->updateCoordinateWidget(DmVector(0.0, 0.0), DmVector(0.0, 0.0), true);
        docView->redraw();
    }

    finish(false);
}
