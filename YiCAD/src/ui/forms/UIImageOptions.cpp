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

/// @file UIImageOptions.cpp
/// @brief 图片插入选项控件

#include "UIImageOptions.h"

#include "ActionInterface.h"
#include "ActionDrawImage.h"
#include "DmSettings.h"
#include "Math2d.h"
#include "Debug.h"
#include "ui_UIImageOptions.h"

UIImageOptions::UIImageOptions(QWidget* parent, Qt::WindowFlags fl)
    : QWidget(parent, fl)
    , ui(new Ui::Ui_ImageOptions)
{
    ui->setupUi(this);
}

/// @brief 销毁对象并释放资源
UIImageOptions::~UIImageOptions()
{
    saveSettings();
}

/// @brief 使用当前语言设置子控件的字符串
void UIImageOptions::languageChange()
{
    ui->retranslateUi(this);
}

void UIImageOptions::saveSettings()
{
    DMSETTINGS->beginGroup("/Image");
    DMSETTINGS->writeEntry("/ImageAngle", ui->leAngle->text());
    DMSETTINGS->writeEntry("/ImageFactor", ui->leFactor->text());
    DMSETTINGS->endGroup();
}

void UIImageOptions::setAction(ActionInterface* a, bool update)
{
    if (a != nullptr && a->getEntityType() == DM::ActionDrawImage)
    {
        action = static_cast<ActionDrawImage*>(a);

        QString sAngle;
        QString sFactor;
        if (update)
        {
            sAngle = QString("%1").arg(Math2d::rad2deg(action->getAngle()));
            sFactor = QString("%1").arg(action->getFactor());
        }
        else
        {
            DMSETTINGS->beginGroup("/Image");
            sAngle = DMSETTINGS->readEntry("/ImageAngle", "0.0");
            sFactor = DMSETTINGS->readEntry("/ImageFactor", "1.0");
            DMSETTINGS->endGroup();
        }
        ui->leAngle->setText(sAngle);
        ui->leFactor->setText(sFactor);
        updateData();
        updateFactor();
    }
    else
    {
        action = nullptr;
    }
}

void UIImageOptions::updateData()
{
    if (action != nullptr)
    {
        action->setAngle(Math2d::deg2rad(Math2d::eval(ui->leAngle->text())));
    }
}

void UIImageOptions::updateDPI()
{
    if (action != nullptr)
    {
        double f = action->dpiToScale(Math2d::eval(ui->leDPI->text()));
        ui->leFactor->blockSignals(true);
        ui->leFactor->setText(QString::number(f));
        ui->leFactor->blockSignals(false);
        action->setFactor(f);
    }
}

void UIImageOptions::updateFactor()
{
    if (action != nullptr)
    {
        double f = Math2d::eval(ui->leFactor->text());
        double dpi = action->scaleToDpi(f);
        ui->leDPI->blockSignals(true);
        ui->leDPI->setText(QString::number(dpi));
        ui->leDPI->blockSignals(false);
        action->setFactor(f);
    }
}
