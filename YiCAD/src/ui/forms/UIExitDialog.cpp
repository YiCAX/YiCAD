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

/// @file UIExitDialog.cpp
/// @brief 退出/关闭文档确认对话框

#include "UIExitDialog.h"

#include <QMessageBox>
#include <QPushButton>

#include "ui_UIExitDialog.h"

/// @brief 构造 UIExitDialog 对话框
/// 默认为非模态对话框，设置 modal 为 true 可构造模态对话框。
UIExitDialog::UIExitDialog(QWidget* parent, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, fl)
    , ui(new Ui::UIExitDialog())
{
    setModal(modal);
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    ui->buttonBox->button(QDialogButtonBox::Close)->setText(tr("Close"));
    ui->buttonBox->button(QDialogButtonBox::Save)->setText(tr("Save"));
    ui->buttonBox->button(QDialogButtonBox::SaveAll)->setText(tr("Save All"));

    this->setWindowIcon(QIcon(":/ribbon/logo.png"));

    init();
}

UIExitDialog::~UIExitDialog() = default;

/// @brief 使用当前语言设置子控件的字符串
void UIExitDialog::languageChange()
{
    ui->retranslateUi(this);
}

void UIExitDialog::init()
{
    setShowSaveAll(false);
    // set dlg icon
    QMessageBox mb("", "", QMessageBox::Question, QMessageBox::Ok, Qt::NoButton, Qt::NoButton);
    ui->l_icon->setPixmap(mb.iconPixmap());
}

void UIExitDialog::clicked(QAbstractButton* button)
{
    QDialogButtonBox::StandardButton bt = ui->buttonBox->standardButton(button);
    switch (bt)
    {
    case QDialogButtonBox::Close:
        emit accept();
        break;
    case QDialogButtonBox::Save:
        slotSave();
        break;
    case QDialogButtonBox::SaveAll:
        slotSaveAll();
        break;
    default:
        emit reject();
        break;
    }
}

void UIExitDialog::setText(const QString& text)
{
    ui->lQuestion->setText(text);
}

void UIExitDialog::setTitle(const QString& text)
{
    setWindowTitle(text);
}

void UIExitDialog::setForce(bool force)
{
    QPushButton* bCancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
    bCancel->setDisabled(force);
}

void UIExitDialog::setShowSaveAll(bool show)
{
    ui->buttonBox->button(QDialogButtonBox::SaveAll)->setVisible(show);
    QString close = show ? tr("Close All") : tr("Close");
    ui->buttonBox->button(QDialogButtonBox::Close)->setText(close);
}

void UIExitDialog::slotSaveAll()
{
    done(3);
}

void UIExitDialog::slotSave()
{
    done(2);
}
