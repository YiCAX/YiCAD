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

/// @file UIDimLinearOptions.cpp
/// @brief 线性标注选项控件实现

#include "UIDimLinearOptions.h"

#include "DmSettings.h"
#include "Math2d.h"
#include "Debug.h"

#include "ui_UIDimLinearOptions.h"
#include "ActionDimLinear.h"

/// @brief 构造 UIDimLinearOptions
/// @param [in] parent 父窗口指针
/// @param [in] fl 窗口标志
UIDimLinearOptions::UIDimLinearOptions(QWidget* parent, Qt::WindowFlags fl)
    : QWidget(parent, fl)
    , ui(new Ui::Ui_DimLinearOptions{})
{
    ui->setupUi(this);
}

/// @brief 析构函数，保存设置
UIDimLinearOptions::~UIDimLinearOptions()
{
    saveSettings();
}

/// @brief 语言切换时刷新界面文本
void UIDimLinearOptions::languageChange()
{
    ui->retranslateUi(this);
}

void UIDimLinearOptions::saveSettings()
{
    DMSETTINGS->beginGroup("/Dimension");
    DMSETTINGS->writeEntry("/Angle", ui->leAngle->text());
    DMSETTINGS->endGroup();
}

void UIDimLinearOptions::setAction(ActionInterface* a, bool update)
{
    if (a && a->getEntityType() == DM::ActionDimLinear)
    {
        action = static_cast<ActionDimLinear*>(a);

        QString sa;
        if (!update)
        {
            sa = QString("%1").arg(Math2d::rad2deg(action->getAngle()));
        }
        else
        {
            DMSETTINGS->beginGroup("/Dimension");
            sa = DMSETTINGS->readEntry("/Angle", "0.0");
            DMSETTINGS->endGroup();
        }
        ui->leAngle->setText(sa);
    }
    else
    {
        action = nullptr;
    }
}

void UIDimLinearOptions::updateAngle(const QString& a)
{
    if (action)
    {
        action->setAngle(Math2d::deg2rad(Math2d::eval(a)));
    }
}
