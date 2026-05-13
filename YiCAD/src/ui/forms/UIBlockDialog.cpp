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

/// @file UIBlockDialog.cpp
/// @brief 块对话框实现

#include "UIBlockDialog.h"

#include <QMessageBox>
#include <QPushButton>

#include "DmBlockTable.h"
#include "DmBlock.h"
#include "Debug.h"

UIBlockDialog::UIBlockDialog(QWidget* parent, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setModal(modal);
    setupUi(this);

    this->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    this->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
}

/// @brief 语言切换时刷新界面文本
void UIBlockDialog::languageChange()
{
    retranslateUi(this);
}

void UIBlockDialog::setBlockList(DmBlockTable* l)
{
    blockTable = l;
    if (blockTable)
    {
        DmBlock* block = blockTable->getActive();
        if (block)
        {
            leName->setText(block->getName());
        }
    }
}

DmBlockData UIBlockDialog::getBlockData()
{
    return DmBlockData(leName->text(), DmVector(0.0, 0.0), false);
}

void UIBlockDialog::validate()
{
    QString name = leName->text();

    if (!name.isEmpty())
    {
        if (blockTable && !blockTable->find(name))
        {
            accept();
        }
        else
        {
            QMessageBox::warning(this, tr("Renaming Block"), tr("Could not name block. A block named \"%1\" already exists.").arg(leName->text()), QMessageBox::Ok, Qt::NoButton);
        }
    }
}

void UIBlockDialog::cancel()
{
    leName->setText("");
    reject();
}
