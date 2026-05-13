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

/// @file UIArcOptions.cpp
/// @brief 圆弧绘制选项控件实现

#include "UIArcOptions.h"

#include "ActionDrawArc.h"
#include "DmSettings.h"
#include "Debug.h"
#include "ui_UIArcOptions.h"

/// @brief 构造 UIArcOptions
/// @param [in] parent 父窗口指针
/// @param [in] fl 窗口标志
UIArcOptions::UIArcOptions(QWidget* parent, Qt::WindowFlags fl)
    : QWidget(parent, fl)
    , ui(new Ui::Ui_ArcOptions{})
{
    ui->setupUi(this);
    connect(ui->rbPos, SIGNAL(toggled(bool)), this, SLOT(slotRdoToggled(bool)));
}

/// @brief 析构函数，保存设置
UIArcOptions::~UIArcOptions()
{
    saveSettings();
}

void UIArcOptions::slotRdoToggled(bool checked)
{
    if (action)
    {
        action->setClockwise(ui->rbNeg->isChecked());
    }
}

/// @brief 语言切换时刷新界面文本
void UIArcOptions::languageChange()
{
    ui->retranslateUi(this);
}

void UIArcOptions::saveSettings()
{
    DMSETTINGS->beginGroup("/Draw");
    DMSETTINGS->writeEntry("/ArcClockwise", static_cast<int>(ui->rbNeg->isChecked()));
    DMSETTINGS->endGroup();
}

void UIArcOptions::setAction(ActionInterface* a, bool update)
{
    if (a && a->getEntityType() == DM::ActionDrawArc)
    {
        action = static_cast<ActionDrawArc*>(a);

        bool clockwise;
        if (update)
        {
            clockwise = action->isClockwise();
        }
        else
        {
            DMSETTINGS->beginGroup("/Draw");
            clockwise = DMSETTINGS->readNumEntry("/ArcClockwise", 0);
            DMSETTINGS->endGroup();
            action->setClockwise(clockwise);
        }
        ui->rbNeg->setChecked(clockwise);
    }
    else
    {
        action = nullptr;
    }
}
