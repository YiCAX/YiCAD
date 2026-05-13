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

/// @file ActionLayersColor.cpp
/// @brief 图层颜色 Action 类的实现

#include "ActionLayersColor.h"
#include "DmLayer.h"
#include "DmDocument.h"
#include "ApplicationWindow.h"
#include "CustomComboboxItem.h"
#include "GuiDocumentView.h"
#include "SARibbonComboBox.h"
#include "MDIWindow.h"
#include "Transaction.h"

/// @brief 构造函数
/// @param sender 发送者对象指针
/// @param doc 文档指针
/// @param docView 文档视图指针
ActionLayersColor::ActionLayersColor(QObject* sender, DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Layer Color", doc, docView)
    , layer(nullptr)
    , theButton(sender)
{
}

/// @brief 初始化并立即触发
/// @param status 初始状态
void ActionLayersColor::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 触发图层颜色修改操作
void ActionLayersColor::trigger()
{
    ApplicationWindow* appWin = ApplicationWindow::getAppWindow();
    auto cbxItems = appWin->getLayerComboboxItems();

    // 按钮触发的action，获得按钮所在的图层，判断需要隐藏还是显示
    if (theButton && nullptr == layer)
    {
        // 当前图层按钮触发
        ComboBoxData* currentData = appWin->getCurrentLayerItem();
        if (currentData->btnColor == theButton)
        {
            setLayer(currentData->getLayerName());
        }
        // 图层下拉列表按钮触发
        else
        {
            for (auto& item : cbxItems)
            {
                auto data = item->getData();
                if (data->btnColor == theButton)
                {
                    setLayer(data->getLayerName());
                }
            }
        }
    }

    if (layer)
    {
        QColorDialog dlg;
        const DmColor dmcolor = layer->getPen().getColor();
        const QColor initColor(dmcolor.red(), dmcolor.green(), dmcolor.blue(), dmcolor.alpha());
        const QColor color = dlg.getColor(initColor, nullptr, tr("Select Color"), QColorDialog::DontUseNativeDialog);

        // 确定
        if (color.isValid())
        {
            DmColor dcolor(color.red(), color.green(), color.blue());
            DmPen dpen = layer->getPen();
            dpen.setColor(dcolor);
            Transaction t(tr("Layer Color").toStdString(), pDocument);
            t.start();
            pDocument->getLayerTable()->startModify(layer);
            layer->setPen(dpen);
            t.commit();
        }
    }
    finish();
}

/// @brief 根据图层名称设置图层
/// @param layerName 图层名称
void ActionLayersColor::setLayer(const QString& layerName)
{
    layer = pDocument->getLayerTable()->find(layerName);
}
