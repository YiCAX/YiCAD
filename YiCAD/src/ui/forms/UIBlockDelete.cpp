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

/// @file UIBlockDelete.cpp
/// @brief 块删除对话框实现

#include "UIBlockDelete.h"

#include <QMessageBox>

#include "DmBlockTable.h"
#include "DmBlock.h"
#include "DmBlockReference.h"
#include "DmDocument.h"
#include "DmEntity.h"
#include "DmEntityContainer.h"
#include "EntityTable.h"
#include "Transaction.h"

UIBlockDelete::UIBlockDelete(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
    , m_pBlockTable(nullptr)
    , m_pDocument(nullptr)
    , m_pPreviewContainer(new DmEntityContainer())
{
    setupUi(this);

    // 初始化预览容器，避免 GuiPreviewWidget 初始化时容器为空
    preview->setContainer(m_pPreviewContainer);

    connect(blockListWidget, &QListWidget::currentTextChanged, this, &UIBlockDelete::slotBlockSelectionChanged);
    connect(deleteButton, &QPushButton::clicked, this, [this]
    {
        slotDeleteClicked();
    });
    connect(closeButton, &QPushButton::clicked, this, [this]
    {
        reject();
    });
}

UIBlockDelete::~UIBlockDelete()
{
    if (m_pPreviewContainer)
    {
        delete m_pPreviewContainer;
        m_pPreviewContainer = nullptr;
    }
}

void UIBlockDelete::setBlockTable(DmBlockTable* blockTable)
{
    m_pBlockTable = blockTable;
    updateBlockList();
}

void UIBlockDelete::setDocument(DmDocument* doc)
{
    m_pDocument = doc;
}

void UIBlockDelete::updateBlockList()
{
    blockListWidget->clear();
    if (!m_pBlockTable)
    {
        return;
    }

    for (auto block : *m_pBlockTable)
    {
        if (!block || block->isErased())
        {
            continue;
        }

        // 过滤匿名块
        std::wstring::size_type idx = block->getName().toStdWString().find(L"*");
        if (idx != std::wstring::npos)
        {
            continue;
        }

        // 过滤内部块
        auto strChar = block->getName().toStdWString().substr(0, 1);
        if (strChar == L"_")
        {
            continue;
        }

        blockListWidget->addItem(block->getName());
    }
    if (blockListWidget->count() > 0)
    {
        blockListWidget->setCurrentRow(0);
    }
}

void UIBlockDelete::slotBlockSelectionChanged(const QString& curBlock)
{
    if (!m_pBlockTable)
    {
        return;
    }

    // 清空预览容器
    m_pPreviewContainer->clear();

    auto block = m_pBlockTable->find(curBlock);
    if (block)
    {
        // 克隆块的实体到预览容器
        for (auto e : block->getEntityTable())
        {
            m_pPreviewContainer->addEntity(e->clone());
        }
    }

    preview->zoomAuto();
    preview->specifyModified();
    preview->update();
}

void UIBlockDelete::slotDeleteClicked()
{
    if (!m_pBlockTable || !m_pDocument)
    {
        return;
    }

    auto items = blockListWidget->selectedItems();
    if (items.isEmpty())
    {
        QMessageBox::information(this, tr("Tips"), tr("Please select a block to delete."));
        return;
    }

    QString blockName = items.first()->text();
    auto block = m_pBlockTable->find(blockName);
    if (!block)
    {
        return;
    }

    // 检查是否存在引用该块的 DmBlockReference
    if (hasReferences(blockName))
    {
        QMessageBox::warning(this, tr("Warning"),
            tr("Block \"%1\" has references in the drawing and cannot be deleted.").arg(blockName));
        return;
    }

    // 确认删除
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Confirm Delete"),
        tr("Are you sure you want to delete block \"%1\"?").arg(blockName),
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes)
    {
        return;
    }

    Transaction t(tr("Delete Block").toStdString(), m_pDocument);
    t.start();
    m_pBlockTable->remove(block);
    t.commit();

    // 刷新列表并更新预览
    updateBlockList();
    if (blockListWidget->count() > 0)
    {
        blockListWidget->setCurrentRow(0);
    }
    else
    {
        m_pPreviewContainer->clear();
        preview->specifyModified();
        preview->update();
    }
}

bool UIBlockDelete::hasReferences(const QString& blockName) const
{
    if (!m_pDocument)
    {
        return false;
    }

    auto entTable = m_pDocument->getEntityTable();
    for (auto e : *entTable)
    {
        if (e && e->getEntityType() == DM::EntityBlockReference)
        {
            DmBlockReference* ref = static_cast<DmBlockReference*>(e);
            if (ref->getName() == blockName && !ref->isErased())
            {
                return true;
            }
        }
    }
    return false;
}
