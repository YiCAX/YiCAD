/*
 * Copyright (C) 2024 YiCAD Team
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

/// @file ActionBlocksSave.cpp
/// @brief 块保存动作类实现文件

#include "ActionBlocksSave.h"

#include <QAction>
#include <QApplication>

#include "ApplicationWindow.h"
#include "Debug.h"
#include "DmDocument.h"
#include "DmBlock.h"
#include "DmBlockReference.h"
#include "DmBlockTable.h"
#include "DmLineTypeTable.h"
#include "GuiDialogFactory.h"
#include "MDIWindow.h"
#include "UIFileDialog.h"

/// @brief 构造函数
ActionBlocksSave::ActionBlocksSave(
    DmDocument* doc, GuiDocumentView* docView)
    : ActionInterface("Block Save", doc, docView)
{
}

/// @brief 递归将块参照引用的块定义添加到目标文档
/// @param[in] ref 块参照指针
/// @param[in] doc 目标文档指针
void ActionBlocksSave::addBlock(
    DmBlockReference* ref, DmDocument* doc)
{
    for (auto entity : ref->getEntityList())
    {
        if (entity->getEntityType()
            == DM::EntityBlockReference)
        {
            auto* subRef =
                static_cast<DmBlockReference*>(entity);
            DmBlock* subBlock =
                subRef->getBlockForInsert();
            if (subBlock
                && !doc->getBlockTable()->find(
                    subBlock->getName()))
            {
                doc->getBlockTable()->add_direct(
                    subBlock);
            }
            addBlock(subRef, doc);
        }
    }
}

/// @brief 将源文档的线型/图层/文字样式/标注样式复制到目标文档
/// @param[in] src 源文档指针
/// @param[in] dst 目标文档指针
void ActionBlocksSave::copyStyles(
    DmDocument* src, DmDocument* dst)
{
    // 线型
    for (auto lineType : *src->getLineTypeTable())
    {
        if (!dst->getLineTypeTable()->find(
                lineType->getLineTypeName()))
        {
            dst->getLineTypeTable()->add_direct(
                new DmLineType(lineType));
        }
    }
    // 图层
    for (auto layer : *src->getLayerTable())
    {
        if (!dst->getLayerTable()->find(
                layer->getName()))
        {
            dst->getLayerTable()->add_direct(
                layer->clone());
        }
    }
    // 文字样式
    for (auto style : *src->getTextStyleTable())
    {
        if (!dst->getTextStyleTable()->find(
                style->getName()))
        {
            dst->getTextStyleTable()->add_direct(
                style->clone());
        }
    }
    // 标注样式
    for (auto style : *src->getDimStyleTable())
    {
        if (!dst->getDimStyleTable()->find(
                style->getName()))
        {
            dst->getDimStyleTable()->add_direct(
                new DmDimensionStyle(*style));
        }
    }
}

/// @brief 触发动作执行
void ActionBlocksSave::trigger()
{
    ApplicationWindow* appWindow =
        ApplicationWindow::getAppWindow();
    if (!appWindow)
    {
        finish(false);
        return;
    }

    DmBlockTable* blockTable =
        pDocument->getBlockTable();
    if (!blockTable)
    {
        finish(false);
        return;
    }

    auto activeBlock = blockTable->getActive();
    if (!activeBlock)
    {
        GUIDIALOGFACTORY->commandMessage(
            tr("No block activated to save"));
        finish(false);
        return;
    }

    // 创建临时文档用于保存
    DmDocument tmpDoc;
    tmpDoc.initDoc();

    // 复制文档级样式（块内实体可能引用这些样式）
    copyStyles(pDocument, &tmpDoc);

    // 将块作为整体添加到临时文档的块表
    tmpDoc.getBlockTable()->add_direct(activeBlock);

    // 递归添加块内嵌套引用的块定义
    for (auto entity : activeBlock->getEntityTable())
    {
        if (entity->getEntityType()
            == DM::EntityBlockReference)
        {
            addBlock(
                static_cast<DmBlockReference*>(entity),
                &tmpDoc);
        }
    }

    // 弹出文件保存对话框
    UIFileDialog dlg(
        appWindow->getMDIWindow(),
        Qt::WindowFlags(),
        UIFileDialog::BlockFile);
    QString formatType;
    QString const& fn = dlg.getSaveFile(formatType);
    if (!fn.isEmpty())
    {
        QApplication::setOverrideCursor(
            QCursor(Qt::WaitCursor));
        tmpDoc.saveAs(fn, formatType);
        QApplication::restoreOverrideCursor();
    }

    finish(false);
}

/// @brief 初始化动作
/// @param[in] status 初始状态
void ActionBlocksSave::init(int status)
{
    ActionInterface::init(status);
    trigger();
}
