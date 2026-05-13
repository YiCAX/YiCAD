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

/// @file ActionOptionsGeneral.cpp
/// @brief 通用选项交互命令实现

#include "ActionOptionsGeneral.h"

#include "GuiDialogFactory.h"
#include "DmSettings.h"
#include "ApplicationWindow.h"
#include "MDIWindow.h"
#include "GuiDocumentView.h"

#include <QMdiSubWindow>
#include <QMdiArea>

// TODO: 建议在构造函数中设置 actionType = DM::ActionOptionsGeneral（与其他文件保持一致）

ActionOptionsGeneral::ActionOptionsGeneral(DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("General Options", doc, docView)
{
}

void ActionOptionsGeneral::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 触发通用选项对话框显示
///
/// 打开通用选项对话框，并从存储的设置中读取当前颜色配置，
/// 将其应用到所有打开的文档视图中。
void ActionOptionsGeneral::trigger()
{
    GUIDIALOGFACTORY->requestOptionsGeneralDialog();

    DMSETTINGS->beginGroup("Colors");
    QColor background(DMSETTINGS->readEntry("/background", Colors::BACKGROUND));
    QColor gridColor(DMSETTINGS->readEntry("/grid", Colors::GRID));
    QColor metaGridColor(DMSETTINGS->readEntry("/meta_grid", Colors::META_GRID));
    QColor selectedColor(DMSETTINGS->readEntry("/select", Colors::SELECT));
    QColor highlightColor(DMSETTINGS->readEntry("/highlight", Colors::HIGHLIGHT));
    DMSETTINGS->endGroup();

    QList<QMdiSubWindow*> windows = ApplicationWindow::getAppWindow()->getMdiArea()->subWindowList();

    for (int i = 0; i < windows.size(); ++i)
    {
        MDIWindow* m = qobject_cast<MDIWindow*>(windows.at(i));

        if (m)
        {
            GuiDocumentView* gv = m->getDocumentView();

            if (gv)
            {
                gv->setBackground(background);
                gv->setGridColor(gridColor);
                gv->setMetaGridColor(metaGridColor);
                gv->setSelectedColor(selectedColor);
                gv->setHighlightColor(highlightColor);
                gv->redraw();
            }
        }
    }

    finish();
}
