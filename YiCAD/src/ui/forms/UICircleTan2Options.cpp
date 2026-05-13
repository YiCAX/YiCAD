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

/// @file UICircleTan2Options.cpp
/// @brief 双切圆选项控件实现

#include "UICircleTan2Options.h"

#include "ActionDrawCircleTan2.h"
#include "DmSettings.h"
#include "Math2d.h"
#include "Debug.h"

#include "ui_UICircleTan2Options.h"

/// @brief 构造 UICircleTan2Options
/// @param [in] parent 父窗口指针
/// @param [in] fl 窗口标志
UICircleTan2Options::UICircleTan2Options(QWidget* parent, Qt::WindowFlags fl)
    : QWidget(parent, fl)
    , ui(new Ui::Ui_CircleTan2Options{})
{
    ui->setupUi(this);
}

/// @brief 析构函数，保存设置
UICircleTan2Options::~UICircleTan2Options()
{
    saveSettings();
    // no need to delete child widgets, Qt does it all for us
}

/// @brief 语言切换时刷新界面文本
void UICircleTan2Options::languageChange()
{
    ui->retranslateUi(this);
}

void UICircleTan2Options::saveSettings()
{
    DMSETTINGS->beginGroup("/Draw");
    DMSETTINGS->writeEntry("/CircleTan2Radius", ui->leRadius->text());
    DMSETTINGS->endGroup();
}

void UICircleTan2Options::setAction(ActionInterface* a, bool update)
{
    if (a && a->getEntityType() == DM::ActionDrawCircleTan2)
    {
        action = static_cast<ActionDrawCircleTan2*>(a);

        QString sr;
        if (update)
        {
            sr = QString("%1").arg(action->getRadius());
        }
        else
        {
            DMSETTINGS->beginGroup("/Draw");
            sr = DMSETTINGS->readEntry("/CircleTan2Radius", "1.0");
            DMSETTINGS->endGroup();
        }
        ui->leRadius->setText(sr);
    }
    else
    {
        action = nullptr;
    }
}

void UICircleTan2Options::updateRadius(const QString& r)
{
    if (action)
    {
        bool ok;
        double radius = Math2d::eval(r, &ok);
        if (ok)
        {
            action->setRadius(radius);
        }/*else{
            ui->leRadius->setText("10.0");
        }*/
    }
}
