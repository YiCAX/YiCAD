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

/// @file ActionLayersActivate.cpp
/// @brief 图层激活 Action 类的实现

#include "ActionLayersActivate.h"

#include <QAbstractItemView>

#include "Debug.h"
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
ActionLayersActivate::ActionLayersActivate(QObject* sender, DmDocument* doc, GuiDocumentView* docView) :
    ActionInterface("Activate Layer", doc, docView)
    , layer(nullptr)
    , cbxData(nullptr)
    , theButton(sender)
{
}

/// @brief 初始化并立即触发
/// @param status 初始状态
void ActionLayersActivate::init(int status)
{
    ActionInterface::init(status);
    trigger();
}

/// @brief 触发图层激活操作
void ActionLayersActivate::trigger()
{
    ApplicationWindow* appWin = ApplicationWindow::getAppWindow();
    auto cbxItems = appWin->getLayerComboboxItems();

    // 按钮触发的action，获得按钮所在的图层，判断需要隐藏还是显示
    if (theButton && nullptr == layer)
    {
        // 图层下拉列表按钮触发
        for (auto& item : cbxItems)
        {
            auto data = item->getData();
            if (data->labelName == theButton)
            {
                setLayer(data->getLayerName());
                setComboBoxData(data);
            }
        }
    }

    if (layer)
    {
        Transaction t(tr("Activate Layer").toStdString(), pDocument);
        t.start();
        pDocument->getLayerTable()->activate(layer);

        // 更新选择的实体的图层
        auto table = pDocument->getEntityTable();
        for (auto ite = table->begin(); ite != table->end(); ite++)
        {
            if ((*ite)->isSelected())
            {
                DmLayer* entityLayer = (*ite)->getLayer();
                if (entityLayer != layer)
                {
                    // 仅需修改图层，不需要update，因为仅需设置最顶级的实体图层，子实体不用设置，
                    // 绘制时获得的pen就是改变图层后的（如果不是bylayer则不变）
                    (*ite)->setLayer(layer);
                }
            }
        }
        t.commit();
    }
    finish();
}

/// @brief 获取当前图层
/// @return 图层指针
DmLayer* ActionLayersActivate::getLayer() const
{
    return layer;
}

/// @brief 设置图层
/// @param layer 图层指针
void ActionLayersActivate::setLayer(DmLayer* layer)
{
    this->layer = layer;
}

/// @brief 根据图层名称设置图层
/// @param layerName 图层名称
void ActionLayersActivate::setLayer(const QString& layerName)
{
    layer = pDocument->getLayerTable()->find(layerName);
}

/// @brief 设置下拉框数据
/// @param data 下拉框数据指针
void ActionLayersActivate::setComboBoxData(ComboBoxData* data)
{
    cbxData = data;
}
